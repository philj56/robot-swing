#include "StateSpace3.h"

StateSpace::StateSpace(const unsigned int _angle_max, const unsigned int _velocity_max, PriorityQueue<Action*,double> queue ):
	angle_max(_angle_max),
	velocity_max(_velocity_max),
	space(_angle_max, std::vector< PriorityQueue<Action*,double> > (_velocity_max, PriorityQueue<Action*,double> (queue)))
{}

StateSpace::~StateSpace()
{
	//currently no cleanup required
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
	return (*this)[state.robot_state][state.theta][state.theta_dot];
}
