//for pendulum

#include "StateSpace3.h"

StateSpace::StateSpace(PriorityQueue<Action*,double> queue):
	space( _angle_bins, std::vector<std::vector<PriorityQueue<Action*,double>>> ( _velocity_bins, std::vector<PriorityQueue<Action*,double>> ( _torque_bins, PriorityQueue<Action*,double> (queue) ) ) )
	
{}

void setAngleBins(const double val)
{
	angle_bins=val;
}

void setVelocityBins(const double val)
{
	velocity_bins=val;
}

void setTorqueBins(const double val)
{
	torque_bins=val;
}

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
