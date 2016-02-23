#include "Action.h"

//experience object to hold an action and the subsequent state reached
//utility is encoded as corresponding priority
//should always be contained inside the state space

struct Experience
{
	explicit Experience(const Action* const _action, const double x, const double y);
	
	const Action* const action;
	const double state_result[2];
}
