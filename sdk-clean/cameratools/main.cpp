/* main.cpp
 * cameratools main file,
 */

#include <iostream>
#include <cstdlib>
#include <boost/shared_ptr.hpp>
#include <qi/os.hpp>

#include "cameratools.h"

#include <alcommon/almodule.h>
#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>
#include <alcommon/alproxy.h>
#include <alproxies/almotionproxy.h>
#include <alproxies/alrobotpostureproxy.h>

// Need to export entry point if on Windows
#ifdef _WIN32
	#define ALCALL __declspec(dllexport)
#else
	#define ALCALL
#endif

// Setup for local module
extern "C"
{
	ALCALL int _createModule(boost::shared_ptr<AL::ALBroker> broker)
	{
		// Initiate broker with main broker instance
		AL::ALBrokerManager::setInstance(broker->fBrokerManager.lock());
		AL::ALBrokerManager::getInstance()->addBroker(broker);

		// Create instance of module
		AL::ALModule::createModule<CameraTools>(broker, "CameraTools");
	}

	ALCALL int _closeModule()
	{
		return 0;
	}
}; /* extern "C" */
