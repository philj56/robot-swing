/**
 * @file main.cpp
 *
 * @brief File containing main Q-Learning algorithm and all necessary associated functions to implementing this routine.
 *
 * @author Machine Learning Team 2015-2016
 * @date March, 2016
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include "StateSpace.h"
#include "PriorityQueue.h"
#include "State.h"
#include "encoder.h"
#include "CreateModule.h"

/**
* @brief Gives a std::string representation of a primitive type.
*
* @remark Can be used to print priority queue contents.
* @param x Primitive type such as int, double, long ...
* @return std::string conversion of param x
*/
template<typename T> std::string to_string(T x) {
	return static_cast<std::ostringstream&>((std::ostringstream() << std::dec << x)).str();
}

/**
 * @brief Determines whether a file exists / is accessible
 * 
 * @param filename File to check for
 * @return true if file exists, false if not or is inaccessible
 */
bool fileExists(const char* filename) {
	std::ifstream file(filename);
	return file.good();
}

/**
 * @brief Analog to temperature variable in Boltzmann Distribution, computes 'evolution variable' of system.
 *
 * This function computes a normal distribution over the parameterised number of loop iterations in order
 * to provide a function which explores the state space dilligently initially which then decays off to the
 * optimal solution after a large number of loop iterations.
 *
 * The normal distribution used by this function is:
 *
 * \f[ a e^{-\frac{bt^2}{c}} + \epsilon \f]
 *
 * where a, b and c are scaling coefficients (see method body) and \f$\epsilon\f$ is some small offset.
 *
 * @param t Number of loop iterations.
 * @return Value of normal distribution at t loop iterations.
 */
double probabilityFluxDensityCoefficient(unsigned long t);

/**
 * @brief Selects an action to perform based on a stochastic factor, epsilon, and past experiences
 *
 * @param a_queue A priority queue instance storing integral types with double type priorities,
 *		  represents the queue of possible actions with pre-initialised priority levels.
 * @param epsilon Fraction determining randomness of greedy search, 0.0 = completely random.
 * @return integer corresponding to chosen action
 */
int selectAction_EpsilonGreedy(const PriorityQueue<int, double>& a_queue, double epsilon);

/**
* @brief Selects an action to perform based on experience, iterations and probabilities.
*
* @param a_queue A priority queue instance storing integral types with double type priorities,
*		  represents the queue of possible actions with pre-initialised priority levels.
* @param iterations Number of iterations completed
* @return integer corresponding to chosen action
*/
int selectAction_BoltzmannFactor(const PriorityQueue<int, double>& a_queue, unsigned long iterations);

/**
 * @brief Updates the utility (Q-value) of the system.
 *
 * Utility (Q-Value) of the system is updated via the Temporal Difference Learning Rule given by the following equation,
 *
 * \f[ Q_{i+1} (s,a) = Q_i (s,a) + \alpha [R(s') + \gamma \underset{a}{max} Q(s',a) - Q_i (s,a)] \f]
 *
 * where Q represents the utility of a state-action pair, \f$ \alpha \f$ is the learning rate, \f$ \gamma \f$ is the discount
 * factor and \f$ max_a (Q) \f$ yields the action-maximum of the Q space. The learning rate should be set to a low value for
 * highly stochastic, asymmetric systems or a high value for a lowly stochastic, symmetric system - this value lies between 0 and 1.
 * The discount factor (also in the interval [0,1]) represents the time considerations of the learning algorithm, lower values indicate
 * "living in the moment", whilst higher values indicate "planning for the future".
 *
 * @param space Reference to StateSpace object
 * @param action Integer code to previous performed action
 * @param new_state Reference to State instance giving the new system state
 * @param old_state Reference to State instance giving the old system state
 * @param alpha Learning rate of temporal difference learning algorithm (in the interval [0,1])
 * @param gamma Discount factor applied to q-learning equation (in the interval [0,1])
 */
void updateQ(StateSpace & space, int action, State & new_state, State & old_state, double alpha, double gamma);

