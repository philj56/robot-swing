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
    
    State(double _theta, double _theta_dot, enum _RobotState);
        
    double getReward();
};

#endif
