#include "StateSpace.h"

int StateSpace::angle_bins=0;
int StateSpace::velocity_bins=0;

// _angle_max and _velocity_max are not declared.
StateSpace::StateSpace(int _angle_bins, int _velocity_bins, const PriorityQueue<int,double>& queue):
	space1(_angle_bins, std::vector< PriorityQueue<int,double> > (_velocity_bins, PriorityQueue<int,double> (queue))),
	space2(_angle_bins, std::vector< PriorityQueue<int,double> > (_velocity_bins, PriorityQueue<int,double> (queue)))
{}

StateSpace::SubscriptProxy1 StateSpace::operator[](const unsigned int robot_state)
{
	//throw if the the index is out of bounds
	if(robot_state>1)throw std::domain_error("action index exceeded");
	//return proxy object to accept second [] operator
	return SubscriptProxy1( robot_state ? space1 : space2 );
}

//searches state space by state object
PriorityQueue<int, double>& StateSpace::operator[](const State & state)
{
	//call the subscript operators with the members of the state object
	return (*this)[state.robot_state][state.theta][state.theta_dot];
}

void StateSpace::setAngleBins(const double val)
{
	angle_bins=val;
}

void StateSpace::setVelocityBins(const double val)
{
	velocity_bins=val;
}
