/**
 * @file State.h
 *
 * @brief Contains State class interface and ROBOT_STATE enum
 *
 * @author Machine Learning Team 2015-2016
 * @date March, 2016
 */

#ifndef STATE_H_
#define STATE_H_

#include <cmath>

/**
 * @enum ROBOT_STATE
 *
 * @brief Gives the state of a robot based on actions to perform, currently
 *		  contains just simply FORWARD and BACKWARD identifiers for actions.
 *
 * @todo Create more enumerated constants inside this state enum to represent
 *		 more fine-tuned actions.
 */
enum ROBOT_STATE { FORWARD, BACKWARD };

const double g = 9.81;

/**
 * @class State
 *
 * @brief Defines a state object that holds angular positions/velocity, and robot state.
 *
 * Holds the state of the system, given by the angle, angular velocity and state of the
 * robot (based on actions performed). Contains methods to calculate rewards for any
 * given state - currently 2 rewards functions exist, one based on energy the other based
 * on height.
 *
 * @todo Generate more complicated reward functions to take other considerations into account.
 *
 * @author Machine Learning Team 2015-2016
 * @date February, 2016
 */
struct State {
	
	double theta;
	double theta_dot;
	ROBOT_STATE robot_state;

	/** 
	 * @brief Constructor with initial angle, velocity and state variables.
	 *
	 * @param _theta Initial angle
	 * @param _theta_dot Initial velocity
	 * @param _robot_state Initial robot state
	 */
	State(double _theta, double _theta_dot, ROBOT_STATE _robot_state);

	/**
	 * @brief Gets the reward value of states based on energy 
	 *
	 * @return Reward value for current state
	 */
	double getReward();
	
	/**
	 * @brief Gets the reward value of states based on height
	 *
	 * @return Reward value for current state
	 */
	double getHeightReward();
};

#endif