#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Experience.h"
#include "SateSpace.h"
#include "Action.h"

//utility calculation function
double q(int action, int x, int y);

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
		double p=rand();
		
		if( p < std::exp( q(current_state, action0)/k(time) ) )
		
		
	}
	
	delete actions[0];
	delete actions[1];
	
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

bool selectAction(a, b)
{	std::vector<double []> action_vec;
	for(PriorityQueue<T,PT>::constIter iter = queue.begin(); iter < queue.end(); iter++)
	{
		sum += std::exp((*iter).second/k)
	}
	for(std::vector<double []>::iterator it = action_vec.begin(); it < action_vec.end(); iter++)
	{
		action_vec[iter][]
	}
	std::sort(action_vec.begin(), action_vec.end(),
          [](const double a[], const double b[]) {
        return a[1] < b[1];
});

	
}

