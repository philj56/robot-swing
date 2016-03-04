#include "Action.h"

Action::Action(const int _action, void (*_execute)()):
action(_action), execute(_execute)
{}

bool Action::operator==(const Action& target) const
{
  //two actions must be equal if their identifiers are equal
  return action==target.action;
}

bool Action::operator!=(const Action& target) const 
{
  return !(*this == target);
}

const Action& Action::operator=(const Action& target)
{
  return Action(target.action, target.execute);
}
