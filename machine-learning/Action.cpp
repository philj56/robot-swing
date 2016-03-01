#include "Action.h"

Action::Action(const int _action):
action(_action)
{}

void Action::execute()
{
  //call stored function
}

bool operator==(const Action& target) const
{
  //two actions must be equal if their identifiers are equal
  return action==target.action;
}

bool operator!=(const Action& target) const 
{
  return !(*this == target);
}
