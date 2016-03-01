/* main.cpp
 * motiontiming main file - locally loads a shared library, instantiates a module and calls a function from that module 
 */

#include <iostream>
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

// Find and open a library, and create an instance of a module in that library
bool createModule(std::string libName, std::string moduleName, boost::shared_ptr<AL::ALBroker> broker, bool verb, bool find);

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
	std::string usage = "Usage: motiontiming [--pip robot_ip] [--pport port] [--lib library] [--mod module] [--fun function]";
	std::cerr << usage << std::endl;
	exit(2);
}

int main(int argc, char* argv[])
{
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

	for (int i = 1; i <= 10; i++)
	{
		std::cout << "Speed: " << i * 0.1 << std::endl;
		movementToolsProxy.callVoid("setSpeed", i*0.1);
		float moveTime = 0;
		qi::os::timeval startTime;
		qi::os::timeval currentTime;
		qi::os::timeval endTime;
		std::vector<float> commandAngles;
		std::vector<float> sensorAngles;
	//	std::cout << "Hip angle: " << bodyInfoProxy.genericCall("getHipPitch", 0) << std::endl; 
		gettimeofday(&startTime);
	
		movementToolsProxy.callVoid("swingForwards");
		qi::os::msleep(40);
	
		commandAngles = motion.getAngles("Body", false);
		
		bool equal = false;
		while (!equal)
		{
			gettimeofday(&currentTime);
			sensorAngles = motion.getAngles("Body", true);
			for (unsigned int i = 0; i < commandAngles.size(); i++)
				{
				if (commandAngles[i] == sensorAngles[i])
					if (i = commandAngles.size() - 1)
						equal = true;
				else 
					break;
			}
		}
		gettimeofday(&endTime);
	
		moveTime = 1000 * (static_cast<int>(endTime.tv_sec) - static_cast<int>(startTime.tv_sec)) 
			       + 0.001 * static_cast<float>(static_cast<int>(endTime.tv_usec) - static_cast<int>(startTime.tv_usec));
		
		std::cout << "Swing Forwards Time:" << moveTime << " ms" << std::endl;	
	
		gettimeofday(&startTime);
	
		movementToolsProxy.callVoid("swingBackwards");
		qi::os::msleep(40);
	
		commandAngles = motion.getAngles("Body", false);
			
		equal = false;
		while (!equal)
		{
			sensorAngles = motion.getAngles("Body", true);
			for (unsigned int i = 0; i < commandAngles.size(); i++)
			{
					if (commandAngles[i] == sensorAngles[i])
					if (i = commandAngles.size() - 1)
						equal = true;
				else 
					break;
			}
		}		
		gettimeofday(&endTime);
		
		moveTime = 1000 * (static_cast<int>(endTime.tv_sec) - static_cast<int>(startTime.tv_sec)) 
			 + 0.001 * static_cast<float>(static_cast<int>(endTime.tv_usec) - static_cast<int>(startTime.tv_usec));
	
		std::cout << "Swing Backwards Time:" << moveTime << " ms" << std::endl;
	}
}
	
// Find and open a library, and create an instance of a module in that library
bool createModule(std::string libName, std::string moduleName, boost::shared_ptr<AL::ALBroker> broker, bool verb, bool find)
{
	std::string library;
	
	if (find)
	{
		// Find the desired library
		if (verb)
			std::cout << bold_on << "Finding " << libName << "..." << bold_off << std::endl;
		library = qi::path::findLib(libName.c_str());

	}
	else
	{
		// Use libName as library path
		library = libName;
	}

	// Open the library
	if(verb)
		std::cout << bold_on << "Loading " << library << "..." << bold_off << std::endl;
	void* handle = qi::os::dlopen(library.c_str());
	if (!handle)
	{
		qiLogWarning(moduleName.c_str()) << "Could not load library:"
					 	<< qi::os::dlerror()
					   	<< std::endl;
		return -1;
	}

	// Load the create symbol
	if(verb)
		std::cout << bold_on << "Loading _createModule symbol..." << bold_off << std::endl;
	int(*createModule)(boost::shared_ptr<AL::ALBroker>) = (int(*)(boost::shared_ptr<AL::ALBroker>))(qi::os::dlsym(handle, "_createModule"));
	if (!createModule)
	{
		qiLogWarning(moduleName.c_str()) << "Could not load symbol _createModule: "
					   	<< qi::os::dlerror()
					   	<< std::endl;
		return -1;
	}

	// Check if module is already present
	if(verb)
	{
		std::cout << bold_on << "Module " << moduleName << " is ";
		if (!(broker->isModulePresent(moduleName)))
		{
			std::cout << "not ";
		}
		std::cout << "present" << bold_off << std::endl;
	}

	// Create an instance of the desired module
	if(verb)
		std::cout << bold_on << "Creating " << moduleName << " instance..." << bold_off << std::endl;
	createModule(broker);

	// Check for module creation
	if(verb)
	{
		std::cout << bold_on << "Module " << moduleName.c_str() << " is ";
		if (!(broker->isModulePresent(moduleName)))
		{
			std::cout << "not ";
		}
		std::cout << "present" << bold_off << std::endl;
	}
	if (broker->isModulePresent(moduleName))
		return true;
	else
		return false;
}
