/* cameratools.cpp
 * various camera tools source file
 */

#include "cameratools.h"

// Constructor
CameraTools::CameraTools(boost::shared_ptr<AL::ALBroker> broker,
			 const std::string &name)
	: AL::ALModule(broker, name)
{
	// Set description, and bind each function
	setModuleDescription("Camera testing module");

	functionName("setRes", getName(), "Set the photo capture resolution");
	addParam("res", "The resolution to set");
	BIND_METHOD(CameraTools::setRes);

	functionName("getRes", getName(), "Get the photo capture resolution");
	BIND_METHOD(CameraTools::setRes);
	
	functionName("setRate", getName(), "Set the photo capture rate");
	addParam("rate", "The capture interval to set");
	BIND_METHOD(CameraTools::setRate);

	functionName("getRate", getName(), "Get the photo capture rate");
	BIND_METHOD(CameraTools::getRate);

	functionName("startLandmarkDetection", getName(), "Start landmark detection writing to memory");
	BIND_METHOD(CameraTools::startLandmarkDetection);

	functionName("stopLandmarkDetection", getName(), "Stop landmark detection writing to memory");
	BIND_METHOD(CameraTools::stopLandmarkDetection);

	functionName("getLandmark", getName(), "Check for any detected landmarks");
	BIND_METHOD(CameraTools::getLandmark);
	
	// Set broker parent IP and port
	pip = broker->getParentIP();
	pport = broker->getParentPort();

	landmarkRunning = false;
		
	// The memory value where landmark detection stores results
	memLandmark = "LandmarkDetected";
}

// Destructor
CameraTools::~CameraTools()
{
}

// init() - called as soon as the module is constructed
void CameraTools::init()
{
	try
	{
		// Get Proxies to desired modules
		photoProxy    = AL::ALPhotoCaptureProxy(pip, pport);
		landmarkProxy = AL::ALLandMarkDetectionProxy(pip, pport);
		memoryProxy   = AL::ALMemoryProxy(pip, pport);
	}	
  	catch (const AL::ALError& e) 
	{
    		std::cerr << "Caught exception: " << e.what() << std::endl;
  	}
}

void CameraTools::setRes(const int &res)
{
	photoProxy.setResolution(res);
}

int CameraTools::getRes()
{
	return photoProxy.getResolution();
}

void CameraTools::setRate(const int &rate)
{
	photoProxy.setCaptureInterval(rate);
}

int CameraTools::getRate()
{
	return photoProxy.getCaptureInterval();
}

// Start landmark detection writing to memory every 50ms
void CameraTools::startLandmarkDetection()
{
	if (!landmarkRunning)
	{
		landmarkProxy.subscribe("CameraTools", 50, 0.0);
		landmarkRunning = true;
	}
}

void CameraTools::stopLandmarkDetection()
{
	if (landmarkRunning)
	{
		landmarkProxy.unsubscribe("CameraTools");
		landmarkRunning = false;
	}
}

AL::ALValue CameraTools::getLandmark()
{
	return memoryProxy.getData(memLandmark);
}
