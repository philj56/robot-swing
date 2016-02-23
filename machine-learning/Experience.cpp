#include "Experience.h"

Experience::Experience(const Action* const _action, const double x, const double y):
action(_action)
{
  state[0]=x;
  state[1]=y;
}
