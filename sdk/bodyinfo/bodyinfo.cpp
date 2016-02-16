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
	setModuleDescription("Read angles from body");

	functionName("getAngles", getName(), "Read body angles");
	BIND_METHOD(BodyInfo::getAngles);

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
}

void BodyInfo::getAngles()
{
	try
	{
		AL::ALMotionProxy motion(pip, pport);


	  	std::vector<float> commandAngles = motion.getAngles("Body", false);
    		std::cout << "Command angles: " << std::endl << commandAngles << std::endl;
	        std::vector<float> sensorAngles = motion.getAngles("Body", true);
		std::cout << "Sensor angles: " << std::endl << sensorAngles << std::endl;
	}	
  	catch (const AL::ALError& e) 
	{
    		std::cerr << "Caught exception: " << e.what() << std::endl;
  	}
}
