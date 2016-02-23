#include <cmath>
#include "Experience.h"
#include "SateSpace.h"
#include "Action.h"

//utility calculation function
double q();

//reward function based off raw position and velocity
double state_reward(const double theta, const double theta_dot);

int main()
{
	//create the state space
	StateSpace space(100,50);
	
	//array to hold the current state of the robot
	double state[2]={0,0};
	
	//create pointers to the possible actions as well as pointers to hold the chosen action and the previous action
	Action* actions[2],chosen_action,last_action;
	actions[0]=new action(FORWARDS);
	actions[1]=new action(BACKWARDS);
	
	//pointer to the previous queue
	PriorityQueue<Experience*,double>* last_queue;
	
	while(true)
	{
		//evaluate state
		state[0]=getPosition();//!
		state[1]=getVelocity();//!
		
		
		//get experiences at current point
		PriorityQueue<Experience*,double>& experiences=space[state[0]][state[1]];
		
		//find rewards//!
		//choose acion//!
		
		//generate new experience based off last action
		Experience* last_experience=new Experience(last_action,state[0],state[1]);
		last_queue->enqueueWithPriority(last_experience,last_q);
		last_queue=&experiences;
		
		//take action
		chosen_action->execuate();
		
		//save last experience
		last_action=current_action
	}
	
	delete actions[0];
	delete actions[1];
	
	//state space house keeping//!
	
	return 1;
}

double q()
{
	return //something competant;
}

double state_reward(const double theta, const double theta_dot)
{
	return 180+theta_dot*theta/std::abs(theta_dot);
}
