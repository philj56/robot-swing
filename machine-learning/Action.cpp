#include "Action.h"

Action::Action(const int _action):
action(_action)
{}

Action::Execute()
{
  //call stored function
}

bool operator==(const Action& a1) const
{
  return action==target.action;
}
