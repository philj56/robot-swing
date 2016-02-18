/* main.cpp
 * localcall main file - locally loads a shared library, instantiates a module and calls a function from that module 
 */

#include <iostream>
#include <iomanip>
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
bool CreateModule(std::string libName, std::string moduleName, boost::shared_ptr<AL::ALBroker> broker, bool verb, bool find);

// Init some modules
void Init (boost::shared_ptr<AL::ALBroker> broker, bool verb);

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
	std::string usage = "Usage: motionderivative [--pip robot_ip] [--pport port]";
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
	std::string libName = "libbodyinfo.so";
	
	// Name of desired module in library
	std::string moduleName = "BodyInfo";

	// Explicit library path
	std::string libPath = "";

	// Set broker name, ip and port, finding first available port from 54000
	const std::string brokerName = "MotionDerivativeBroker";
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
			{"pip", 	1, 	0, 	'i'},
			{"pport",	1,	0,	'p'},
			{"path",	1,	0,	'x'},
			{"time",	0,	0,	't'},
			{"verb",	0,	0,	'v'},
			{"help",	0,	0,	'h'},
			{0,		0,	0,	 0 }
		};

		// Get next option, and check return value
		switch(index = getopt_long(argc, argv, "i:p:t:vh", longopts, &index))
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
			case 'x':
				if (optarg)
					libPath = std::string(optarg);
				else
					argErr();
				break;
			case 't':
				if (optarg)
					timeToRun = atoi(optarg);
				else
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
	AL::ALProxy bodyProxy(moduleName, pip, pport);
	
	// Get COM angles as vector of [upper body, lower body]
	std::vector<float> COMAngles = bodyProxy.genericCall("getSittingCOMAngles", 0);

	// Get start time of simulation and store it
	qi::os::timeval startTime;
	qi::os::timeval currentTime;
	qi::os::gettimeofday(&startTime);
	qi::os::gettimeofday(&currentTime);

	// Run for time "timeToRun"
	while (currentTime.tv_sec - startTime.tv_sec < timeToRun)
	{
		std::cout << std::setw(15) << std::left << "Upper Body:" << COMAngles[0] << std::endl;
		std::cout << std::setw(15) << std::left << "Lower Body:" << COMAngles[1] << std::endl;
		qi::os::sleep(1);
		qi::os::gettimeofday(&currentTime);
		COMAngles = bodyProxy.genericCall("getSittingCOMAngles", 0);
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

// Find and open a library, and create an instance of a module in that library
bool CreateModule(std::string libName, std::string moduleName, boost::shared_ptr<AL::ALBroker> broker, bool verb, bool find)
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
