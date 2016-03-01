#ifndef STATE_H_
#define STATE_H_

#include <cmath>

 enum ROBOT_STATE {FORWARD, BACKWARD};
 const double g = 9.81;

//class defines state object that holds angular position and velocity, and 
//robot state. Has functions to calculate reward for a state
struct State
{
    //state variables
    double theta;
    double theta_dot;
    ROBOT_STATE robot_state;
    
    //static const double g = 9.81;
    
    State(double _theta, double _theta_dot, ROBOT_STATE _robot_state);
    
    //function to get the reward of the stored state based on energy
    double getReward();
};

#endif
