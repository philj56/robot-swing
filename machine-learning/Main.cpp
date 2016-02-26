#include <cmath>
#include <cstdlib>
#include <ctime>
#include "StateSpace.h"
#include "Action.h"
#include "PriorityQueue.h"
#include "State.h"

//utility calculation function
double q();

Action * selectAction(PriorityQueue<Action *,double>& a_queue, double temp);

void updateQ(StateSpace & space, Action & action, State & new_state, State & old_state, double alpha, double gamma);

int main()
{
	std::srand(std::time(NULL));
	
	//create the state space
	StateSpace space(100,50);
	
	//create pointers to the possible actions as well as pointers to hold the chosen action and the previous action
//	Action* chosen_action;
	Action* actions[2];
	actions[0]=new Action(FORWARD);
	actions[1]=new Action(BACKWARD);

	//state objects
	State current_state(0,0,FORWARD);
	State old_state(0,0,FORWARD);
	State new_state;
	
	while(true)
	{
		updateQ();
		
		old_state=current_state;
		
		chosen_action=selectAction(space.stateSearch(current_state));
		
		chosen_action.execute();
		
		current_state.theta=getAngle();
		current_state.theta_dot=getVelocity();
		current_state.robot_state=chosen_action.action;
	}
	
	delete actions[0];
	delete actions[1];
	
	return 1;
}

//function is fed with a priority queue of action-values 
//generates Boltzmann distribution of these action-values
//and selects an action based on probabilities 
Action * selectAction(PriorityQueue<Action *,double>& a_queue, double temp)
{	
	typedef PriorityQueue<Action *,double> PQ;
	typedef std::vector< std::pair<Action *, double> > Vec_Pair;
	typedef std::pair<Action *, double> Pair;
	
	double sum= 0;
	int i = 0;
	
	int size = a_queue.getSize();
	Vec_Pair action_vec(size);
	Pair pear;
	
	//Calculate partition function by iterating over action-values
	for(auto iter = a_queue.begin(),end=a_queue.end(); iter < end; ++iter)
	{
		sum += std::exp((iter->second)/temp);
	}
	//Calculate boltzmann factors for action-values
	for(Vec_Pair::iterator it = action_vec.begin(),end=action_vec.end(); it < end; ++it)
	{
	    it->first = a_queue[i].first;
	    it->second = std::exp(a_queue[i].second / temp) / sum;
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

void updateQ(StateSpace & space, Action & action, State & new_state,
                 State & old_state, double alpha, double gamma)
{
    //oldQ value 
    double oldQ = space.StateSearch(old_state).search(action).second;
    //reward given to current state 
    double R = new_state.getReward();
    //optimal Q value for new state i.e. first element 
    double maxQ = space.StateSearch(current_state)[0].second;
    
    //new Q value determined by Q learning algorithm
    double newQ = oldQ + alpha * (R + (gamma * maxQ) - oldQ;
    
    //updates Q value
    space.StateSearch(old_state).search(action).second = newQ;
}
