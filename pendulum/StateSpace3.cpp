#include "StateSpace3.h"

int StateSpace::angle_bins;
int StateSpace::velocity_bins;
int StateSpace::torque_bins;
int StateSpace::torque_max;

StateSpace::StateSpace(const PriorityQueue<float, double>& queue, int _angle_bins, int _velocity_bins, int _torque_bins) :
	space(_angle_bins, std::vector<std::vector<PriorityQueue<float, double>>>(_velocity_bins, std::vector<PriorityQueue<float, double>>(2*_torque_bins+1, PriorityQueue<float, double>(queue))))

{}

void StateSpace::setAngleBins(int val)
{
	angle_bins = val;
}

void StateSpace::setVelocityBins(int val)
{
	velocity_bins = val;
}

void StateSpace::setTorqueBins(int val)
{
	torque_bins = val;
}

void StateSpace::setTorqueMax(int val)
{
	torque_max = val;
}


StateSpace::SubscriptProxy1 StateSpace::operator[](const double angle)
{
	//error if angle exceeds bounds
	if (std::abs(angle)>2 * 3.1415)throw std::domain_error("angle argument exceeded");
	//descretise index
	int discrete_index = static_cast<int>(std::round(angle * 100 / angle_bins) + angle_bins / 2);

	//return appropriate object
	return SubscriptProxy1(space[discrete_index]);
}

//searches state space by state object
PriorityQueue<float, double>& StateSpace::operator[](const State & state)
{
	//call the subscripts with the members of the state object
	return (*this)[state.theta][state.theta_dot][state.torque];
}

