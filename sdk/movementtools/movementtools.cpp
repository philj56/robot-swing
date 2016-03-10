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

	functionName("goLimp", getName(), "Set all joints to 0 stiffness");
	BIND_METHOD(MovementTools::goLimp);
	
	functionName("goStiff", getName(), "Set all joints to full stiffness");
	BIND_METHOD(MovementTools::goStiff);
	
	functionName("swingForwards", getName(), "Move to forward seated position");
	BIND_METHOD(MovementTools::swingForwards);
	
	functionName("swingBackwards", getName(), "Move to backward seated position");
	BIND_METHOD(MovementTools::swingBackwards);

	functionName("setSpeed", getName(), "Set movement speed");
	addParam("newSpeed", "The new speed");
	BIND_METHOD(MovementTools::setSpeed);
	
	// Set broker parent IP and port
	pip = broker->getParentIP();
	pport = broker->getParentPort();

	speed = 0.6;

	std::string angleNamesArray[] = { "LAnklePitch",		
				    "LAnkleRoll",		
	//		    	    "LElbowRoll",		
	//		 	    "LElbowYaw",		
	//			    "LHipPitch",		
	//			    "LHipRoll",		
	//			    "LHipYawPitch",	
				    "LKneePitch",		
	//			    "LShoulderPitch",	
	//			    "LShoulderRoll",	
	//			    "LWristYaw",		
				    "RAnklePitch",		
				    "RAnkleRoll",		
	//			    "RElbowRoll",		
	//			    "RElbowYaw",		
	//			    "RHipPitch",		
	//			    "RHipRoll",		
	//			    "RHipYawPitch",	
				    "RKneePitch",	
	//			    "RShoulderPitch",	
	//			    "RShoulderRoll",	
	//			    "RWristYaw"
				    };	

	float sitForwardAnglesArray[] = { 0.921892f,
				    0.04146f,
	//			   -1.54462f,
	//			   -0.154976f,
	//			   -1.0845f,
	//			   -0.0337059f,
	//			   -0.0183661f,
				    1.54163f,
	//			    0.05825f,
	//			    0.800706f,
	//			   -1.39905f,
				    0.921976f,
				   -0.0168321f,
	//			    1.53864f,
	//			    0.22699f,
	//			   -1.07231f,
	//			    0.0506639f,
	//			   -0.0183661f,
				    1.52637f,
	//			    0.219404f,
	//			   -0.83147f,
	//			    1.45419f
				    };

	float sitBackwardAnglesArray[] = { 0.820648f,
				    0.023052f,
	//			   -0.0475121f,
	//			    0.118076f,
	//			   -0.601286f,
	//			   -0.0337059f,
	//			   -0.02757f,
				   -0.090548f,
	//			    0.408002f,
	//			    0.0367742f,
	//			   -1.7488f,
				    0.921976f,
				   -0.0168321f,
	//			    0.0583339f,
	//			    0.20398f,
	//		 	   -0.615176f,
	//			    0.0368581f,
	//			   -0.02757f,
				   -0.0923279f,
	//			    0.420358f,
	//			    0.0152981f,
	//			    1.38516f
				    };
	
	angleNames = AL::ALValue(std::vector<std::string> (angleNamesArray, angleNamesArray + 17));
	sitForwardAngles = AL::ALValue(std::vector<float> (sitForwardAnglesArray, sitForwardAnglesArray + 17));
	sitBackwardAngles = AL::ALValue(std::vector<float> (sitBackwardAnglesArray, sitBackwardAnglesArray + 17));
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

void MovementTools::setSpeed(const float &newSpeed)
{
	speed = newSpeed;
}

void MovementTools::goLimp()
{
	motion.setStiffnesses("Body", 0.0f);
}

void MovementTools::goStiff()
{
	motion.setStiffnesses("Body", 1.0f);
}

void MovementTools::swingForwards()
{
	motion.setStiffnesses("Body", 1.0f);
	motion.setStiffnesses("RArm", 0.0f);
	motion.setAngles(angleNames, sitForwardAngles, speed);	
	qi::os::msleep(700);
	motion.setStiffnesses("Body", 0.2f);
	motion.setStiffnesses("RArm", 0.0f);
}

void MovementTools::swingBackwards()
{
	motion.setStiffnesses("Body", 1.0f);
	motion.setStiffnesses("RArm", 0.0f);
	motion.setAngles(angleNames, sitBackwardAngles, speed);	
	qi::os::msleep(700);
	motion.setStiffnesses("Body", 0.2f);
	motion.setStiffnesses("RArm", 0.0f);
}
