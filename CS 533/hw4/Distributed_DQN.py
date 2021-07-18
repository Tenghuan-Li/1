#Tenghuan Li
#933638707; liten@oregonstate.edu
#6/4/2021

import torch
import time
import os
import ray

from tqdm import tqdm
from random import uniform, randint

from dqn_model import DQNModel
from dqn_model import _DQNModel
from memory import ReplayBuffer

import matplotlib.pyplot as plt

FloatTensor = torch.FloatTensor

from memory_remote import ReplayBuffer_remote
from dqn_model import _DQNModel
import torch
from custom_cartpole import CartPoleEnv

# Set the Env name and action space for CartPole
ENV_NAME = 'Distributed_CartPole'
ACTION_DICT = {
    "LEFT": 0,
    "RIGHT":1
}


# Set result saveing floder
the_env_simulator = CartPoleEnv()
result_floder = ENV_NAME
result_file = ENV_NAME + "/results88.txt"
if not os.path.isdir(result_floder):
    os.mkdir(result_floder)
torch.set_num_threads(12)

#The Help function
def plot_result(total_rewards ,learning_num, legend):
    print("\nLearning Performance:\n")
    episodes = []
    for i in range(len(total_rewards)):
        episodes.append(i * learning_num + 1)
        
    plt.figure(num = 1)
    fig, ax = plt.subplots()
    plt.plot(episodes, total_rewards)
    plt.title('performance')
    plt.legend(legend)
    plt.xlabel("Episodes")
    plt.ylabel("total rewards")
    plt.savefig("Distributed_DQN_8cw8ew.png")
    plt.show()

hyperparams_CartPole = {
    'epsilon_decay_steps' : 100000, 
    'final_epsilon' : 0.1,
    'batch_size' : 32, 
    'update_steps' : 10, 
    'memory_size' : 2000, 
    'beta' : 0.99, 
    'model_replace_freq' : 2000,
    'learning_rate' : 0.0003,
    'use_target_model': True
}

#init Ray
ray.shutdown()
ray.init(include_webui=False, ignore_reinit_error=True, redis_max_memory=50000000, object_store_memory=500000000)


#use memory_remote.py to set the memory server
#The_memoryserver = ReplayBuffer_remote.remote(2000)

#come frome hw4 part1 part:
class DQN_agent(object):
    def __init__(self, env, hyper_params, action_space = len(ACTION_DICT)):
        self.env = env
        self.max_episode_steps = env._max_episode_steps
        self.beta = hyper_params['beta']
        self.initial_epsilon = 1
        self.final_epsilon = hyper_params['final_epsilon']
        self.epsilon_decay_steps = hyper_params['epsilon_decay_steps']
        self.episode = 0
        self.steps = 0
        self.best_reward = 0
        self.learning = True
        self.action_space = action_space
        state = env.reset()
        input_len = len(state)
        output_len = action_space
        self.eval_model = DQNModel(input_len, output_len, learning_rate = hyper_params['learning_rate'])
        self.use_target_model = hyper_params['use_target_model']
        if self.use_target_model:
            self.target_model = DQNModel(input_len, output_len)
        #memory: Store and sample experience replay.
        #self.memory = ReplayBuffer(hyper_params['memory_size'])
        self.batch_size = hyper_params['batch_size']
        self.update_steps = hyper_params['update_steps']
        self.model_replace_freq = hyper_params['model_replace_freq']
        
    # Linear decrease function for epsilon
    def linear_decrease(self, initial_value, final_value, curr_steps, final_decay_steps):
        decay_rate = curr_steps / final_decay_steps
        if decay_rate > 1:
            decay_rate = 1
        return initial_value - (initial_value - final_value) * decay_rate
    
    def explore_or_exploit_policy(self, state):
        p = uniform(0, 1)
        # Get decreased epsilon
        epsilon = self.linear_decrease(self.initial_epsilon, 
                               self.final_epsilon,
                               self.steps,
                               self.epsilon_decay_steps) 
        if p < epsilon:
            #return action
            return randint(0, self.action_space - 1)
        else:
            #return action
            return self.greedy_policy(state)  
    def greedy_policy(self, state):
        return self.eval_model.predict(state)

