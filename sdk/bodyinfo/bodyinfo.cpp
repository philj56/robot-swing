/* bodyinfo.cpp
 * Get body angles source file
 */

#include "bodyinfo.h"
#include <alproxies/almotionproxy.h>

// Constructor
BodyInfo::BodyInfo(boost::shared_ptr<AL::ALBroker> broker,
			 const std::string &name)
	: AL::ALModule(broker, name)
{
	// Set description, and bind each function
	setModuleDescription("Various functions to obtain relevant body information");

	functionName("getHipPitch", getName(), "Read hip pitch angles");
	BIND_METHOD(BodyInfo::getHipPitch);

	functionName("getSittingCOMAngles", getName(), "Get COM angles from seat for sitting position");
	BIND_METHOD(BodyInfo::getSittingCOMAngles);
	
	// Set broker parent IP and port
	pip = broker->getParentIP();
	pport = broker->getParentPort();
}

// Destructor
BodyInfo::~BodyInfo()
{
}

// init() - called as soon as the module is constructed
void BodyInfo::init()
{
	try
	{
		motion = AL::ALMotionProxy(pip, pport);
		torsoMass = motion.getMass("Torso");
		lArmMass = motion.getMass("LArm");
		rArmMass = motion.getMass("RArm");
		headMass = motion.getMass("Head");
	}	
  	catch (const AL::ALError& e) 
	{
    		std::cerr << "Caught exception: " << e.what() << std::endl;
  	}
}

// Return the average hip pitch angle
float BodyInfo::getHipPitch()
{
	try
	{
	        std::vector<float> sensorAngles = motion.getAngles("Body", true);
		return (sensorAngles[10] + sensorAngles[16]) / 2.0;
	}	
  	catch (const AL::ALError& e) 
	{
    		std::cerr << "Caught exception: " << e.what() << std::endl;
  	}
}

// Return vector of [Torso COM angle, lower body COM angle] when robot is sitting
std::vector<float> BodyInfo::getSittingCOMAngles()
{
	try
	{
	        std::vector<float> sensorAngles = motion.getAngles("Body", true);
	}	
  	catch (const AL::ALError& e) 
	{
    		std::cerr << "Caught exception: " << e.what() << std::endl;
  	}
}
