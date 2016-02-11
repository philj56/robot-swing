/* bodyangles.cpp
 * Get body angles source file
 */

#include "bodyangles.h"
#include <alproxies/almotionproxy.h>

// Constructor
BodyAngles::BodyAngles(boost::shared_ptr<AL::ALBroker> broker,
			 const std::string &name)
	: AL::ALModule(broker, name)
{
	// Set description, and bind each function
	setModuleDescription("Read angles from body");

	functionName("getAngles", getName(), "Read body angles");
	BIND_METHOD(BodyAngles::getAngles);

	// Set broker parent IP and port
	pip = broker->getParentIP();
	pport = broker->getParentPort();
}

// Destructor
BodyAngles::~BodyAngles()
{
}

// init() - called as soon as the module is constructed
void BodyAngles::init()
{
}

void BodyAngles::getAngles()
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
