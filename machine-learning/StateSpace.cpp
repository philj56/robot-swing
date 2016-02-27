#include "StateSpace.h"

StateSpace::StateSpace(const unsigned int _angle_max, const unsigned int _velocity_max, PriorityQueue<Action*,double> queue ):
	angle_max(_angle_max),
	velocity_max(_velocity_max),
	space(_angle_max, std::vector< PriorityQueue<Action*,double> > (_velocity_max, PriorityQueue<Action*,double> (queue)))
{}

StateSpace::~StateSpace()
{
	//ridiculously extensive clean-up
	for(unsigned int i=0 ; i<angle_max ; ++i)
	{
		for(unsigned int j=0 ; j<velocity_max ; ++j)
		{
			PriorityQueue<Action*,double>& queue1=space1[i][j];
			PriorityQueue<Action*,double>& queue2=space2[i][j];
			
			for(auto iter=queue1.begin(),queue1=list.end() ; iter!=end ; ++iter)
			{
				delete *iter;
			}
			
			for(auto iter=queue2.begin(),queue2=list.end() ; iter!=end ; ++iter)
			{
				delete *iter;
			}
		}
	}
}

StateSpace::SubscriptProxy1 StateSpace::operator[](const unsigned int robot_state)
{
	if(robot_state>1)throw std::domain_error("action index exceeded");
	//return proxy object to accept second [] operator
	return SubscriptProxy1( robot_state ? space1 : space2 );
}

//searches state space by state object
PriorityQueue<Action *, double>& StateSpace::operator[](State & state)
{
	return (*this)[state.RobotState][state.theta][state.theta_dot];
}
