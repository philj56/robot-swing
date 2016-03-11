/* main.cpp
 * localcall main file - locally loads a shared library, instantiates a module and calls a function from that module 
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <getopt.h>
#include <boost/shared_ptr.hpp>
#include <qi/os.hpp>
#include <qi/path.hpp>
#include <qi/log.hpp>

#include <alcommon/almodule.h>
#include <alcommon/alproxy.h>
#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>
#include <alproxies/almotionproxy.h>
#include <alproxies/alrobotpostureproxy.h>

#include "encoder.h"
#include "createmodule.h"

// Turn bold text on
std::ostream& bold_on(std::ostream& os)
{
	    return os << "\e[1m";
}

// Turn bold text off
std::ostream& bold_off(std::ostream& os)
{
	    return os << "\e[0m";
}

// Wrong number of arguments
void argErr(void)
{
	// Standard usage message
	std::string usage = "Usage: humanmotion [--pip robot_ip] [--pport port]";
	std::cerr << usage << std::endl;
	exit(2);
}

int main(int argc, char* argv[])
{
	// Whether to print verbose output - default off
	bool verb = false;
	
	// Whether to manually load Libraries - default off
	bool init = false;

	// Name of desired library
	std::string libName = "movementtools";

	// Name of desired module in library
	std::string moduleName = "MovementTools";

	// Explicit library path
	std::string libPath = "";

	// Set broker name, ip and port, finding first available port from 54000
	const std::string brokerName = "HumanMotionBroker";
	int brokerPort = qi::os::findAvailablePort(54000);
	const std::string brokerIp = "0.0.0.0";
	
	// Default parent port and ip
	int pport = 9559;
	std::string pip = "127.0.0.1";
	
	// Default time to run in seconds
	int timeToRun = 10;

	// Get any arguments
	while (true)
	{
		static int index = 0;

		// Struct of options
		// Columns are:
		// 	{Option name,	Option required?,	flag(not needed here),	return value/character}
		static const struct option longopts[] =
		{
			{"verb",	0,	0,	'v'},
			{"help",	0,	0,	'h'},
			{0,		0,	0,	 0 }
		};

		// Get next option, and check return value
		switch(index = getopt_long(argc, argv, "vh", longopts, &index))
		{
			// Print usage and quit
			case 'h':
				argErr();
				break;
			case 'v':
				verb = true;
				break;
		}
		if (index == -1)
			break;
	}

	// Need this for SOAP serialisation of floats to work
	setlocale(LC_NUMERIC, "C");

	// Create a broker
	if(verb)
		std::cout << bold_on << "Creating broker..." << bold_off << std::endl;
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

	// Create an instance of the desired module
	if (libPath == "")
		CreateModule(libName, moduleName, broker, verb, true);
	else
		CreateModule(libPath, moduleName, broker, verb, false);

	
	// Create a proxy to the module	
	if(verb)
		std::cout << bold_on << "Creating proxy to module..." << bold_off << std::endl;
	AL::ALProxy movementToolsProxy(moduleName, pip, pport);
	
	if(verb)
		std::cout << bold_on << "Creating encoder..." << bold_off << std::endl;
	Encoder encoder;
	
	float lastAngle;
	float currentAngle = encoder.GetAngle();

	// Min, max for last swing
	float minAngle = 0;
	float maxAngle = 1;

	// Bools for whether the robot is moving forwards or backwards
	bool forwards;
	bool backwards;

	// Get start time of simulation and store it
	qi::os::timeval startTime;
	qi::os::timeval currentTime;
	qi::os::gettimeofday(&startTime);
	qi::os::gettimeofday(&currentTime);

	std::ofstream outFile("/home/nao/humanSwingOut.txt");

	// Run for time "timeToRun"
	while (currentTime.tv_sec - startTime.tv_sec < timeToRun)
	{
		qi::os::gettimeofday(&currentTime);
		lastAngle = currentAngle;
		currentAngle = encoder.GetAngle();
		std::cout << "Current angle: " << currentAngle << std::endl;
		// Check for direction of motion, and a change in direction
		forwards = currentAngle > lastAngle;
		if (forwards == backwards)
		{
			if (forwards)
			{
				minAngle = currentAngle;
				std::cout << currentTime.tv_sec << "." << currentTime.tv_usec << ": minAngle = " << minAngle << std::endl; 
			}
			else
			{
				maxAngle = currentAngle;
				std::cout << currentTime.tv_sec << "." << currentTime.tv_usec << ": maxAngle = " << maxAngle << std::endl; 
			}
		}
		backwards = !forwards;

		// Ensuer humanSwing is only called with an angle between 0 and 1
		movementToolsProxy.callVoid("humanSwing", std::abs(currentAngle - minAngle) / std::abs(maxAngle - minAngle), forwards); 
		qi::os::msleep(100);
	}

	// Get a handle to the module and close it
	{
		boost::shared_ptr<AL::ALModuleCore> module = broker->getModuleByName(moduleName);
		if(verb)
			std::cout << bold_on << "Closing module " << moduleName << "..." << bold_off << std::endl;
		module->exit();
	}

	// Check module has closed
	if(verb)
	{
		std::cout << bold_on << "Module " << moduleName << " is ";
		if (!(broker->isModulePresent(moduleName)))
		{
			std::cout << "not ";
		}
		std::cout << "present" << bold_off << std::endl;
	}
	
	// Close the broker
	if(verb)
		std::cout << bold_on << "Closing broker..." << bold_off << std::endl;
	broker->shutdown();

	// Exit program
	if(verb)
		std::cout << bold_on << "Exiting..." << bold_off << std::endl;

	return 0;
}
