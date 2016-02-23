#include <vector>
#include <list>

class Experience;

//class to hold the 2d vector of containers of experiences that represents the robot's state and memory
class StateSpace
{
	public:
		//@_x_size: the size of the first vector
		//@_y_size: the size of the second vector
		explicit StateSpace(const unsigned int _x_size, const unsigned int _y_size);
		~StateSpace();
		
		//this nested class is neccessary so that the [][] operator can be called on this class
		//the operator should be called with the continuous state variables which it will then discretise
		class SubscrpitProxy
		//-----------------------------------------------------------------------------
		{
			public:
				SubscriptProxy(std::vector<std::list<Experience*>>& _vec):vec(_vec){}
			
				std::list<Experience*>& operator[](const double index)
				{
					//descretise index
					int discrete_index=;
					//return appropriate array
					return vec[discrete_index];
				}
			
			private:
				std::vector<std::list<Experience*>>& vec;
		};
		//-----------------------------------------------------------------------------
		
		SubscriptProxy operator[](const double index);
		
	private:
		//the sizes of the two arrays
		const int x_size;
		const int y_size;
		
		//the 2d array that contains the robots previous experiences in each state
		std::vector<std::vector<std::list<Experience*>>> space;
};
