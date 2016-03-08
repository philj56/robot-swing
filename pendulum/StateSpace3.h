#ifndef STATESPACE_H
#define STATESPACE_H

#include <vector>
#include <cmath>
#include <stdexcept>
#include "PriorityQueue3.h"
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
		explicit StateSpace(const PriorityQueue<float,double>& queue, int _angle_bins, int _velocity_bins, int _torque_bins);
		~StateSpace();
		
		//deny copy construction
		//StateSpace(const StateSpace&)=delete;
		
		void setAngleBins(double val);
		void setVelocityBins(double val);
		void setTorqueBins(double val);
		void setTorqueMax(double val);

		//these nested classes are necessary so that the [][][] operator can be called on this class
		//the operator should be called with the continuous state variables which it will then discretise
		//-----------------------------------------------------------------------------
		class SubscriptProxy2
		{
			public:
				SubscriptProxy2(std::vector<PriorityQueue<float,double> >& _vec):vec(_vec){}
				
				PriorityQueue<float,double>& operator[](const double torque)
				{
					//error if angle exceeds bounds
					if(std::abs(torque)>torque_max)throw std::domain_error("torque argument exceeded");
					//descretise index
					int discrete_index=round(torque*100/torque_bins)+torque_bins/2;
					
					//return appropriate vector
					return vec[discrete_index];
				}
			private:
				std::vector<PriorityQueue<float,double> >& vec;
		};
		
		class SubscriptProxy1
		{
			public:
				SubscriptProxy1(std::vector<std::vector<PriorityQueue<float,double> > >& _vec):vec(_vec){}
				
				SubscriptProxy2 operator[](const double velocity)
				{
					//error if velocity exceeds bounds
					if(std::abs(velocity)>1)throw std::domain_error("velocity argument exceeded");
					//descretise index
					int discrete_index=round(velocity*100/velocity_bins)+velocity_bins/2;
					
					//return appropriate object
					return SubscriptProxy2(vec[discrete_index]);
				}
			
			private:
				std::vector<std::vector<PriorityQueue<float,double> > >& vec;
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
		static int torque_max;
		
		//the 3d vector that contains the robots previous experiences in each state
		std::vector< std::vector< std::vector< PriorityQueue<float,double> > > > space;
};

#endif

