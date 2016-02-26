/* movementtools.cpp
 * Various movement tools source file
 */

#include "movementtools.h"

// Constructor
MovementTools::MovementTools(boost::shared_ptr<AL::ALBroker> broker,
			 const std::string &name)
	: AL::ALModule(broker, name)
{
	// Set description, and bind each function
	setModuleDescription("Smooth movement testing module");

	functionName("swingForwards", getName(), "Move to forward seated position");
	BIND_METHOD(MovementTools::swingForwards);
	
	functionName("swingBackwards", getName(), "Move to backward seated position");
	BIND_METHOD(MovementTools::swingBackwards);

	// Set broker parent IP and port
	pip = broker->getParentIP();
	pport = broker->getParentPort();

	speed = 0.6;

	std::string angleNamesArray[] = { "LAnklePitch",		
				    "LAnkleRoll",		
			    	    "LElbowRoll",		
			 	    "LElbowYaw",		
				    "LHipPitch",		
				    "LHipRoll",		
				    "LHipYawPitch",	
				    "LKneePitch",		
				    "LShoulderPitch",	
				    "LShoulderRoll",	
				    "LWristYaw",		
				    "RAnklePitch",		
				    "RAnkleRoll",		
				    "RElbowRoll",		
				    "RElbowYaw",		
				    "RHipPitch",		
				    "RHipRoll",		
				    "RHipYawPitch",	
				    "RKneePitch",	
				    "RShoulderPitch",	
				    "RShoulderRoll",	
				    "RWristYaw"};	

	float sitForwardAnglesArray[] = { 0.820648f,
				    0.023052f,
				   -0.0475121f,
				    0.118076f,
				   -0.601286f,
				   -0.0337059f,
				   -0.02757f,
				   -0.090548f,
				    0.408002f,
				    0.0367742f,
				   -1.7488f,
				    0.921976f,
				   -0.0168321f,
				    0.0583339f,
				    0.20398f,
			 	   -0.615176f,
				    0.0368581f,
				   -0.02757f,
				   -0.0923279f,
				    0.420358f,
				    0.0152981f,
				    1.38516f};

	float sitBackwardAnglesArray[] = { 0.922581f,
				   -0.00609398f,
				   -0.049046f,
				   -0.32525f,
				   -0.41107f,
				   -0.0168321f,
				   -0.032172f,
				   -0.0923279f,
				    0.765424f,
				    0.1733f,
				   -1.73039f,
				    0.926578f,
				    0.00157595f,
				    0.131966f,
				    0.645772f,
				   -0.428028f,
				    0.021518f,
				   -0.032172f,
				   -0.0843279f,
				    0.825334f,
				    0.078192f,
				    0.44942f};
	
	angleNames = AL::ALValue(std::vector<std::string> (angleNamesArray, angleNamesArray + 22));
	sitForwardAngles = AL::ALValue(std::vector<float> (sitForwardAnglesArray, sitForwardAnglesArray + 22));
	sitBackwardAngles = AL::ALValue(std::vector<float> (sitBackwardAnglesArray, sitBackwardAnglesArray + 22));
}

// Destructor
MovementTools::~MovementTools()
{
}

// init() - called as soon as the module is constructed
void MovementTools::init()
{
	motion = AL::ALMotionProxy(pip, pport);
	posture = AL::ALRobotPostureProxy(pip, pport);
}

void MovementTools::swingForwards()
{
	motion.setStiffnesses("Body", 1.0f);
	motion.setAngles(angleNames, sitForwardAngles, speed);	
	qi::os::msleep(700);
	//motion.setStiffnesses("Body", 0.0f);
}

void MovementTools::swingBackwards()
{
	motion.setStiffnesses("Body", 1.0f);
	motion.setAngles(angleNames, sitBackwardAngles, speed);	
	qi::os::msleep(700);
	//motion.setStiffnesses("Body", 0.0f);
}
