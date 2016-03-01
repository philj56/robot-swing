#include "StateSpace.h"

StateSpace::StateSpace(const unsigned int _angle_bins, const unsigned int _velocity_bins, PriorityQueue<Action*,double> queue ):
	angle_bins(_angle_bins),
	velocity_bins(_velocity_bins),
	space(_angle_max, std::vector< PriorityQueue<Action*,double> > (_velocity_max, PriorityQueue<Action*,double> (queue)))
{}

StateSpace::SubscriptProxy1 StateSpace::operator[](const unsigned int robot_state)
{
	//throw if the the index is out of bounds
	if(robot_state>1)throw std::domain_error("action index exceeded");
	//return proxy object to accept second [] operator
	return SubscriptProxy1( robot_state ? space1 : space2 );
}

//searches state space by state object
PriorityQueue<Action *, double>& StateSpace::operator[](const State & state)
{
	//call the subscript operators with the members of the state object
	return (*this)[state.robot_state][state.theta][state.theta_dot];
}
