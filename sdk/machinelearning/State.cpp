#include "State.h"
#include <cmath>        // std::abs

State::State(double _theta, double _theta_dot, ROBOT_STATE _robot_state):
  theta(_theta),
  theta_dot(_theta_dot),
  robot_state(_robot_state)
{}

double State::getReward(){
  //return kinetic + potential
  return g * std::cos(theta) + 0.5 * theta_dot * theta_dot;
}

double State::getHeightReward(){
  //return height and sign of velocity, add 4 so it is always positive
  return (theta * theta_dot)/std::abs(theta_dot) + 4;
}
