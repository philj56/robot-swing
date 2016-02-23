#include "Action.h"

//experience object to hold an action, a reward associated with that action and the subsequent state reached
//should always be contained inside the state space
//note that the stored action is currently delete in the object's destructor

struct Experience
{
	explicit Experience(const double _utility, const Action* const _action);
	
	const Action* const action;
	const double utility;
	double state_result[2];
}
