#include <iostream>
#include <cstdlib>
#include <qi/os.hpp>
#include <boost/shared_ptr.hpp>

#include "moduletest.h"

#include <alcommon/almodule.h>
#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>
#include <alcommon/alproxy.h>
#include <alproxies/altexttospeechproxy.h>

#ifdef _WIN32
	#define ALCALL __declspec(dllexport)
#else
	#define ALCALL
#endif

extern "C"
{
	ALCALL int _createModule(boost::shared_ptr<AL::ALBroker> broker)
	{
		// Initiate broker with main broker instance
		AL::ALBrokerManager::setInstance(broker->fBrokerManager.lock());
		AL::ALBrokerManager::getInstance()->addBroker(broker);

		// Create instance of module
		AL::ALModule::createModule<ModuleTest>(broker, "ModuleTest");
	}

	ALCALL int _closeModule()
	{
		return 0;
	}
}; /* extern "C" */

#ifdef MODULETEST_IS_REMOTE
int main(int argc, char* argv[])
{
	// Default port and ip
	int pport = 9559;
	std::string pip = "127.0.0.1";

	// Check number of command line arguments
	if (argc != 1 && argc != 3 && argc != 5)
	{
		std::cerr << "Wrong number of arguments!" << std::endl;
		std::cerr << "Usage: moduletest [--pip robot_ip] [--pport port]" << std::endl;
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
			std::cerr << "Usage: moduletest [--pip robot_ip] [--pport port]" << std::endl;
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
			std::cerr << "Usage: moduletest [--pip robot_ip] [--pport port]" << std::endl;
			exit(2);
		}
	}

	// Need this for SOAP serialisation of floats to work
	setlocale(LC_NUMERIC, "C");

	// Set broker name, ip and port
	const std::string brokerName = "moduleTestBroker";
	int brokerPort = 54000;
	const std::string brokerIp = "0.0.0.0";

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

	// Load the module
	AL::ALModule::createModule<ModuleTest>(broker, "ModuleTest");

	// Create a proxy
	AL::ALProxy testProxy = AL::ALProxy("ModuleTest", pip, pport);

	testProxy.callVoid("sayHello");

	return broker->shutdown();
}
#endif /* MODULETEST_IS_REMOTE */
