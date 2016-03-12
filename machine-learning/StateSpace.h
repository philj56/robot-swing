#ifndef STATESPACE_H
#define STATESPACE_H

#include <vector>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <string>
#include "PriorityQueue.h"
#include "State.h"

//index with state_space_object[angle][velocity][torque]
//   or with state_space_object[state_object]

//class to hold the 3d vector of priority queues that represents the robot's state and memory
class StateSpace
{
public:
	//@_angle_bins: the size of the first vector
	//@_velocity_bins: the size of the second vector
	//@_torque_bins: the size of the third vector
	//@queue: the PriorityQueue to initialise the StateSpace with (this should normally contain just one of every action all with 0 priority)
	explicit StateSpace(const PriorityQueue<float, double>& queue, int _angle_bins, int _velocity_bins, int _torque_bins, double _angle_max, double _velocity_max, double _torque_max);


	//deny copy construction
	StateSpace(const StateSpace&)=delete;
	
	//these nested classes are necessary so that the [][][] operator can be called on this class
	//the operator should be called with the continuous state variables which it will then discretise
	//-----------------------------------------------------------------------------
	class SubscriptProxy2
	{
	public:
		SubscriptProxy2(std::vector<PriorityQueue<float, double> >& _vec) :vec(_vec) {}

		PriorityQueue<float, double>& operator[](const double torque)
		{
			//error if angle exceeds bounds
			if ( std::abs(torque) > torque_max )
			{
				std::string error("torque argument exceeded with value: ");
				error+=to_string(torque);
				throw std::domain_error(error);
			}
			
			//get the coefficient
			int coef=0.5*torque_bins;
			
			//descretise index
			double discrete_index = static_cast<int>( std::round( coef*(1+torque/torque_max) ) );
			
			//return appropriate vector
			PriorityQueue<float, double> v=vec[discrete_index];
			
			return vec[discrete_index];
		}
	private:
		std::vector<PriorityQueue<float, double> >& vec;
	};

	class SubscriptProxy1
	{
	public:
		SubscriptProxy1(std::vector<std::vector<PriorityQueue<float, double> > >& _vec) :vec(_vec) {}

		SubscriptProxy2 operator[](const double velocity)
		{
			//error if velocity exceeds bounds
			if( std::abs(velocity) > velocity_max )
			{
				std::string error("velocity argument exceeded with value: ");
				error+=to_string(velocity);
				throw std::domain_error(error);
			}
			
			//get the coefficient
			double coef=0.5*velocity_bins;
			
			//descretise index
			int discrete_index = static_cast<int>( std::round( coef*(1+velocity/velocity_max) ) );
			
			//return appropriate object
			return SubscriptProxy2(vec[discrete_index]);
		}

	private:
		std::vector<std::vector<PriorityQueue<float, double> > >& vec;
	};
	//-----------------------------------------------------------------------------

	//subscript operator for accessing states
	//MUST be called with two additional subscripts
	SubscriptProxy1 operator[](const double angle);

	//subscript to get state queue from a state object
	PriorityQueue<float, double>& operator[](const State & state);
	
private:
	//the sizes of the three arrays
	static int angle_bins;
	static int velocity_bins;
	static int torque_bins;
	static double angle_max;
	static double velocity_max;
	static double torque_max;

	//the 3d vector that contains the robots previous experiences in each state
	std::vector< std::vector< std::vector< PriorityQueue<float, double> > > > space;
};

#endif
