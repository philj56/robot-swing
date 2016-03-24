/* main.cpp
 * localcall main file - locally loads a shared library, instantiates a module and calls a function from that module 
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

#include "createmodule.h"

// Find and open a library, and create an instance of a module in that library
bool CreateModule(std::string libName, std::string moduleName, boost::shared_ptr<AL::ALBroker> broker, bool verb, bool find);

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
	std::string usage = "Usage: localcall [--pip robot_ip] [--pport port] [--lib library] [--mod module] [--fun function]";
	std::cerr << usage << std::endl;
	exit(2);
}

int main(int argc, char* argv[])
{
	// Whether to print verbose output - default off
	bool verb = false;
	
	// Whether to manually load Libraries - default off
	bool init = false;

	// Default name of desired library
	std::string libName = "libmoduletest.so";
	
	// Default name of desired module in library
	std::string moduleName = "ModuleTest";

	// Default name of void(void) function to call in module
	std::string funcName = "printHello";

	// Explicit library path
	std::string libPath = "";
	
	// Motion library path
	std::string motionLibPath = "/usr/lib/naoqi/libmotion.so";

	// Set broker name, ip and port, finding first available port from 54000
	const std::string brokerName = "localCallBroker";
	int brokerPort = qi::os::findAvailablePort(54000);
	const std::string brokerIp = "0.0.0.0";
	
	// Default parent port and ip
	int pport = 9559;
	std::string pip = "127.0.0.1";
	
	// Get any arguments
	while (true)
	{
		static int index = 0;

		// Struct of options
		// Columns are:
		// 	{Option name,	Option required?,	flag(not needed here),	return value/character}
		static const struct option longopts[] =
		{
			{"pip", 	1, 	0, 	'i'},
			{"pport",	1,	0,	'p'},
			{"lib",		1,	0,	'l'},
			{"mod",		1,	0,	'm'},
			{"fun",		1,	0,	'f'},
			{"path",	1,	0,	'x'},
			{"verb",	0,	0,	'v'},
			{"help",	0,	0,	'h'},
			{0,		0,	0,	 0 }
		};

		// Get next option, and check return value
		// p:o:l:m:f:vh allow short option specification
		switch(index = getopt_long(argc, argv, "i:p:l:m:f:vh", longopts, &index))
		{
			// Print usage and quit
			case 'h':
				argErr();
				break;
			// Set parent IP
			case 'i':
				if (optarg)
					pip = std::string(optarg);
				else
					argErr();
				break;
			// Set parent port
			case 'p':
				if (optarg)
					pport = atoi(optarg);
				else
					argErr();
				break;
			// Set library name
			case 'l':
				if (optarg)
					libName = std::string(optarg);
				else
					argErr();
				break;
			// Set module name
			case 'm':
				if (optarg)
					moduleName = std::string(optarg);
				else
					argErr();
				break;
			// Set function name
			case 'f':
				if (optarg)
					funcName = std::string(optarg);
				else
					argErr();
				break;
			// Use explicit file path
			case 'x':
				if (optarg)
					libPath = std::string(optarg);
				else
					argErr();
				break;
			// Verbose output
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
	AL::ALProxy testProxy(moduleName, pip, pport);
	
	// Call a generic function from the module with no arguments
	if(verb)
		std::cout << bold_on << "Calling function " << funcName << "..." << bold_off << std::endl;
	testProxy.callVoid(funcName);

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

