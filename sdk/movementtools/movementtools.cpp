/* movementtools.cpp
 * Various movement tools source file
 */

#include "movementtools.h"


// Clamp a number to a range
inline float clip(float n, float lower, float upper) 
{
	  return std::max(lower, std::min(n, upper));
}

// Linearly interpolate between two vectors of floats, truncating to shortest vector
std::vector<float> vectorLerp(const std::vector<float> &vector1, const std::vector<float> &vector2, float t)
{
	t = clip(t, 0, 1);
	std::vector<float> result;
	size_t size = vector1.size() < vector2.size() ? vector1.size() : vector2.size();
	for (size_t i = 0; i < size; i++)
	{
		result.push_back(t * vector1[i] + (1 - t) * vector2[i]);
	}
	return result;
}

// Constructor
MovementTools::MovementTools(boost::shared_ptr<AL::ALBroker> broker,
			 const std::string &name)
	: AL::ALModule(broker, name)
{
	qiLogInfo("Human.movement") << "Loading human stuff" << std::endl;
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

	functionName("lerpSwing", getName(), "Move to a position between backwards and forwards");
	addParam("t", "The position to move to (0 = backwards, 1 = forwards)");
	BIND_METHOD(MovementTools::swingBackwards);
	
	functionName("humanSwing", getName(), "Replicate a human's swinging motion");
	addParam("theta", "The angle of the swing (0 - 1)");
	addParam("forwards", "Whether the swing is moving forwards");
	BIND_METHOD(MovementTools::swingBackwards);
	
	functionName("setSpeed", getName(), "Set movement speed");
	addParam("newSpeed", "The new speed");
	BIND_METHOD(MovementTools::setSpeed);
	
	// Set broker parent IP and port
	pip = broker->getParentIP();
	pport = broker->getParentPort();

	speed = 0.6;

	std::string humanAngleNamesArray[] = { "LHipPitch",
					 "RHipPitch",
					 "LKneePitch",
					 "RKneePitch"
					 };


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

	static const float sitForwardAnglesArray[] = { 0.921892f,
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

	static const float sitBackwardAnglesArray[] = { 0.820648f,
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
	
	humanAngleNames = AL::ALValue(std::vector<std::string> (humanAngleNamesArray, humanAngleNamesArray + sizeof(humanAngleNamesArray) / sizeof(humanAngleNamesArray[0])));
	angleNames = AL::ALValue(std::vector<std::string> (angleNamesArray, angleNamesArray + sizeof(angleNamesArray) / sizeof(angleNamesArray[0])));
	sitForwardAngles = AL::ALValue(std::vector<float> (sitForwardAnglesArray, sitForwardAnglesArray + sizeof(sitForwardAnglesArray) / sizeof(sitForwardAnglesArray[0])));
	sitBackwardAngles = AL::ALValue(std::vector<float> (sitBackwardAnglesArray, sitBackwardAnglesArray + sizeof(sitBackwardAnglesArray) / sizeof(sitBackwardAnglesArray[0])));
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

void MovementTools::lerpSwing(const float &t)
{
	std::vector <float> angles = vectorLerp(sitBackwardAngles, sitForwardAngles, t);
	motion.setStiffnesses("Body", 1.0f);
	motion.setStiffnesses("RArm", 0.0f);
	motion.setAngles(angleNames, angles, speed);	
	qi::os::msleep(700);
	motion.setStiffnesses("Body", 0.2f);
	motion.setStiffnesses("RArm", 0.0f);
}

void MovementTools::humanSwing(const float &theta, const bool &forwards)
{
	std::vector<float> angles = humanPosition(theta, forwards);
	motion.setStiffnesses("Body", 1.0f);
	motion.setStiffnesses("RArm", 0.0f);
	motion.setAngles(humanAngleNames, angles, speed);
	qi::os::msleep(100);
}

std::vector<float> MovementTools::humanPosition(const float &theta, const bool &forwards)
{
	std::vector<float> result;
	float t = clip (theta, 0, 1);
	float body;
	float legs;
	const static float maxBody =  0.48f;
	const static float minBody = -1.53f;
	const static float maxLegs =  1.52f;
	const static float minLegs = -0.09f;
	if (forwards)
	{
		body = t * (t * (t * (t * (-3.58786) + 7.39683) - 6.0916) + 1.65163) - 0.0301169;
		legs = t * (t * (t * (t * (10.7608) - 19.1384) + 14.255) - 5.12733) + 0.640052;
	}
	else
	{
		body = t * (t * (t * (t * (6.03095) - 12.6726) + 9.93016) - 3.90386) - 0.0387037;
		legs = t * (t * (t * (t * (-18.9054) + 40.952) - 32.3375) + 11.0356) + 0.641814;
	}

	body = clip(body, minBody, maxBody);
	legs = clip(legs, minLegs, maxLegs);

	result.push_back(body);
	result.push_back(body);
	result.push_back(legs);
	result.push_back(legs);

	qiLogInfo("Human.Movement") << "Body Angle: " << body << "Leg Angle: " << legs << std::endl;

	return result;
}
