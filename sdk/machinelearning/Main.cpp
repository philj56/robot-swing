#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include "StateSpace.h"
#include "PriorityQueue.h"
#include "State.h"
#include "encoder.h"
#include "CreateModule.h"

/**
 * @brief Gives a std::string representation of a primitive type
 * 
 * @param x Primitive type such as int, double, long
 * @return std::string conversion of param x
 */
template<typename T> std::string to_string(T x) {
	return static_cast<std::ostringstream&>((std::ostringstream() << std::dec << x)).str();
}

//function to calculate a temperature for the select action function as a function of time
double temperature();

//function to select next action
int selectAction(const PriorityQueue<int,double>& a_queue);

//function to update a q value
void updateQ(StateSpace & space, int action, State & new_state, State & old_state, double alpha, double gamma);

int main()
{	
	// STUFF WE DONT UNDERSTAND, AND DONT NEED TO
	//__________________________________________________________________________________________
	//__________________________________________________________________________________________
	// Libraries to load
	std::string bodyLibName = "bodyinfo";
	std::string movementLibName = "movementtools";
	
	// Name of camera module in library
	std::string bodyModuleName = "BodyInfo";
	std::string movementModuleName = "MovementTools";

	// Set broker name, ip and port, finding first available port from 54000
	const std::string brokerName = "MotionTimingBroker";
	int brokerPort = qi::os::findAvailablePort(54000);
	const std::string brokerIp = "0.0.0.0";
	
	// Default parent port and ip
	int pport = 9559;
	std::string pip = "127.0.0.1";
	
	// Need this for SOAP serialisation of floats to work
	setlocale(LC_NUMERIC, "C");

	// Create a broker
	boost::shared_ptr<AL::ALBroker> broker;
	try
	{
		broker = AL::ALBroker::createBroker(
				brokerName,
				brokerIp,
				brokerPort,
				pip,
				pport,
				0);
	}
	// Throw error and quit if a broker could not be created
	catch(...)
	{
		std::cerr << "Failed to connect broker to: "
			  << pip
			  << ":"
			  << pport
			  << std::endl;
		AL::ALBrokerManager::getInstance()->killAllBroker();
		AL::ALBrokerManager::kill();

		return 1;
	}

	// Add the broker to NAOqi
	AL::ALBrokerManager::setInstance(broker->fBrokerManager.lock());
	AL::ALBrokerManager::getInstance()->addBroker(broker);

	CreateModule(movementLibName, movementModuleName, broker, false, true);
	CreateModule(bodyLibName, bodyModuleName, broker, false, true);
	
	AL::ALProxy bodyInfoProxy(bodyModuleName, pip, pport);
	AL::ALProxy movementToolsProxy(movementModuleName, pip, pport);
	AL::ALMotionProxy motion(pip, pport);
	//__________________________________________________________________________________________
	//__________________________________________________________________________________________
	//END OF STUFF WE DONT UNDERSTAND, BREATHE NOW
	
	//learning factor
	const double alpha=0.5;
	//discount factor
	const double gamma=0.5;
	
	//seed rng
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	
	int action_forwards = FORWARD;
	int action_backwards = BACKWARD;
	int chosen_action = action_forwards;
	
	//create a priority queue to copy to all the state space priority queues
	PriorityQueue<int,double> initiator_queue(MAX);
	initiator_queue.enqueueWithPriority(action_forwards,0);
	initiator_queue.enqueueWithPriority(action_backwards,0);
	
	//create encoder
	Encoder encoder;
	encoder.Calibrate();
	
	//create the state space
	StateSpace space(100,50,initiator_queue);
	
	//state objects
	State current_state(0,0,FORWARD);
	State old_state(0,0,FORWARD);
	
	while(true)
	{
		// set current state angle to angle received from encoder
		// and set current state velocity to difference in new and
		// old state angles over some time difference
		current_state.theta= M_PI * (encoder.GetAngle())/180;
		current_state.theta_dot=(current_state.theta - old_state.theta)/700; //Needs actual time
		current_state.robot_state=static_cast<ROBOT_STATE>(chosen_action);
		
		// call updateQ function with state space, old and current states
		// and learning rate, discount factor
		updateQ(space, chosen_action, old_state, current_state, alpha, gamma);
		
		// set old_state to current_state
		old_state=current_state;
		
		// determine chosen_action for current state
		chosen_action=selectAction(space[current_state]);
		
		// depending upon chosen action, call robot movement tools proxy with either
		// swingForwards or swingBackwards commands.
		(chosen_action)?movementToolsProxy.callVoid("swingForwards"):movementToolsProxy.callVoid("swingBackwards");
	}
	
	return 1;
}

