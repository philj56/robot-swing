/**
 * @file StateSpace.h
 *
 * @brief Interface file for StateSpace class.
 *
 * @author Machine Learning Team 2015-2016
 * @date March, 2016
 */

#ifndef STATESPACE_H
#define STATESPACE_H

#include <vector>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include "PriorityQueue.h"
#include "State.h"

//index with state_space_object[robot_state][angle][velocity]
//   or with state_space_object[state_object]

/**
 * @class StateSpace
 *
 * @brief Holds a 2D std::vector of PriorityQueue instances of experiences that represents 
 *		  the robot's state and memory.
 *
 * A class used to contain all information about the robot-swing system given by angles, velocities
 * actions and experiences. Primarily uses a 2-dimensional std::vector container storing PriorityQueue
 * instances which in turn hold the actions and prioritised experiences at each state of the system.
 *
 * @author Machine Learning Team 2015-2016
 * @date February, 2016
 */
class StateSpace {
public:
	//@_var_bins: the number of bin in the array of the corresponding variable
	//@queue: the PriorityQueue to initialise the StateSpace with (this should normally contain just one of every action all with 0 priority)

	/**
	 * @brief Constructor with number of angle and velocity bins and initial queue instance.
	 *
	 * @param _angle_bins The number of bins for angles of the system space
	 * @param _velocity_bins The number of bins for velocity of the system space
	 * @param queue A PriorityQueue instance (referenced to avoid copying) containing initial (default) action and experience.
	 */
	explicit StateSpace(int _angle_bins, int _velocity_bins, double _angle_max, double _velocity_max, const PriorityQueue<int, double>& queue);
	
	//these nested classes are necessary so that the [][][] operator can be called on this class
	//the operator should be called with the continuous state variables which it will then discretise
	//---------------------------------------------------------------------------------------------------------
	
	/**
	 * @class SubscriptProxy2
	 *
	 * @brief One of two classes used to enable operator[][][] usage on StateSpace instances.
	 *
	 * @see SubscriptProxy1
	 */
	class SubscriptProxy2 {
	public:
		SubscriptProxy2(std::vector< PriorityQueue<int, double> >& _vec) :vec(_vec) {}

		PriorityQueue<int, double>& operator[](const double velocity) {
			//error if angle exceeds bounds
			if (std::abs(velocity)>1)throw std::domain_error("velocity argument exceeded");
			
			//get the coefficient
			double coef=0.5*velocity_bins;
			
			//descretise index
			int discrete_index = static_cast<int>(round( coef*(1+1/velocity_max) ) );

			//return appropriate array
			return vec[discrete_index];
		}
	private:
		std::vector< PriorityQueue<int, double> >& vec;
	};

	/**
	 * @class SubscriptProxy2
	 *
	 * @brief One of two classes used to enable operator[][][] usage on StateSpace instances.
	 *
	 * @see SubscriptProxy2
	 */
	class SubscriptProxy1 {
	public:
		SubscriptProxy1(std::vector< std::vector< PriorityQueue<int, double> > >& _vec) :vec(_vec) {}

		SubscriptProxy2 operator[](const double angle) {
			//throw if angle exceeds bounds
			if (std::abs(angle) > angle_max)throw std::domain_error("angle argument exceeded");
			
			//get the coefficient
			double coef=0.5*angle_bins;
			
			//descretise index
			int discrete_index = static_cast<int>(round( coef*(1+1/angle_max) ) );

			//return appropriate object
			return SubscriptProxy2(vec[discrete_index]);
		}

	private:
		std::vector< std::vector< PriorityQueue<int, double> > >& vec;
	};
	//---------------------------------------------------------------------------------------------------------

	//subscript operator to access state queues
	//MUST be called with two other subscripts as described at top of document

	/**
	 * @brief Overloaded subscript operator for accessing state objects.
	 *
	 * @warning MUST be called with 2 other subscripts as shown above!
	 * @param robot_state Robot state indexing variable
	 * @return Object of type SubscriptProxy1 
	 * @throw Throws std::domain_error if index is out of bounds
	 */
	SubscriptProxy1 operator[](const unsigned int robot_state);

	//allow subscript to be used with this state objects
	//only one subscript is required to get a state queue in this fashion

	/**
	 * @brief Overloaded subscript operator for accessing queues
	 *
	 * @warning Only ONE subscript required to get a state queue.
	 * @param state Constant reference to a State object for indexing
	 * @return Reference to PriorityQueue object contained at this location in state space
	 */
	PriorityQueue<int, double>& operator[](const State & state);
	
	static std::ostream& input(std::ostream& stream, const StateSpace& space) {
		for(unsigned short i=0;i<StateSpace::angle_bins;++i)
		{
			for(unsigned short j=0;j<StateSpace::angle_bins;++j)
			{
				stream<<space.space1[i][j];
				stream<<space.space2[i][j];
			}
		}
		return stream;
	}
	
	static std::istream& output(std::istream& stream, StateSpace& space) {
		int action1=0;
		double priority1=0;
		int action2=0;
		double priority2=0;
	
		for(unsigned short i=0;i<StateSpace::angle_bins;++i)
		{
			for(unsigned short j=0;j<StateSpace::angle_bins;++j)
			{
				stream>>action1;
				stream>>priority1;
				stream>>action2;
				stream>>priority2;
			
				space.space1[i][j].enqueueWithPriority(action1,priority1);
				space.space2[i][j].enqueueWithPriority(action2,priority2);
			}
		}
		return stream;
	}
	
//	friend std::ostream& operator<<(std::ostream& stream, const StateSpace& space);
//	friend std::istream& operator>>(std::istream& stream, StateSpace& space);
	
private:
	//this object should NEVER be copied

	/**
	 * @brief Copy constructor, set to private to prevent copying
	 *
	 * @remark If C++11 is available then this can be declared public as follows:
	 * \code{.cpp}
	 *	//...
	 *	public:
	 *		StateSpace(const StateSpace&) = delete;
	 *	//...
	 * \endcode
	 */
	StateSpace(const StateSpace&);

	//the sizes of the two arrays
	static int angle_bins;
	static int velocity_bins;
	
	//the max absolute values of the dimensions
	static double angle_max;
	static double velocity_max;

	//the 2d array that contains the robots previous experiences in each state
	std::vector< std::vector< PriorityQueue<int, double> > > space1;
	std::vector< std::vector< PriorityQueue<int, double> > > space2;
};

inline std::ostream& operator<<(std::ostream& stream, const StateSpace& space)
{
	return StateSpace::input(stream, space);
}

inline std::istream& operator>>(std::istream& stream, StateSpace& space)
{
	return StateSpace::output(stream, space);
}

#endif
