#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "StateSpace3.h"
#include "PriorityQueue.h"
#include "State3.h"
#include "environment.h"

template<typename T> std::string to_string(T x) {
	return static_cast<std::ostringstream&>((std::ostringstream() << std::dec << x)).str();
}

//function to calculate a temperature for the select action function as a function of time
double temperature(unsigned long t);

//function to select next action
float selectAction(PriorityQueue<float, double>& a_queue, unsigned long int iterations);

//function to update a q value
void updateQ(StateSpace & space, float  action, State & new_state, State & old_state, double alpha, double gamma);

int main()
{
	//learning factor
	const double alpha = 0.5;
	//discount factor
	const double gamma = 0.5;
	const int torque_bins = 9;
	const double deltatime = 0.1;
	const double mass = 0.5;
	const double length = 0.08;
	const int maxtorque = 4;

	/*for (int i = 0; i < torque_bins; ++i) {
		const int t_i = -maxtorque + i;
	}*/

	environment* env = new environment(0, 0, 0, maxtorque, 0, deltatime, mass, length, gamma);

	//seed rng
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	//create pointers to the possible actions as well as a pointer to hold the chosen action
	float chosen_action = 1.0f;
	float actions[torque_bins];
	for (int i = 0; i < (maxtorque*2)+1 ; ++i) {
		actions[i] = static_cast<float>(-maxtorque+i);
	}

	//create a priority queue to copy to all the state space priority queues
	PriorityQueue<float, double> initiator_queue(MAX);
	for (int i = 0; i < torque_bins; ++i) {
		initiator_queue.enqueueWithPriority(actions[i], 0);
	}

	//create the state space
	StateSpace space(initiator_queue, 100, 50, torque_bins);
	space.setAngleBins(100);
	space.setVelocityBins(50);
	space.setTorqueBins(torque_bins);
	space.setTorqueMax(maxtorque);

	//state objects
	State current_state(0, 0, 0);
	State old_state(0, 0, 0);

	std::ofstream file("output.txt");
	file.precision(16);

	file << "Trialno" << "	" << "Time" << "		" << "Theta" << "	" << "Thetadot" << "		" << "Torque" << std::endl;

	double trialno = 1;
	unsigned long i=0UL;
	while (true)
	{
		current_state.theta = env->getTheta();
		current_state.theta_dot = env->getThetadot();
		current_state.torque = env->getTorque();
		//current_state.time=env->getTime();
		std::cout << "before updateQ" << std::endl;
		updateQ(space, chosen_action, old_state, current_state, alpha, gamma);

		if (current_state.theta>2 * 3.1415 && current_state.theta_dot>2 * 3.1415 && env->getTime() >= 10) {
			env->resetPendulum();
			std::domain_error("unsuccessful trial");
			trialno++;
			i = 0;
		}

		std::cout << "before selectAction" << std::endl;
		old_state = current_state;
		chosen_action = selectAction(space[current_state],i);
		env->setTorque(chosen_action);
		env->propagate();

		file << trialno << "	  " << env->getTime() << "	  " << current_state.theta << "	  " << current_state.theta_dot << "	   " << current_state.torque << std::endl;
		++i;
	}

	file.close();
	delete env;
	return 1;
}

double temperature(unsigned long t)
{
	return 100000.0*std::exp((-8.0*t*t)/(26000.0*26000.0))+0.1;
}

//function is fed with a priority queue of action-values 
//generates Boltzmann distribution of these action-values
//and selects an action based on probabilities 
float selectAction(PriorityQueue<float, double>& a_queue, unsigned long int iterations)
{
	typedef PriorityQueue<float, double> PQ;
	typedef std::vector< std::pair<float, double> > Vec_Pair;
	typedef std::pair<float, double> Pair;

	double sum = 0;
	int i = 0;

	int size = a_queue.getSize();
	Vec_Pair action_vec(size);

	//Calculate partition function by iterating over action-values
	for (PQ::const_iterator iter = a_queue.begin(), end = a_queue.end(); iter < end; ++iter)
	{
		sum += std::exp((iter->second) / temperature(iterations));
	}
	//Calculate boltzmann factors for action-values
	for (Vec_Pair::iterator it = action_vec.begin(), end = action_vec.end(); it < end; ++it)
	{
		it->first = a_queue[i].first;
		it->second = std::exp(a_queue[i].second / temperature(iterations)) / sum;
		++i;
	}

	//calculate cumulative probability distribution    
	for (std::vector< std::pair<float, double> >::iterator it1 = action_vec.begin()++, it2 = action_vec.begin(), end = action_vec.end(); it1 < end; ++it1, ++it2)
	{
		it1->second += it2->second;
	}

	//generate RN between 0 and 1
	double rand_num = (double)rand() / (RAND_MAX);

	//select action based on probability 
	for (Vec_Pair::iterator it = action_vec.begin(), end = action_vec.end(); it < end; ++it)
	{
		//if RN falls within cumulative probability bin return the corresponding action
		if (rand_num < it->second)return it->first;
	}

	return NULL; //note that this line should never be reached
}

void updateQ(StateSpace & space, float action, State & new_state, State & old_state, double alpha, double gamma)
{
	//std::cout << action << std::endl;
	//std::cout << space[old_state].toString() << std::endl << "-----------";

	//oldQ value reference
	double oldQ = space[old_state].search(action).second;

	//reward given to current state 
	double R = new_state.getReward();
	std::cout << "Reward: " << R << std::endl;
	//optimal Q value for new state i.e. first element 
	double maxQ = space[new_state].peekFront().second;

	//new Q value determined by Q learning algorithm
	double newQ = oldQ + alpha * (R + (gamma * maxQ) - oldQ);
	//std::cout << "New Q-Value: " << newQ << std::endl;
	space[old_state].changePriority(action, newQ);
}

