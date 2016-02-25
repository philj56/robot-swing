#include <vector>
#include <cmath>
#include <stdexcept>
#include "PriorityQueue.h"

class Experience;

//index with state_space_object[action][angle][velocity]

//class to hold the 2d vector of containers of experiences that represents the robot's state and memory
class StateSpace
{
	public:
		//@_x_size: the size of the first vector
		//@_y_size: the size of the second vector
		explicit StateSpace(const unsigned int _x_size, const unsigned int _y_size);
		~StateSpace();
		
		StateSpace(const StateSpace&)=delete;
		
		//these nested classes are neccessary so that the [][][] operator can be called on this class
		//the operator should be called with the continuous state variables which it will then discretise
		//-----------------------------------------------------------------------------
		class SubscrpitProxy1
		{
			public:
				SubscriptProxy1(std::vector<std::vector<PriorityQueue<Experience*,double>>>& _vec):vec(_vec){}
				
				SubscriptProxy2 operator[](const double angle)
				{
					//error if angle exceeds bounds
					if(std::abs(angle)>M_PI/4)throw std::domain_error("angle argument exceeded");
					//descretise index
					int discrete_index=std::round(angle*100/x_size)+x_size/2;
					
					//return appropriate array
					return SubscriptProxy2(vec[discrete_index]);
				}
			
			private:
				std::vector<std::vector<PriorityQueue<Experience*,double>>>& vec;
		};
		
		class SubscrpitProxy2
		{
			public:
				SubscriptProxy2(std::vector<PriorityQueue<Experience*,double>>& _vec):vec(_vec){}
				
				PriorityQueue<Experience*,double>& operator[](const double velocity)
				{
					//error if angle exceeds bounds
					if(std::abs(velocity)>1)throw std::domain_error("velocity argument exceeded");
					//descretise index
					int discrete_index=std::round(velocity*100/y_size)+y_size/2;
					
					//return appropriate array
					return SubscriptProxy2(vec[discrete_index]);
				}
			
			private:
				std::vector<PriorityQueue<Experience*,double>>& vec;
		};
		//-----------------------------------------------------------------------------
		
		SubscriptProxy1 operator[](const int action);
		
	private:
		//the sizes of the two arrays
		const int x_size;
		const int y_size;
		
		//the 2d array that contains the robots previous experiences in each state
		std::vector<std::vector<PriorityQueue<Experience*,double>>> space1;
		std::vector<std::vector<PriorityQueue<Experience*,double>>> space2;
};
