#include "StateSpace.h"

int StateSpace::angle_bins;
int StateSpace::velocity_bins;
int StateSpace::torque_bins;
double StateSpace::angle_max;
double StateSpace::velocity_max;
double StateSpace::torque_max;

StateSpace::StateSpace(const PriorityQueue<float, double>& queue, int _angle_bins, int _velocity_bins, int _torque_bins, double _angle_max, double _velocity_max, double _torque_max) :
	space(_angle_bins, std::vector<std::vector<PriorityQueue<float, double>>>(_velocity_bins, std::vector<PriorityQueue<float, double>>(_torque_bins, PriorityQueue<float, double>(queue))))

{
	angle_bins=_angle_bins-1; //offset is to account for array indexes starting at 0
	velocity_bins=_velocity_bins-1;
	torque_bins=_torque_bins-1;
	angle_max=_angle_max;
	velocity_max=_velocity_max;
	torque_max=_torque_max;
}

StateSpace::SubscriptProxy1 StateSpace::operator[](const double angle)
{
	//error if angle exceeds bounds
	if ( std::abs(angle) > angle_max )
	{
		std::string error("angle argument exceeded with value: ");
		error+=to_string(angle);
		throw std::domain_error(error);
	}
	
	//get the coefficient
	double coef=0.5*angle_bins;
	
	//descretise index
	int discrete_index = static_cast<int>( std::round( coef*(1+angle/angle_max) ) );
	
	//return appropriate object
	return SubscriptProxy1(space[discrete_index]);
}

//searches state space by state object
PriorityQueue<float, double>& StateSpace::operator[](const State & state)
{
	//call the subscripts with the members of the state object
	return (*this)[state.theta][state.theta_dot][state.torque];
}
