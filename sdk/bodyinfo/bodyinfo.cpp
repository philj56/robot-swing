/* bodyinfo.cpp
 * Get body angles source file
 */

#include "bodyinfo.h"
#include <alproxies/almotionproxy.h>

// Argument of point
double arg (double x, double y)
{
	double offset;
	if (x < 0)
	{
		offset = M_PI;
	}
	else
	{
		if (y > 0)
		{
			offset = 0;
		}
		else
		{
			offset = 2 * M_PI;
		}
	}

	return offset + atan(y / x);
}

// Constructor
BodyInfo::BodyInfo(boost::shared_ptr<AL::ALBroker> broker,
			 const std::string &name)
	: AL::ALModule(broker, name)
{
	// Set description, and bind each function
	setModuleDescription("Various functions to obtain relevant body information");

	functionName("getHipPitch", getName(), "Read hip pitch angles");
	BIND_METHOD(BodyInfo::getHipPitch);

	functionName("calibrateAngles", getName(), "Calibrate body angles to current position");
	BIND_METHOD(BodyInfo::calibrateAngles);

	functionName("getSittingCOMAngles", getName(), "Get COM angles from seat for sitting position");
	BIND_METHOD(BodyInfo::getSittingCOMAngles);

	functionName("printAngles", getName(), "Print joint angles relative to some initial set");
	BIND_METHOD(BodyInfo::printAngles);
	
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
		motion    = AL::ALMotionProxy(pip, pport);
		torsoMass = motion.getMass("Torso");
		lArmMass  = motion.getMass("LArm");
		rArmMass  = motion.getMass("RArm");
		headMass  = motion.getMass("Head");
		lLegMass  = motion.getMass("LLeg");
		rLegMass  = motion.getMass("RLeg");

		hipOffset = 0.085;
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
		return (sensorAngles[8] + sensorAngles[14]) / 2.0; // Should be 10 and 16 for actual robot
	}	
  	catch (const AL::ALError& e) 
	{
    		std::cerr << "Caught exception: " << e.what() << std::endl;
  	}
}

void BodyInfo::calibrateAngles()
{
	initAngles = motion.getAngles("Body", true);	
}

void BodyInfo::printAngles()
{
	std::vector<float> angles = static_cast<std::vector<float> >(motion.getAngles("Body", true));
	std::vector<std::string> names = static_cast<std::vector<std::string> >(motion.getBodyNames("Body"));

	if (initAngles.size() != angles.size())
		calibrateAngles();
	
	for (unsigned int i = 0; i < angles.size(); i++)
	{
		std::cout << std::setprecision(6)
			  << std::setw(20) << names[i]
			  << std::setw(20) << angles[i] - initAngles[i]
			  << std::endl;
	}
}

// Return vector of [Torso COM angle, lower body COM angle] when robot is sitting
std::vector<float> BodyInfo::getSittingCOMAngles()
{
	try
	{
		// Angle to transform torso frame to seat frame
		float hipAngle = getHipPitch();
		unsigned int frame = 0;		// FRAME_TORSO
		bool sensors = true;		// Use sensors
	
		std::vector<float> torsoCOM = motion.getCOM("Torso", frame, sensors);
		std::vector<float> lArmCOM  = motion.getCOM("LArm",  frame, sensors);
		std::vector<float> rArmCOM  = motion.getCOM("RArm",  frame, sensors);
		std::vector<float> headCOM  = motion.getCOM("Head",  frame, sensors);
		std::vector<float> lLegCOM  = motion.getCOM("LLeg",  frame, sensors);
		std::vector<float> rLegCOM  = motion.getCOM("RLeg",  frame, sensors);

//		std::cout << "Torso COM: " << torsoCOM[0] << ", " << torsoCOM[1] << "," << torsoCOM[2] << std::endl;

		std::vector<float> upperCOM;
		std::vector<float> lowerCOM;

		// Only care about x and z (0 & 2 index)
		for (int i = 0; i < 3; i+=2)
		{

			upperCOM.push_back(
					torsoCOM[i] * torsoMass +
					lArmCOM[i]  * lArmMass  +
					rArmCOM[i]  * rArmMass  +
					headCOM[i]  * headMass
					);

			lowerCOM.push_back(
					lLegCOM[i]  * lLegMass  +
					rLegCOM[i]  * rLegMass
					);

		}

		// TODO: Get better lower COM angles, good enough for now
		std::vector<float> COMAngles;
		COMAngles.push_back(arg (upperCOM[0], upperCOM[1] + hipOffset) + hipAngle);
		COMAngles.push_back(arg (lowerCOM[0], lowerCOM[1] + hipOffset) + hipAngle / 2.0);
		//std::cout << "Hip Angle:\t" << hipAngle << std::endl;
		//std::cout << "Seat Frame Upper: " << COMAngles[0] << std::endl;
		//std::cout << "Seat Frame Lower: " << COMAngles[1] << std::endl;

		return COMAngles;
	}
  	catch (const AL::ALError& e) 
	{
    		std::cerr << "Caught exception: " << e.what() << std::endl;
  	}
}