/**
 * @brief Performs all proxy initialisation through NAO SDK functions and structures, setting up a connection
 *		  to the robot and allowing use of movement tools and body info libraries.
 *
 * @return A vector of size 3 where the first element contains the body info proxy (type AL::ALProxy), the second
 *		   element contains the movement tools proxy (type AL::ALProxy) and the third element contains the motion
 *		   proxy (type AL::ALMotionProxy).
 */
std::vector<AL::ALProxy> robotProxyInitialisation() {
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
	try {
		broker = AL::ALBroker::createBroker(
			brokerName,
			brokerIp,
			brokerPort,
			pip,
			pport,
			0);
	}
	// Throw error and quit if a broker could not be created
	catch (...) {
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

	std::vector<AL::ALProxy> proxyVec;

	// initialise proxies
	AL::ALProxy bodyInfoProxy(bodyModuleName, pip, pport);
	AL::ALProxy movementToolsProxy(movementModuleName, pip, pport);
	AL::ALMotionProxy motion(pip, pport);

	proxyVec.push_back(bodyInfoProxy);
	proxyVec.push_back(movementToolsProxy);
	proxyVec.push_back(motion);

	return proxyVec;
}

/**
 * @brief Program launcher!
 *
 * Contains all initialisations including setting up the StateSpace and connecting to robot via a proxy. Holds main
 * program loop to perform algorithm continuously using calls to selectAction and updateQ sequentially to update
 * utilities and choose most optimal actions to perform based on position and velocity.
 * 
 * @return Program exit code
 */
int main() {
	
	// initialise proxies and robot connection and set movementToolsProxy object to 2nd
	// element of robotProxyInitialisation return vector 
	AL::ALProxy movementToolsProxy = robotProxyInitialisation()[1];

	// Learning rate - set low for highly stochastic systems, high for less random and more symmetric systems
	const double alpha = 0.8;
	// Discount factor - set low for disregarding future events, high for taking the future into more consideration
	const double gamma = 0.5;

	// Seed PRNG with current system time
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	// possible actions, initialise starting action to full forwards motion
	int action_forwards = FORWARD;
	int action_backwards = BACKWARD;
	int chosen_action = action_forwards;

	// create a priority queue to copy to all the state space priority queues
	PriorityQueue<int, double> initiator_queue(MAX);

	// enqueue possible actions with intial zero utilities to initiator_queue
	initiator_queue.enqueueWithPriority(action_forwards, 0);
	initiator_queue.enqueueWithPriority(action_backwards, 0);

	// create encoder and calibrate to current angle
	Encoder encoder;
	encoder.Calibrate();
	
	std::cout << "Encoder angle calibrated... " << std::endl;
	
	//pause briefly to allow the robot to be given a push if desired
	qi::os::msleep(5000);
	
	//create the state space, initialised with number of bins for angle and velocity
	// and maximum angle and velocities for discretisation limits (alter if necessary)
	const int angleBins = 100;
	const int velocityBins = 50;
	const double angleMax = 0.25*M_PI;
	const double velocityMax = 1.0;
	StateSpace space(angleBins, velocityBins, angleMax, velocityMax, initiator_queue);
	
	const char* serializedSpacePath = "serializedStateSpaceData.txt";
	
	// create output file to send encoder data to
	const char* encoderDataPath = "encoderData.txt";
	std::ofstream encoderOutput(encoderDataPath);
	
	// if file containing previous state space data exists
	// get handle to this file and stream contents to space object
	if (fileExists(serializedSpacePath)) {
		std::ifstream inputFile(serializedSpacePath);
		std::string firstFileLine;
		std::getline(inputFile, firstFileLine);
		// if file is not empty, save file data to state space object
		if (!firstFileLine.empty())
			inputFile >> space;
	}
	
	// Create State objects for current state and previous state
	State current_state(0.0, 0.0, FORWARD);
	State old_state(0.0, 0.0, FORWARD);

	// initialise epsilon parameter for greedy search to 0.0 (fully stochastic)
	double epsilon = 0.0;

	std::cout << "Initialisation complete!" << std::endl;

	bool useEpsilonGreedy = true;

	// Each iteration currently requires 700ms time for action performing
	// => increase maxIterations for longer learning times
	const unsigned long maxIterations = 500UL;
	for(unsigned long i = 0; i < maxIterations; ++i) {
		// set current state angle to angle received from encoder
		// and set current state velocity to difference in new and
		// old state angles over some time difference
		current_state.theta = M_PI * (encoder.GetAngle()) / 180;
		current_state.theta_dot = (current_state.theta - old_state.theta) / 700; //Needs actual time
		current_state.robot_state = static_cast<ROBOT_STATE>(chosen_action);

		// save encoder data to encoderData.txt output file
		encoderOutput << current_state.theta << std::endl;

		// call updateQ function with state space, previous and current states
		// and learning rate, discount factor
		updateQ(space, chosen_action, old_state, current_state, alpha, gamma);

		// set old_state to current_state
		old_state = current_state;
		
		// after 100 iterations, start slowly increasing epsilon to
		// reduce stochasticity of action learning
		if (i > 100) {
			epsilon += 0.005;	
		}
		
		// determine chosen_action for current state
		if (useEpsilonGreedy) 
			chosen_action = selectAction_EpsilonGreedy(space[current_state], epsilon);
		else 
			chosen_action = selectAction_BoltzmannFactor(space[current_state], i);

		// depending upon chosen action, call robot movement tools proxy with either
		// swingForwards or swingBackwards commands.
		(chosen_action) ? movementToolsProxy.callVoid("swingForwards") : movementToolsProxy.callVoid("swingBackwards");
	}
	
	// create output file for sending final contents of StateSpace object to, allowing 
	// use of previously acquired learning runs to use for future learning runs
	std::ofstream serializedStateSpace(serializedSpacePath);
	serializedStateSpace<<space;
	serializedStateSpace.close();
	encoderOutput.close();
	
	return 1;
}

double probabilityFluxDensityCoefficient(unsigned long t) {
	return 100.0*std::exp((-8.0*t*t) / (2600.0*2600.0)) + 0.1;//0.1 is an offset
}

int selectAction_BoltzmannFactor(const PriorityQueue<int, double>& a_queue, unsigned long iterations) {
	
	typedef std::vector<std::pair<int, double> > VecPair ; 
	
	//turn priority queue into a vector of pairs
	VecPair vec = a_queue.saveOrderedQueueAsVector();

    //sum for partition function 
	double sum = 0.0;

	// calculate partition function by iterating over action-values
	for (VecPair::iterator iter = vec.begin(), end = vec.end(); iter < end; ++iter) {
		sum += std::exp((iter->second) / probabilityFluxDensityCoefficient(iterations));
	}

	// compute Boltzmann factors for action-values and enqueue to vec
	for (VecPair::iterator iter = vec.begin(); iter < vec.end(); ++iter) {
		iter->second = std::exp(iter->second / probabilityFluxDensityCoefficient(iterations)) / sum;
	}

	// calculate cumulative probability distribution
	for (VecPair::iterator iter = vec.begin(), end = vec.end(); iter < end; ++iter) {
        //second member of pair becomes addition of its current value
        //and that of the index before it
        	if (iter != vec.begin())
			iter->second += (iter-1)->second;
	}

	//generate RN between 0 and 1
	double rand_num = static_cast<double>(rand()) / RAND_MAX;

	// choose action based on random number relation to priorities within action queue
	for (VecPair::iterator iter = vec.begin(), end = vec.end(); iter < end; ++iter) {
		if (rand_num < iter->second)
			return iter->first;
	}
	
	return -1; //note that this line should never be reached	
}

int selectAction_EpsilonGreedy(const PriorityQueue<int, double>& a_queue, double epsilon) {

	// generate random number between 0 and 1
	double rand_num = static_cast<double>(rand()) / RAND_MAX;

	// if random double is less than epsilon, take action of front element of queue
	// i.e. the action with current highest utility value
	if(rand_num < epsilon){
		return a_queue.peekFront().first;
	}
	
	// else generate another random double and use this to access a random
	// element in the queue to take action from
	rand_num = static_cast<double>(rand()) / RAND_MAX;
	return a_queue[ round( rand_num*(a_queue.getSize()-1) ) ].first;
}

void updateQ(StateSpace & space, int action, State & new_state, State & old_state, double alpha, double gamma) {
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