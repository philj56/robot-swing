#include "State3.h"

const double State::g=9.81;

State::State(double _theta, double _theta_dot, double _torque):
  theta(_theta),
  theta_dot(_theta_dot),
  torque(_torque)
{}

double State::getReward(){
  //reward proportional to theta
  return theta;
}

