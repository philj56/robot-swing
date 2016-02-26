#ifndef STATESPACE_H
#define STATESPACE_H

#include <array>
#include <cmath>
#include <stdexcept>
#include "PriorityQueue.h"

//index with state_space_object[angle][velocity][torque]

class Action;

//class to hold the 2d vector of containers of experiences that represents the robot's state and memory
class StateSpace
{
	public:
		//@_x_size: the size of the first vector
		//@_y_size: the size of the second vector
		explicit StateSpace(const unsigned int _angle_bins, const unsigned int _velocity_bins, const unsigned int _torque_bins);
		~StateSpace();
		
		//deny copy construction
		StateSpace(const StateSpace&)=delete;
		
		//these nested classes are necessary so that the [][][] operator can be called on this class
		//the operator should be called with the continuous state variables which it will then discretise
		//-----------------------------------------------------------------------------
		class SubscriptProxy2
		{
			public:
				SubscriptProxy2(std::array<PriorityQueue<Action*,double>,torque_bins>& _vec):vec(_vec){}
				
				PriorityQueue<Action*,double>& operator[](const double torque)
				{
					//error if angle exceeds bounds
					if(std::abs(torque)>10)throw std::domain_error("torque argument exceeded");
					//descretise index
					int discrete_index=std::round(torque*100/torque_bins)+torque_bins/2;
					
					//return appropriate array
					return vec[discrete_index];
				}
			private:
				std::array<PriorityQueue<Action*,double>,torque_bins>& vec;
		};
		
		class SubscriptProxy1
		{
			public:
				SubscriptProxy1(std::array<std::array<PriorityQueue<Action*,double>,torque_bins>,velocity_bins>& _vec):vec(_vec){}
				
				SubscriptProxy2 operator[](const double velocity)
				{
					//error if velocity exceeds bounds
					if(std::abs(velocity)>1)throw std::domain_error("velocity argument exceeded");
					//descretise index
					int discrete_index=std::round(velocity*100/velocity_bins)+velocity_bins/2;
					
					//return appropriate object
					return SubscriptProxy2(vec[discrete_index]);
				}
			
			private:
				std::array<std::array<PriorityQueue<Action*,double>,torque_bins>,velocity_bins>& vec;
		};
		//-----------------------------------------------------------------------------
		
		SubscriptProxy1 operator[](const double angle);
		
	private:
		//the sizes of the three arrays
		static const int angle_bins;
		static const int velocity_bins;
		static const int torque_bins;
		
		//the 2d array that contains the robots previous experiences in each state
		std::array<std::array<std::array<PriorityQueue<Action*,double>,torque_bins>,velocity_bins>,angle_bins> space;
};

#endif
