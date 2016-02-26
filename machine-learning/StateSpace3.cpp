#include "StateSpace.h"

StateSpace::StateSpace(const unsigned int _angle_bins, const unsigned int _velocity_bins, const unsigned int _torque_bins):
	angle_bins(_angle_bins),
	velocity_bins(_velocity_bins),
	torque_bins(_torque_bins),
	space( std::array< std::array< PriorityQueue<Action*,double>(HeapType::MAX),_torque_bins>, _velocity_bins )
{}

StateSpace::~StateSpace()
{
	//ridiculously extensive clean-up
	for(unsigned int i=0 ; i<angle_max ; ++i)
	{
		for(unsigned int j=0 ; j<velocity_max ; ++j)
		{
			for(unsigned int k=0 ; k<torque_max ; ++k)
			{
				PriorityQueue<Action*,double>& queue1=space1[i][j][k];
				PriorityQueue<Action*,double>& queue2=space2[i][j][k];
				
				for(auto iter=queue1.begin(),queue1=list.end() ; iter!=end ; ++iter)
				{
					delete *iter;
				}
			}
		}
	}
}

StateSpace::SubscriptProxy1 StateSpace::operator[](const double angle)
{
	//error if angle exceeds bounds
	if(std::abs(angle)>2*M_PI)throw std::domain_error("angle argument exceeded");
	//descretise index
	int discrete_index=std::round(angle*100/angle_bins)+angle_bins/2;
	
	//return appropriate object
	return SubscriptProxy1(space[discrete_index]);
}
