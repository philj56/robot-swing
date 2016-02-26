#ifndef STATE_H_
#define STATE_H_

#include <cmath>

//class defines state object that holds angular position and velocity, and 
//robot state. Has functions to calculate reward for a state
struct State
{
    double theta;
    double theta_dot;
    enum ROBOT_STATE {FORWARD, BACKWARD};
    static const double g = 9.81;
    
    State(double _theta, double _theta_dot, enum _RobotState):
        theta(_theta),
        theta_dot(_theta_dot),
        RobotState(_RobotState)
    {}
        
    double getReward(){
        g * std::cos(theta) + 0.5 * theta_dot * theta_dot;
    }
};

#endif
