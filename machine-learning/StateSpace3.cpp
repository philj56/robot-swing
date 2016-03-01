#include "StateSpace3.h"

StateSpace::StateSpace(const unsigned int _angle_bins, const unsigned int _velocity_bins, const unsigned int _torque_bins, PriorityQueue<Action*,double> queue):
	angle_bins(_angle_bins),
	velocity_bins(_velocity_bins),
	torque_bins(_torque_bins),
	space( _angle_bins, std::vector<std::vector<PriorityQueue<Action*,double>>> ( _velocity_bins, std::vector<PriorityQueue<Action*,double>> ( _torque_bins, PriorityQueue<Action*,double> (queue) ) ) )
	
{}

StateSpace::SubscriptProxy1 StateSpace::operator[](const double angle)
{
	//error if angle exceeds bounds
	if(std::abs(angle)>2*M_PI)throw std::domain_error("angle argument exceeded");
	//descretise index
	int discrete_index=std::round(angle*100/angle_bins)+angle_bins/2;
	
	//return appropriate object
	return SubscriptProxy1(space[discrete_index]);
}

//searches state space by state object
PriorityQueue<Action *, double>& StateSpace::operator[](const State & state)
{
	//call the subscripts with the members of the state object
	return (*this)[state.theta][state.theta_dot][state.torque];
}
