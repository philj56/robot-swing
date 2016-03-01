#ifndef STATESPACE_H
#define STATESPACE_H

#include <vector>
#include <cmath>
#include <stdexcept>
#include "PriorityQueue.h"

//index with state_space_object[robot_state][angle][velocity]
//   or with state_space_object[state_object]

class Action;
class State;

//class to hold the 2d vector of containers of experiences that represents the robot's state and memory
class StateSpace
{
	public:
		//@_var_max: the maximum absolute size of the corresponding variable
		//@queue: the PriorityQueue to initialise the StateSpace with (this should normally contain just one of every action all with 0 priority)
		explicit StateSpace(const unsigned int _angle_max, const unsigned int _velocity_max, PriorityQueue<Action*,double> queue);
		
		//this object should NEVER be copied
		StateSpace(const StateSpace&)=delete;
		
		//these nested classes are necessary so that the [][][] operator can be called on this class
		//the operator should be called with the continuous state variables which it will then discretise
		//---------------------------------------------------------------------------------------------------------
		class SubscriptProxy2
		{
			public:
				SubscriptProxy2(std::vector<PriorityQueue<Action*,double>>& _vec):vec(_vec){}
				
				PriorityQueue<Action*,double>& operator[](const double velocity)
				{
					//error if angle exceeds bounds
					if(std::abs(velocity)>1)throw std::domain_error("velocity argument exceeded");
					//descretise index
					int discrete_index=std::round(velocity*100/velocity_max)+velocity_max/2;
					
					//return appropriate array
					return vec[discrete_index];
				}
			private:
				std::vector<PriorityQueue<Action*,double>>& vec;
		};
		
		class SubscriptProxy1
		{
			public:
				SubscriptProxy1(std::vector<std::vector<PriorityQueue<Action*,double>>>& _vec):vec(_vec){}
				
				SubscriptProxy2 operator[](const double angle)
				{
					//error if angle exceeds bounds
					if(std::abs(angle)>M_PI/4)throw std::domain_error("angle argument exceeded");
					//descretise index
					int discrete_index=std::round(angle*100/angle_max)+angle_max/2;
					
					//return appropriate object
					return SubscriptProxy2(vec[discrete_index]);
				}
			
			private:
				std::vector<std::vector<PriorityQueue<Action*,double>>>& vec;
		};
		//---------------------------------------------------------------------------------------------------------
		
		//subscript operator to access state queues
		//MUST be called with two other subscripts as described at top of document
		SubscriptProxy1 operator[](const unsigned int robot_state);
		
		//allow subscript to be used with this state objects
		//only one subscript is required to get a state queue in this fashion
		PriorityQueue<Action*, double>& operator[](const State & state);
		
	private:
		//the sizes of the two arrays
		static const int angle_max;
		static const int velocity_max;
		
		//the 2d array that contains the robots previous experiences in each state
		std::vector<std::vector<PriorityQueue<Action*,double>>> space1;
		std::vector<std::vector<PriorityQueue<Action*,double>>> space2;
};

#endif
