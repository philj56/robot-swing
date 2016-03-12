#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "StateSpace.h"
#include "PriorityQueue.h"
#include "State.h"
#include "Environment.h"

//function to calculate a temperature for the select action function as a function of time
double temperature(unsigned long t);

//function to select next action
float selectAction(PriorityQueue<float, double>& a_queue, unsigned long iterations);

//function to update a q value
void updateQ(StateSpace & space, float  action, State & new_state, State & old_state, double alpha, double gamma);

int main()
{
	//learning factor
	const double alpha = 0.5;
	//discount factor
	const double gamma = 0.5;
	
	const double deltatime = 0.1;
	const double mass = 0.5;
	const double length = 0.08;
	const int angle_bins = 100;
	const int velocity_bins = 50;
	const int torque_bins = 9;
	const int max_angle = M_PI;
	const int max_velocity = 10;
	const int max_torque = 4;
	
	Environment* env = new Environment(0, 0, 0, max_torque, 0, deltatime, mass, length, gamma);
	
	//seed rng
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	
	//create the possible actions as well as the chosen action
	float chosen_action = 1;	// rip f
	float actions[torque_bins];
	for (int i = 0; i < (max_torque*2)+1 ; ++i)
	{
		actions[i] = static_cast<float>(-max_torque+i);
	}
	
	//create a priority queue to copy to all the state space priority queues
	PriorityQueue<float, double> initiator_queue(MAX);
	for (int i = 0; i < torque_bins; ++i)
	{
		initiator_queue.enqueueWithPriority(actions[i], 0);
	}
	
	//create the state space
	StateSpace space(initiator_queue, angle_bins, velocity_bins, torque_bins, max_angle, max_velocity, max_torque);
	
	//state objects
	State current_state(0, 0, 0);
	State old_state(0, 0, 0);
	
	std::ofstream file("output.txt");
	file.precision(16);
	
	file << "Trialno" << "	" << "Time" << "		" << "Theta" << "	" << "Thetadot" << "		" << "Torque" << std::endl;
	
	double trialno = 1;
	unsigned long i=0;
	while (true)
	{
		current_state.theta = env->getTheta();
		current_state.theta_dot = env->getThetadot();
		current_state.torque = env->getTorque();
		
		std::cout << "State Read" << std::endl;
		
		updateQ(space, chosen_action, old_state, current_state, alpha, gamma);
		
		std::cout << "Q Updated" << std::endl;

		if (current_state.theta>2 * 3.1415 && current_state.theta_dot>2 * 3.1415 && env->getTime() >= 10) {
			env->resetPendulum();
			std::cout<<"->unsuccessful trial\n";
			trialno++;
			i = 0;
			continue;
		}
		
		old_state = current_state;
		
		chosen_action = selectAction(space[current_state],i);
		std::cout << "Action Selected" << std::endl;
		env->setTorque(chosen_action);
		
		env->propagate();
		std::cout << "Environment Propagated\n" << std::endl;

		file << trialno << "	  " << env->getTime() << "	  " << current_state.theta << "	  " << current_state.theta_dot << "	   " << current_state.torque << std::endl;
		++i;
	}

	file.close();
	delete env;
	return 1;
}

double temperature(unsigned long t)
{
	return 100.0*std::exp((-8.0*t*t)/(2600.0*2600.0))+0.1;
}

//function is fed with a priority queue of action-values 
//generates Boltzmann distribution of these action-values
//and selects an action based on probabilities 
float selectAction(PriorityQueue<float, double>& a_queue, unsigned long iterations)
{
	typedef std::vector<std::pair<float, double>> VecPair ; 
	
	//turn priority queue into a vector of pairs
	VecPair vec = a_queue.saveOrderedQueueAsVector();
	
    //sum for partition function 
	double sum = 0;
	
	// calculate partition function by iterating over action-values
	for (VecPair::iterator iter = vec.begin(), end = vec.end(); iter < end; ++iter)
	{
		sum += std::exp((iter->second) / temperature(iterations));
	}
	
	//compute Boltzmann factors for action-values and enqueue to vec
	for (VecPair::iterator iter = vec.begin(); iter < vec.end(); ++iter)
	{
		iter->second = std::exp(iter->second / temperature(iterations)) / sum;
	}
	
	//calculate cumulative probability distribution
	for (VecPair::iterator iter = vec.begin()++, end = vec.end(); iter < end; ++iter)
	{
	//second member of pair becomes addition of its current value
	//and that of the index before it
	iter->second += (iter-1)->second;
	}
	
	//generate RN between 0 and 1
	double rand_num = static_cast<double>(rand()) / RAND_MAX;
	
	// choose action based on random number relation to priorities within action queue
	for (VecPair::iterator iter = vec.begin(), end = vec.end(); iter < end; ++iter)
	{
		if (rand_num < iter->second)return iter->first;
	}
	
	return -10; //note that this line should never be reached	
}

void updateQ(StateSpace & space, float action, State & new_state, State & old_state, double alpha, double gamma)
{
	//oldQ value reference
	double oldQ = space[old_state].search(action).second;
	
	//reward given to current state 
	double R = new_state.getReward();
	
	//optimal Q value for new state i.e. first element 
	double maxQ = space[new_state].peekFront().second;
	
	//new Q value determined by Q learning algorithm
	double newQ = oldQ + alpha * (R + (gamma * maxQ) - oldQ);
	
	space[old_state].changePriority(action, newQ);
}
