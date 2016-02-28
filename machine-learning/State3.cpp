#include "State3.h"

State::State(double _theta, double _theta_dot, double _torque):
  theta(_theta),
  theta_dot(_theta_dot),
  torque(_robot_state)
{}

double State::getReward(){
  return theta;
}