# Set the DQN server
@ray.remote
class DQN_server(DQN_agent):
    def __init__(self, env, hyper_params, training_episodes, test_interval, memory):
        super().__init__(env, hyper_params)
        self.spisode = 0
        self.results = []
        self.Train_esisodes = training_episodes
        self.Test_interval = test_interval
        self.collector_completed = False
        self.evaluator_completed = False
        self.Mem_serv = memory
        self.prev_network = []
        self.reuslt_count = 0
        self.results = [0] * (training_episodes // test_interval + 1)
        
    def evaluation_model(self):
        if self.episode >= self.Train_esisodes:
            self.collector_completed = True
        return self.collector_completed
    
    def update_batch(self):
        the_new_batch = ray.get(self.Mem_serv.sample.remote(self.batch_size))
        if not the_new_batch:
            return
        (states, actions, reward, next_states, is_terminal) = the_new_batch
        states = states
        next_states = next_states
        terminal = FloatTensor([0 if t else 1 for t in is_terminal])
        reward = FloatTensor(reward)
        the_new_batch_index = torch.arange(self.batch_size, dtype=torch.long)
        
        # get the current Q values
        _, q_values = self.eval_model.predict_batch(states)
        q_values = q_values[the_new_batch_index, actions]
        
        # the set of calculate the target:
        if self.use_target_model:
            actions, q_next = self.target_model.predict_batch(next_states)
        else:
            actions, q_next = self.eval_model.predict_batch(next_states)

        q_target = reward + self.beta * torch.max(q_next, dim=1)[0] * terminal

        self.eval_model.fit(q_values, q_target)

    def learn(self, steps):
        self.episode += 1
        if self.evaluation_model():
            return True
        if self.episode // self.Test_interval + 1 > len(self.prev_network):
            self.prev_network.append(self.eval_model)
        for _ in range(steps):
            self.steps += 1
            if self.steps % self.update_steps == 0:
                self.update_batch()

            if self.steps % self.model_replace_freq == 0:
                self.target_model.replace(self.eval_model)
        return self.collector_completed

    def ask_evaluation(self):
        if len(self.prev_network) > self.reuslt_count:
            number = self.reuslt_count
            evaluation = self.prev_network[number]
            self.reuslt_count += 1
            return False, number
        else:
            if self.episode >= self.Train_esisodes:
                self.evaluator_completed = True
            return self.evaluator_completed, None
    
    def add_result(self, result, number):
        self.results[number] = result
    
    def get_results(self):
        return self.results

# Set the collecting worker
@ray.remote
def collecting_worker(model, memory, env, max_episode_steps):
    The_train = False
    while True:
        if The_train:
            break
        state = env.reset()
        done = False
        steps = 0
        while steps < max_episode_steps and not done:
            steps += 1
            a = ray.get(model.explore_or_exploit_policy.remote(state))
            s_, reward, done, info = env.step(a)
            # add experience from explore-exploit policy to memory
            ray.get(memory.add.remote(state, a, reward, s_, done))
            state = s_
        The_train = ray.get(model.learn.remote(steps))

# Set the evaluation worker
@ray.remote
def evaluation_worker(model, env, max_episode_steps, trials = 30):
    while True:
        The_ev_done, Num = ray.get(model.ask_evaluation.remote())
        if The_ev_done:
            break
        if not Num:
            continue
        total_reward = 0
        for _ in range(trials):
            state = env.reset()
            The_ev_done = False
            steps = 0

            while steps < max_episode_steps and not The_ev_done:
                steps += 1
                action = ray.get(model.greedy_policy.remote(state))
                state, reward, The_ev_done, _ = env.step(action)
                total_reward += reward

        avg_reward = total_reward / trials
        print(Num, " avg_reward: ", avg_reward)
        f = open(result_file, "a+")
        f.write(str(avg_reward) + "\n")
        f.close()
        model.add_result.remote(avg_reward, Num)

# Set the distributed DQN agent
class Distributed_DQN_agent():
    def __init__(self, env, hyper_params, training_episodes, test_interval, cw_num = 4, ew_num = 4):
        self.Mem_serv = ReplayBuffer_remote.remote(hyper_params['memory_size'])
        self.model_server = DQN_server.remote(env, hyper_params, training_episodes, test_interval, self.Mem_serv)
        self.env = env
        self.max_episode_steps = env._max_episode_steps
        self.cw_num = cw_num
        self.ew_num = ew_num
        
    def learn_and_evaluate(self):
        workers_id = []
        # learn
        for _ in range(cw_num):
            workers_id.append(collecting_worker.remote(self.model_server, self.Mem_serv, self.env, self.max_episode_steps))
        # evaluate
        for _ in range(ew_num):
            workers_id.append(evaluation_worker.remote(self.model_server, self.env, self.max_episode_steps))
        ray.wait(workers_id, len(workers_id))
        
        return ray.get(self.model_server.get_results.remote())

#  Main part for part2
the_env_simulator.reset()
# collecter workers
cw_num = 8
# evaluater workers
ew_num = 8
training_episodes, test_interval = 5000, 50
start_time = time.time()
agent = Distributed_DQN_agent(the_env_simulator, hyperparams_CartPole, training_episodes, test_interval, cw_num, ew_num)
result = agent.learn_and_evaluate()

result = result[1:-1]
print("running time: ", time.time()-start_time)
plot_result(result, test_interval, ["Distributed DQN"])