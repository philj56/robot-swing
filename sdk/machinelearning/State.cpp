/**
 * @file State.cpp
 *
 * @brief Implementation file for State class.
 * 
 * @author Machine Learning Team 2015-2016
 * @date March, 2016
 */

#include "State.h"
#include <cmath>

/**
 * Creates a state object initialised with starting values of angle of robot
 * based on encoder value, velocity of robot (both typically should be zero)
 * and the initial default 'action state' to perform.
 */
State::State(double _theta, double _theta_dot, ROBOT_STATE _robot_state) :
	theta(_theta),
	theta_dot(_theta_dot),
	robot_state(_robot_state) {
}

/**
 * Computes the reward value of a given state through the principle of energy
 * increase requirements for the system; the equation used is,
 *
 * \f[ R = g \cos(\theta) + \frac{1}{2} \dot\theta ^2 \f]
 *
 * where R is the reward, \f$\theta\f$ is the angle and \f$\dot\theta\f$ is the velocity. 
 * This equation simply sums the kinetic and potential energies for the state.
 *
 */
double State::getReward() {
	//return kinetic + potential
	return g * std::cos(theta) + 0.5 * theta_dot * theta_dot;
}

/**
 * Computes the reward value of a given state through the principle of height
 * maximisation. The equation used in this case is,
 *
 * \f[ R = \frac{\theta \dot\theta}{|\dot\theta|} + 4 \f]
 *
 * where R is the reward, \f$ \theta \f$ the angle and \f$\dot\theta\f$ is the velocity.
 * This equation gets the height of a state and the sign of the velocity (to determine
 * direction of swinging) with an offset of 4 included to ensure the return value is
 * always positive.
 * 
 */
double State::getHeightReward() {
	//return height and sign of velocity, add 4 so it is always positive
	return (theta * theta_dot) / std::abs(theta_dot) + 4;
}