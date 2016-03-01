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
  //two actions must be equal if their identifiers are equal
  return action==target.action;
}
