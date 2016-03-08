#ifndef STATESPACE_H
#define STATESPACE_H

#include <vector>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include "PriorityQueue.h"
#include "State.h"

//index with state_space_object[robot_state][angle][velocity]
//   or with state_space_object[state_object]

//class to hold the 2d vector of containers of experiences that represents the robot's state and memory
class StateSpace
{
	public:
		//@_var_bins: the number of bin in the array of the corresponding variable
		//@queue: the PriorityQueue to initialise the StateSpace with (this should normally contain just one of every action all with 0 priority)
		explicit StateSpace(int _angle_bins, int _velocity_bins, const PriorityQueue<int,double>& queue);
		
		//these nested classes are necessary so that the [][][] operator can be called on this class
		//the operator should be called with the continuous state variables which it will then discretise
		//---------------------------------------------------------------------------------------------------------
		class SubscriptProxy2
		{
			public:
				SubscriptProxy2(std::vector< PriorityQueue<int,double> >& _vec):vec(_vec){}
				
				PriorityQueue<int,double>& operator[](const double velocity)
				{
					//error if angle exceeds bounds
					if(std::abs(velocity)>1)throw std::domain_error("velocity argument exceeded");
					//descretise index
					int discrete_index=round(velocity*100/velocity_bins)+velocity_bins/2;
					
					//return appropriate array
					return vec[discrete_index];
				}
			private:
				std::vector< PriorityQueue<int,double> >& vec;
		};
		
		class SubscriptProxy1
		{
			public:
				SubscriptProxy1(std::vector< std::vector< PriorityQueue<int,double> > >& _vec):vec(_vec){}
				
				SubscriptProxy2 operator[](const double angle)
				{
					//error if angle exceeds bounds
					std::cout << angle << std::endl;
					if(std::abs(angle)>M_PI/4)throw std::domain_error("angle argument exceeded");
					//descretise index
					int discrete_index=round(angle*100/angle_bins)+angle_bins/2;
					
					//return appropriate object
					return SubscriptProxy2(vec[discrete_index]);
				}
			
			private:
				std::vector< std::vector< PriorityQueue<int,double> > >& vec;
		};
		//---------------------------------------------------------------------------------------------------------
		
		//subscript operator to access state queues
		//MUST be called with two other subscripts as described at top of document
		SubscriptProxy1 operator[](const unsigned int robot_state);
		
		//allow subscript to be used with this state objects
		//only one subscript is required to get a state queue in this fashion
		PriorityQueue<int, double>& operator[](const State & state);
		
	private:
		//this object should NEVER be copied
		StateSpace(const StateSpace&);
		
		//the sizes of the two arrays
		static int angle_bins;
		static int velocity_bins;
		
		//the 2d array that contains the robots previous experiences in each state
		std::vector< std::vector< PriorityQueue<int,double> > > space1;
		std::vector< std::vector< PriorityQueue<int,double> > > space2;
};

#endif
