#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Experience.h"
#include "StateSpace.h"
#include "Action.h"

//utility calculation function
double q();

//reward function based off raw position and velocity
double state_reward(const double theta, const double theta_dot);

int main()
{
	std::srand(std::time(NULL));
	
	//create the state space
	StateSpace space(100,50);
	
	//array to hold the current state of the robot
	double current_state[2]={0,0};
	double last_state[2]={0,0};
	
	//create pointers to the possible actions as well as pointers to hold the chosen action and the previous action
	Action* actions[2],chosen_action;
	actions[0]=new action(FORWARDS);
	actions[1]=new action(BACKWARDS);
	
	//evaluate state
	current_state[0]=getPosition();//!
	current_state[1]=getVelocity();//!
	
	while(true)
	{
		//evaluate q of last action
		//current_state>>old_state
		
		chosen_action=selectAction(/*...*/);
		
		//>>current_state
	}
	
	delete actions[0];
	delete actions[1];
	
	return 1;
}

double q()
{
	return //StateSpace[originalstate[0],originalstate[1]].GetExperience().GetUtility(action) + learningrate(state_reward(newstate[0], newstate[1]) + gamma(StateSpace[newstate[0],newstate[1]].GetExperience().GetHighestUtility()) - StateSpace[originalstate[0],originalstate[1]].GetExperience().GetUtility(action); // Utility calculation pseudocode
}

double state_reward(const double theta, const double theta_dot)
{
	return 180+theta_dot*theta/std::abs(theta_dot);
}

//function is fed with a priority queue of action-values 
//generates Boltzmann distribution of these action-values
//and selects an action based on probabilities 
Action * selectAction(PriorityQueue<Action *,double> a_queue, double temp)
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
	for(PQ::constIter iter = a_queue.begin(); iter < a_queue.end(); iter++)
	{
		sum += std::exp((iter->second)/temp);
	}
	//Calculate boltzmann factors for action-values
	for(Vec_Pair::iterator it = action_vec.begin(); it < action_vec.end(); it++)
	{
	    it->first = a_queue[i].first;
	    it->second = std::exp(a_queue[i].second / temp) / sum;
	    i++;
	}
	//calculate cumulative probability distribution    
    for(std::vector< std::pair<int, double> >::iterator it = action_vec.begin()+1; it < action_vec.end(); it++)
	{
	    it->second += (it-1)->second;
	}
    //seed random number generator
	srand(time(NULL));
	//generate RN between 0 and 1 
	double rand_num = (double)rand()/ (RAND_MAX);
	
	//select action based on probability 
    for(Vec_Pair::iterator it = action_vec.begin(); it < action_vec.end(); it++)
    {
        //if RN falls within cumulative probability bin return the corresponding action
        if(rand_num < it->second){return it->first};
    }
    return -1;
}
