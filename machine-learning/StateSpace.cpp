#include "StateSpace.h"

StateSpace::StateSpace(const int _x_size, const int _y_size):
	x_size(_x_size),
	y_size(_y_size),
	space(x_size, std::vector< std::list<Experience*> > (y_size))
{}

SateSpace::SubscriptProxy SateSpace::operator[](const double index)
{
	//discretise index
	int discrete_index=;
	//return proxy object to accept second [] operator
	return SubscriptProxy(space[discrete_index]);
}
