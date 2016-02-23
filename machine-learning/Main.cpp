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
	
	//a pointer to the last experience
	Experience* last_experience;
	
	//create pointers to the possible actions as well as a pointer to hold the chosen action
	Action* actions[2],chosen_action;
	actions[0]=new action(FORWARDS);
	actions[1]=new action(BACKWARDS);
	
	while(true)
	{
		//evaluate state
		state[0]=getPosition();//!
		state[1]=getVelocity();//!
		
		//add to last experience
		last_experience.state[0]=state[0];
		last_experience.state[1]=state[1];
		
		//get experiences at current point
		std::list<Experience*>& experiences=space[state[0]][state[1]];
		
		//find rewards//!
		//choose acion//!
		
		//generate new experience
		Experience* current_experience=new Experience(chosen_action);
		experiences.push_back(current_experience);
		//delete lowest value experience//!
		
		//take action
		chosen_action->execuate();
		
		//save last experience
		last_experience=current_experience
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
