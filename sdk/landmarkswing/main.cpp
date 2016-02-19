/* main.cpp
 * landmarkswing main file - use landmark detection to detect changes in swinging direction, and move accordingly 
 */

#include "createmodule.h"

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

int vectorOrder (std::vector<float> vec);

// Wrong number of arguments
void argErr(void)
{
	// Standard usage message
	std::string usage = "Usage: landmarkswing [--pip robot_ip] [--pport port]";
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
	std::string libName = "cameratools";
	
	// Name of desired module in library
	std::string moduleName = "CameraTools";

	// Explicit library path
	std::string libPath = "";

	// Set broker name, ip and port, finding first available port from 54000
	const std::string brokerName = "LandmarkSwingBroker";
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
		std::cout << "Creating broker..." << std::endl;
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
		std::cout << "Creating proxy to module..." << std::endl;
	AL::ALProxy camToolsProxy(moduleName, pip, pport);
	
	// List of detected landmark info
	AL::ALValue landmarks;

	// Vectors of last landmark widths
	std::vector<float> landmark1Widths;
	std::vector<float> landmark2Widths;
	
	// IDs of desired landmarks FIXME -- need correct values
	const int landmark1ID = 0;
	const int landmark2ID = 1;

	// Whether each landmark has been detected this cycle
	bool landmark1Detected = false;
	bool landmark2Detected = false;

	// Number of previous widths to smooth over
	unsigned int landmarkSmoothing = 3;

	// Current direction of movement, either +1 or -1
	int currentDirection;

	// Get start time of simulation and store it
	qi::os::timeval startTime;
	qi::os::timeval currentTime;
	qi::os::gettimeofday(&startTime);
	qi::os::gettimeofday(&currentTime);

	// Run for time "timeToRun"
	while (currentTime.tv_sec - startTime.tv_sec < timeToRun)
	{
		// Reset landmark detection flags
		landmark1Detected = false;
		landmark2Detected = false;

		// Read memory for landmarks
		landmarks = camToolsProxy.genericCall("getLandmark", 0);
		
		std::cout << std::endl << "*****" << std::endl << std::endl;

		// Check for landmark detection
		if (landmarks.isValid() && landmarks.isArray() && landmarks.getSize() >= 2)
		{
			// Get widths of desired landmarks, and store them in appropriate vectors
			for (unsigned int i = 0; i < landmarks[1].getSize(); i++)
			{
				// Relevant parameters in landmark vector
				int   ID     = landmarks [1][i][1][0];
				float width  = landmarks [1][i][0][3];
				float height = landmarks [1][i][0][4];
	
				switch (ID)
				{
					case landmark1ID:
						landmark1Detected = true;
						landmark1Widths.push_back(width);
						if (landmark1Widths.size() > landmarkSmoothing + 1)
						{
							landmark1Widths.erase(landmark1Widths.begin());
						}
						break;
					case landmark2ID:
						landmark2Detected = true;
						landmark2Widths.push_back(width);
						if (landmark2Widths.size() > landmarkSmoothing + 1)
						{
							landmark2Widths.erase(landmark2Widths.begin());
						}
						break;
					default:
						std::cout << "Unkown landmark detected" << std::endl;
				}
				
				std::cout << "Mark ID: " << ID << std::endl;
				std::cout << "Width:   " << width << std::endl;
				std::cout << "Height:  " << height << std::endl;
			}

			/* TODO: position code for movement */
			// Check whether a landmark was detected this interval
			if (landmark1Detected || landmark2Detected)
			{
				// The order of each vector (+1, 0, -1)
				int order1 = vectorOrder(landmark1Widths);
				int order2 = vectorOrder(landmark2Widths);
				// Both landmarks detected
				if (landmark1Detected && landmark2Detected)
				{
					// Both landmarks moving in same direction
					if (order1 == order2)
					{
						if (order1 > 0)
						{
							// CODE FOR SOME DIRECTION
						}
						else if (order1 < 0)
						{
							// CODE FOR OTHER DIRECTION
						}
					}
				}
				// Only first landmark detected
				else if (landmark1Detected)
				{
					if (order1 > 0)
					{
						// CODE FOR SOME DIRECTION
					}
					else if (order1 < 0)
					{
						// CODE FOR OTHER DIRECTION
					}
				}
				// Only second landmark detected
				else
				{
					if (order2 > 0)
					{
						// CODE FOR SOME DIRECTION
					}
					else if (order2 < 0)
					{
						// CODE FOR OTHER DIRECTION
					}
				}
			}
		}
		else
		{
			std::cout << "No landmark detected" << std::endl;
		}
		
		qi::os::sleep(1);
		qi::os::gettimeofday(&currentTime);
	}

	// Get a handle to the module and close it
	{
		boost::shared_ptr<AL::ALModuleCore> module = broker->getModuleByName(moduleName);
		if(verb)
			std::cout << "Closing module " << moduleName << "..." << std::endl;
		module->exit();
	}

	// Check module has closed
	if(verb)
	{
		std::cout << "Module " << moduleName << " is ";
		if (!(broker->isModulePresent(moduleName)))
		{
			std::cout << "not ";
		}
		std::cout << "present" << std::endl;
	}
	
	// Close the broker
	if(verb)
		std::cout << "Closing broker..." << std::endl;
	broker->shutdown();

	// Exit program
	if(verb)
		std::cout << "Exiting..." << std::endl;

	return 0;
}

// Order of vector - 1 = ascending, 0 = neither, -1 = descending
// Non-zero order requires entire vector to follow that order
// Default to 0 (no order)
int vectorOrder (std::vector<float> vec)
{
	int order = 0;
	if (vec.size() > 1)
	{
		if (vec[1] > vec[0])
			order = 1;
		else if (vec[1] < vec[0])
			order = -1;
		else
			order = 0;
	}
	for (unsigned int i = 2; i < vec.size(); i++)
	{
		if (vec[i] > vec[i-1] && order == 1)
			continue;
		else if (vec[i] < vec[i-1] && order == -1)
			continue;
		else
			return 0;
	}
	return order;
}
