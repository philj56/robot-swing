#include <cmath>
#include <cstdlib>
#include <ctime>
#include "StateSpace.h"
#include "Action.h"
#include "PriorityQueue.h"
#include "State.h"
#include "encoder.h"
#include "CreateModule.h"

//function to calculate a temperature for the select action function as a function of time
double temperature();

//function to select next action
Action * selectAction(PriorityQueue<Action *,double>& a_queue);

//function to update a q value
void updateQ(StateSpace & space, Action & action, State & new_state, State & old_state, double alpha, double gamma);

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

	createModule(movementLibName, movementModuleName, broker, false, true);
	createModule(bodyLibName, bodyModuleName, broker, false, true);
	
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
	std::srand(std::time(NULL));
	
	//create pointers to the possible actions as well as a pointer to hold the chosen action
	Action* chosen_action;
	Action* actions[2];
	actions[0]=new Action(FORWARD,/*function pointer here*/);
	actions[1]=new Action(BACKWARD,/*function pointer here*/);
	
	//create a priority queue to copy to all the state space priority queues
	PriorityQueue<Action*,double> initiator_queue(MAX);
	initiator_queue.enqueueWithPriority(actions[0],0);
	initiator_queue.enqueueWithPriority(actions[1],0);
	
	//create encoder
	Encoder encoder();
	encoder.calibrate();
	
	//create the state space
	StateSpace space(initiator_queue);
	space.setAngle(100);
	space.setVelocity(50);
	
	//state objects
	State current_state(0,0,FORWARD);
	State old_state(0,0,FORWARD);
	
	while(true)
	{
		current_state.theta=encoder.GetAngle();
		current_state.theta_dot=(current_state.theta - old_state.theta)/TIME;
		current_state.robot_state=chosen_action.action;
		
		updateQ(space, chosen_action, old_state, current_state, alpha, gamma);
		
		old_state=current_state;
		
		chosen_action=selectAction(space[current_state]);
		
		chosen_action.execute();
	}
	
	delete actions[0];
	delete actions[1];
	
	return 1;
}

double temperature()
{
	return std::time(NULL);
}

//function is fed with a priority queue of action-values 
//generates Boltzmann distribution of these action-values
//and selects an action based on probabilities 
Action * selectAction(PriorityQueue<Action *,double>& a_queue)
{	
	typedef PriorityQueue<Action *,double> PQ;
	typedef std::vector< std::pair<Action *, double> > Vec_Pair;
	typedef std::pair<Action *, double> Pair;
	
	double sum= 0;
	int i = 0;
	
	int size = a_queue.getSize();
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
	for(std::vector< std::pair<int, double> >::iterator it1 = action_vec.begin()++,it2 = action_vec.begin(),end=action_vec.end(); it < end; ++it1,++it2)
	{
	    it1->second += it2->second;
	}
	
	//generate RN between 0 and 1
	double rand_num = static_cast<double>(rand()/ (RAND_MAX));
	
	//select action based on probability 
	for(Vec_Pair::iterator it = action_vec.begin(),end=action_vec.end(); it < end; ++it)
	{
		//if RN falls within cumulative probability bin return the corresponding action
		if(rand_num < it->second)return it->first;
	}
 	
	return NULL; //note that this line should never be reached
}

void updateQ(StateSpace & space, Action * action, State & new_state, State & old_state, double alpha, double gamma)
{
    //oldQ value reference
    double oldQ = space[old_state].search(action).second;
    
    //reward given to current state 
    double R = new_state.getReward();
    
    //optimal Q value for new state i.e. first element 
    double maxQ = space[current_state].peekFront().second;
    
    //new Q value determined by Q learning algorithm
    double newQ = oldQ + alpha * (R + (gamma * maxQ) - oldQ);
    
    // change priority of action to new Q value
    space[old_state].changePriority(action, newQ);
}

movement()
{
	
	// Add the broker to NAOqi
	AL::ALBrokerManager::setInstance(broker->fBrokerManager.lock());
	AL::ALBrokerManager::getInstance()->addBroker(broker);
	
	createModule(movementLibName, movementModuleName, broker, false, true);
	
	AL::ALProxy movementToolsProxy(movementModuleName, pip, pport);
	
	movementToolsProxy.callVoid("swingForwards");
}
