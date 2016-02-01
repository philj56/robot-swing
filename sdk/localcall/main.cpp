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
	
	std::cerr << "Wrong number of arguments!" << std::endl;
	std::cerr << usage << std::endl;
	exit(2);
}

int main(int argc, char* argv[])
{
	// Default name of desired library
	std::string libName = "libmoduletest.so";
	
	// Default name of desired module in library
	std::string moduleName = "ModuleTest";

	// Default name of void(void) function to call in module
	std::string funcName = "printHello";

	// Set broker name, ip and port, finding first available port from 54000
	const std::string brokerName = "localCallBroker";
	int brokerPort = qi::os::findAvailablePort(54000);
	const std::string brokerIp = "0.0.0.0";
	
	// Default parent port and ip
	int pport = 9559;
	std::string pip = "127.0.0.1";
	
	// Check for odd number of command line arguments (in this case)
	if (argc % 2 != 1)
	{
		argErr();
	}
	
	// Get any arguments
	while (true)
	{
		static int index = 0;

		// Struct of options
		static const struct option longopts[] =
		{
			{"pip", 	1, 	0, 	'p'},
			{"pport",	1,	0,	'o'},
			{"lib",		1,	0,	'l'},
			{"mod",		1,	0,	'm'},
			{"fun",		1,	0,	'f'},
			{0,		0,	0,	 0 }
		};

		switch(index = getopt_long(argc, argv, "", longopts, &index))
		{
			case 'p':
				if (optarg)
					pip = std::string(optarg);
				else
					argErr();
				break;
			case 'o':
				if (optarg)
					pport = atoi(optarg);
				else
					argErr();
				break;
			case 'l':
				if (optarg)
					libName = std::string(optarg);
				else
					argErr();
				break;
			case 'm':
				if (optarg)
					moduleName = std::string(optarg);
				else
					argErr();
				break;
			case 'f':
				if (optarg)
					funcName = std::string(optarg);
				else
					argErr();
				break;
		}
		if (index == -1)
			break;
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
	{
		boost::shared_ptr<AL::ALModuleCore> module = broker->getModuleByName(moduleName);
		std::cout << bold_on << "Closing module " << moduleName << "..." << bold_off << std::endl;
		module->exit();
	}

	// Check module has closed
	std::cout << bold_on << "Module " << moduleName << " is ";
	if (!(broker->isModulePresent(moduleName)))
	{
		std::cout << "not ";
	}
	std::cout << "present" << bold_off << std::endl;

	// Close the broker
	std::cout << bold_on << "Closing broker..." << bold_off << std::endl;
	broker->shutdown();

	// Exit program
	std::cout << bold_on << "Exiting..." << bold_off << std::endl;

	return 0;
}
