#include "State.h"

State::State(double _theta, double _theta_dot, enum _RobotState):
  theta(_theta),
  theta_dot(_theta_dot),
  RobotState(_RobotState)
{}

double State::getReward(){
        g * std::cos(theta) + 0.5 * theta_dot * theta_dot;
    }
