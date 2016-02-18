/* cameratools.h
 * Generic module header file
 */

#ifndef CAMERATOOLS_H
#define CAMERATOOLS_H

#include <iostream>
#include <cstdlib>
#include <alcommon/almodule.h>
#include <alcommon/albroker.h>
#include <alcommon/alproxy.h>
#include <alproxies/alphotocaptureproxy.h>
#include <alproxies/allandmarkdetectionproxy.h>
#include <alproxies/almemoryproxy.h>
#include <alvalue/alvalue.h>

#include <qi/os.hpp>

// Forward declaration of ALBroker
namespace AL
{
	class ALBroker;
}

// CameraTools - generic NAOqi module
class CameraTools : public AL::ALModule
{
public:
	// Constructor
	CameraTools(boost::shared_ptr<AL::ALBroker> broker,
		 const std::string &name);

	// Destructor
	virtual ~CameraTools();

	// init() - called just after the constructor
	virtual void init();

	// Testing camera resolution
	void setRes(const int &res);
	int getRes();

	// Testing camera resolution
	void setRate(const int &rate);
	int getRate();

	// Start and stop landmark detection
	void startLandmarkDetection();
	void stopLandmarkDetection();

	// Read memory for any detected landmarks
	AL::ALValue getLandmark();
	
private:
	// Broker Parent IP and port
	std::string pip;
	int pport;

	// Whether landmark detection is running
	bool landmarkRunning;

	// Landmark memory key
	std::string memLandmark;
	
	// Proxies to relevant modules
	AL::ALPhotoCaptureProxy photoProxy;
	AL::ALLandMarkDetectionProxy landmarkProxy;
	AL::ALMemoryProxy memoryProxy;
};

#endif /* CAMERATOOLS_H */
