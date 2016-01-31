/* main.cpp
 * localcall main file - locally loads a shared library, instantiates a module and calls a function from that module 
 */

#include <iostream>
#include <cstdlib>
#include <boost/shared_ptr.hpp>
#include <qi/os.hpp>
#include <qi/path.hpp>
#include <qi/log.hpp>


#include <alcommon/almodule.h>
#include <alcommon/alproxy.h>
#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>

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

int main(int argc, char* argv[])
{
	// Name of desired library
	std::string libName = "libmoduletest.so";
	
	// Name of desired module in library
	std::string moduleName = "ModuleTest";

	// Name of void(void) function to call in class
	std::string funcName = "printHello";

	// Set broker name, ip and port, finding first available port from 54000
	const std::string brokerName = "localCallBroker";
	int brokerPort = qi::os::findAvailablePort(54000);
	const std::string brokerIp = "0.0.0.0";
	
	// Default parent port and ip
	int pport = 9559;
	std::string pip = "127.0.0.1";
	
	// Check number of command line arguments
	if (argc != 1 && argc != 3 && argc != 5)
	{
		std::cerr << "Wrong number of arguments!" << std::endl;
		std::cerr << "Usage: localcall [--pip robot_ip] [--pport port]" << std::endl;
		exit(2);
	}

	// If there is only one argument it should be the IP or PORT
	if (argc == 3)
	{
		if (std::string(argv[1]) == "--pip")
		{
			pip = argv[2];
		}
		else if (std::string(argv[1]) == "--pport")
		{
			pport = atoi(argv[2]);
		}
		else
		{
			std::cerr << "Wrong number of arguments!" << std::endl;
			std::cerr << "Usage: localcall [--pip robot_ip] [--pport port]" << std::endl;
			exit(2);
		}
	}

	// Specified IP or PORT for the connection
	if (argc == 5)
	{
		if (std::string(argv[1]) == "--pport"
		    && std::string(argv[3]) == "--pip")
		{
			pport = atoi(argv[2]);
			pip = argv[4];
		}
		else if (std::string(argv[3]) == "--pport"
			 && std::string(argv[1]) == "--pip")
		{
			pport = atoi(argv[4]);
			pip = argv[2];
		}
		else
		{
			std::cerr << "Wrong number of arguments!" << std::endl;
			std::cerr << "Usage: localcall [--pip robot_ip] [--pport port]" << std::endl;
			exit(2);
		}
	}

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

	// Find the desired library
	std::cout << bold_on << "Finding " << libName << "..." << bold_off << std::endl;
	std::string library = qi::path::findLib(libName.c_str());

	// Open the library
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
	std::cout << bold_on << "Module " << moduleName << " is ";
	if (!(broker->isModulePresent(moduleName)))
	{
		std::cout << "not ";
	}
	std::cout << "present" << bold_off << std::endl;

	// Create an instance of the desired module
	std::cout << bold_on << "Creating " << moduleName << " instance..." << bold_off << std::endl;
	createModule(broker);

	// Check for module creation
	std::cout << bold_on << "Module " << moduleName.c_str() << " is ";
	if (!(broker->isModulePresent(moduleName)))
	{
		std::cout << "not ";
	}
	std::cout << "present" << bold_off << std::endl;

	// Create a proxy to the module	
	std::cout << bold_on << "Creating proxy to module..." << bold_off << std::endl;
	AL::ALProxy testProxy(moduleName, pip, pport);
	
	// Call a generic function from the module with no arguments
	std::cout << bold_on << "Calling function " << funcName << "..." << bold_off << std::endl;
	testProxy.callVoid(funcName);

	// Get a handle to the module and close it
	boost::shared_ptr<AL::ALModuleCore> module = broker->getModuleByName(moduleName);
	std::cout << bold_on << "Closing module " << moduleName << "..." << bold_off << std::endl;
	module->exit();
	
	// Check module has closed
	std::cout << bold_on << "Module " << moduleName << " is ";
	if (!(broker->isModulePresent(moduleName)))
	{
		std::cout << "not ";
	}
	std::cout << "present" << bold_off << std::endl;

	return 0;
}