/**
 * @brief Analog to temperature variable in Boltzmann Distribution.
 * 
 * @return current system time in milliseconds
 */
double temperature()
{
	return static_cast<double>(std::time(NULL));
}

/**
 * @brief Selects an action to perform based on probabilities.
 *
 * @param a_queue A priority queue instance storing integral types with double type priorities,
 *		  represents the queue of possible actions with pre-initialised priority levels.
 * @return integer corresponding to chosen action
 */
int selectAction(const PriorityQueue<int, double>& a_queue) {

	// queue to store action values
	PriorityQueue<int, double> actionQueue(MAX);

	double sum = 0.0;

	// calculate partition function by iterating over action-values
	for (PriorityQueue<int, double>::const_iterator iter = a_queue.begin(), end = a_queue.end(); iter < end; ++iter) {
		sum += std::exp((iter->second) / temperature());
	}

	// compute Boltzmann factors for action-values and enqueue to actionQueue
	for (PriorityQueue<int, double>::const_iterator iter = a_queue.begin(); iter < a_queue.end(); ++iter) {
		double priority = std::exp(iter.operator*().second / temperature()) / sum;
		actionQueue.enqueueWithPriority(iter.operator*().first, priority);
	}

	// calculate cumulative probability distribution
	for (PriorityQueue<int, double>::const_iterator it1 = actionQueue.begin()++, it2 = actionQueue.begin(), end = actionQueue.end(); it1 < end; ++it1, ++it2) {
		// change priority of it1->first data item in actionQueue to
		// sum of priorities of it1 and it2 items
		actionQueue.changePriority(it1->first, it1->second + it2->second);
	}

	//generate RN between 0 and 1
	double rand_num = static_cast<double>(rand()) / RAND_MAX;

	// choose action based on random number relation to priorities within action queue
	for (PriorityQueue<int, double>::const_iterator iter = actionQueue.begin(), end = actionQueue.end(); iter < end; ++iter) {
		if (rand_num < iter->second)
			return iter->first;
	}

	return -1; //note that this line should never be reached
}

/**
 * @brief Updates the utility (Q-value) of the system
 * 
 * @param space Reference to StateSpace object
 * @param new_state Reference to State instance giving the new system state
 * @param old_state Reference to State instance giving the old system state
 * @param alpha Learning rate of temporal difference learning algorithm
 * @param gamma Discount factor applied to q-learning equation
 */
void updateQ(StateSpace & space, int action, State & new_state, State & old_state, double alpha, double gamma)
{
    //oldQ value reference
    double oldQ = space[old_state].search(action).second;
    
    //reward given to current state 
    double R = new_state.getReward();
    
    //optimal Q value for new state i.e. first element 
    double maxQ = space[new_state].peekFront().second;
    
    //new Q value determined by Q learning algorithm
    double newQ = oldQ + alpha * (R + (gamma * maxQ) - oldQ);
    
    // change priority of action to new Q value
    space[old_state].changePriority(action, newQ);
}

/*old select action function in case the new one doesn't work
int selectAction(const PriorityQueue<int,double>& a_queue)
{	
	typedef PriorityQueue<int,double> PQ;
	typedef std::vector< std::pair<int, double> > Vec_Pair;
	typedef std::pair<int, double> Pair;
	
	double sum= 0;
	size_t i = 0;
	
	size_t size = a_queue.getSize();
	Vec_Pair action_vec(size);
	
	//Calculate partition function by iterating over action-values
	for(PQ::const_iterator iter = a_queue.begin(),end=a_queue.end(); iter < end; ++iter)
	{
		sum += std::exp((iter->second)/temperature());
	}
	//Calculate boltzmann factors for action-values
	for(Vec_Pair::iterator it = action_vec.begin(),end=action_vec.end(); it < end; ++it)
	{
	    it->first = a_queue[i].first;
	    it->second = std::exp(a_queue[i].second /temperature()) / sum;
	    ++i;
	}
	
	//calculate cumulative probability distribution    
	for(Vec_Pair::iterator it1 = action_vec.begin()++,it2 = action_vec.begin(),end=action_vec.end(); it1 < end; ++it1,++it2)
	{
	    it1->second += it2->second;
	}
	
	//generate RN between 0 and 1
	double rand_num = static_cast<double>(rand())/ RAND_MAX;
	
	//select action based on probability 
	for(Vec_Pair::iterator it = action_vec.begin(),end=action_vec.end(); it < end; ++it)
	{
		//if RN falls within cumulative probability bin return the corresponding action
		if(rand_num < it->second)return it->first;
	}
 	
	return -1; //note that this line should never be reached
}
*/
