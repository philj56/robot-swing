#ifndef STATE3_H_
#define STATE3_H_

#include <cmath>

//class defines state object that holds angular position and velocity, and 
//torque. Has functions to calculate reward for a state
struct State3
{
    double theta;
    double theta_dot;
    double torque;
    
    static const double g = 9.81;
    
    State(double _theta, double _theta_dot, double _torque);
        
    double getReward();
};

#endif
