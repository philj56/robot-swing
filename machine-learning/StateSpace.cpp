#include "StateSpace.h"

StateSpace::StateSpace(const int _x_size, const int _y_size):
	x_size(_x_size),
	y_size(_y_size),
	space(x_size, std::vector< std::list<Experience*> > (y_size))
{}

StateSpace::~StateSpace()
{
	//ridiculously extensive clean-up
	for(unsigned int i=0 ; i<x_size ; ++i)
	{
		for(unsigned int j=0 ; j<x_size ; ++j)
		{
			std::vector<std::list<Experience*>>& list=space[i][j];
			
			for(std::list<Experience*>::iterator iter=list.begin(),end=list.end() ; iter!=end ; ++iter)
			{
				delete *iter;
			}
		}
	}
}

SateSpace::SubscriptProxy SateSpace::operator[](const double index)
{
	//discretise index
	int discrete_index=;
	//return proxy object to accept second [] operator
	return SubscriptProxy(space[discrete_index]);
}
