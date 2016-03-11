/**
 * @file StateSpace.cpp
 *
 * @brief Implementation file for StateSpace class.
 *
 * @author Machine Learning Team 2015-2016
 * @date March, 2016 
 */

#include "StateSpace.h"

int StateSpace::angle_bins;
int StateSpace::velocity_bins;

/**
 * Creates a state space object initialised with a given number of bins for discretising angles and
 * velocities of the system. A (const referenced) PriorityQueue instance is passed to the constructor
 * in order to initialise state space actions and experiences - not that this queue then should be
 * the queue of default initial action(s) and prioritised experience(s).
 */
StateSpace::StateSpace(int _angle_bins, int _velocity_bins, double _angle_max, double _velocity_max, const PriorityQueue<int, double>& queue) :
	space1(_angle_bins, std::vector< PriorityQueue<int, double> >(_velocity_bins, PriorityQueue<int, double>(queue))),
	space2(_angle_bins, std::vector< PriorityQueue<int, double> >(_velocity_bins, PriorityQueue<int, double>(queue)))
{
	angle_bins = _angle_bins-1;
	velocity_bins = _velocity_bins-1;
	angle_max = _angle_max;
	velocity_ = _velocity_max;
}

/**
 * Operator to be used for triple subscript indexing, returns a SubscriptProxy1 object.
 * 
 * Correct indexing of this method is as follows:
 *
 * \code{.cpp}
 * state_space_object[robot_state][angle][velocity]
 * \endcode
 */
StateSpace::SubscriptProxy1 StateSpace::operator[](const unsigned int robot_state) {
	//throw if the the index is out of bounds
	if (robot_state>1)throw std::domain_error("action index exceeded");
	//return proxy object to accept second [] operator
	return SubscriptProxy1(robot_state ? space1 : space2);
}

/**
 * Operator to be used for single subscript indexing, returns a PriorityQueue object.
 *
 * Correct indexing of this method is as follows:
 * 
 * \code{.cpp}
 * state_space_object[state_object]
 * \endcode
 */
PriorityQueue<int, double>& StateSpace::operator[](const State & state) {
	//call the subscript operators with the members of the state object
	return (*this)[state.robot_state][state.theta][state.theta_dot];
}
