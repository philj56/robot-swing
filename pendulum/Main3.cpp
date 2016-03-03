#include <cmath>
#include <cstdlib>
#include <ctime>
#include "StateSpace.h"
#include "Action.h"
#include "PriorityQueue.h"
#include "State.h"

//function to calculate a temperature for the select action function as a function of time
double temperature();

//function to select next action
Action * selectAction(PriorityQueue<Action *,double>& a_queue);

//function to update a q value
void updateQ(StateSpace & space, Action & action, State & new_state, State & old_state, double alpha, double gamma);

int main()
{
	//learning factor
	const double alpha=0.5;
	//discount factor
	const double gamma=0.5;
	const int torque_bins=9;
	

	for (int i=0, i++, i<torque_bins){
		const int t_i=-4+i
	}

	
	//seed rng
	std::srand(std::time(NULL));
	
	//create pointers to the possible actions as well as a pointer to hold the chosen action
	Action* chosen_action;
	Action* actions[torque_bins];
	for (int i=0, i++, i<torque_bins){
		actions[i]=new Action(t_i);
	}

	//create a priority queue to copy to all the state space priority queues
	PriorityQueue<Action,double> initiator_queue(MAX);
	for (int i=0, i++, i<torque_bins){
		ititiator_queue.enqueueWithPriority(actions[i],0);
	}
	
	//create the state space
	StateSpace space(initiator_queue);
	space.setAngleBins(100);
	space.setVelocityBins(50);
	space.setTorqueBins(10);
	
	//state objects
	State current_state(0,0,t_6);
	State old_state(0,0,t_6);
	
	//timing variables
	double loop_start_time;

	while(true)
	{
		current_state.theta=getAngle();
		current_state.theta_dot=getVelocity();
		current_state.torque=getTorque();
		current_state.robot_state=chosen_action.action;

		if(current_state.theta>2*M_PI & current_state.theta_dot>2*M_PI & environment.getTime()>=10){
			environment.resetPendulum();
			std::domain_error("unsuccessful trial")
		}

		updateQ(space, chosen_action, old_state, current_state, alpha, gamma);

		old_state=current_state;+
		chosen_action=selectAction(space[current_state]);
		chosen_action.execute();


	}	
	delete actions[0];
	delete actions[1];
	
	
	return 1;
}

double temperature()
{
	return std::time(NULL);
}

//function is fed with a priority queue of action-values 
//generates Boltzmann distribution of these action-values
//and selects an action based on probabilities 
Action * selectAction(PriorityQueue<Action *,double>& a_queue)
{	
	typedef PriorityQueue<Action *,double> PQ;
	typedef std::vector< std::pair<Action *, double> > Vec_Pair;
	typedef std::pair<Action *, double> Pair;
	
	double sum= 0;
	int i = 0;
	
	int size = a_queue.getSize();
	Vec_Pair action_vec(size);
	
	//Calculate partition function by iterating over action-values
	for(PQ::const_iterator iter = a_queue.begin(),end=a_queue.end(); iter < end; ++iter)
	{
		sum += std::exp((iter->second)/temperature());
	}
	//Calculate boltzmann factors for action-values
	for(Vec_Pair::iterator it = action_vec.begin(),end=action_vec.end(); it < end; ++it)
	{
	    it->first = a_queue[i].first;
	    it->second = std::exp(a_queue[i].second /temperature()) / sum;
	    ++i;
	}
	
	//calculate cumulative probability distribution    
	for(std::vector< std::pair<int, double> >::iterator it1 = action_vec.begin()++,it2 = action_vec.begin(),end=action_vec.end(); it < end; ++it1,++it2)
	{
	    it1->second += it2->second;
	}
	
	//generate RN between 0 and 1
	double rand_num = (double)rand()/ (RAND_MAX);
	
	//select action based on probability 
	for(Vec_Pair::iterator it = action_vec.begin(),end=action_vec.end(); it < end; ++it)
	{
		//if RN falls within cumulative probability bin return the corresponding action
		if(rand_num < it->second)return it->first;
	}
 	
	return NULL; //note that this line should never be reached
}

void updateQ(StateSpace & space, Action & action, State & new_state, State & old_state, double alpha, double gamma)
{
    //oldQ value reference
    double oldQ = space[old_state].search(action).second;
    
    //reward given to current state 
    double R = new_state.getReward();
    
    //optimal Q value for new state i.e. first element 
    double maxQ = space[current_state].peekFront().second;
    
    //new Q value determined by Q learning algorithm
    double newQ = oldQ + alpha * (R + (gamma * maxQ) - oldQ);
    
    space[old_state].changePriority(action, newQ);
}
