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
		float hipAngle = getHipPitch();
		unsigned int frame = 0;		// FRAME_TORSO
		bool sensors = true;		// Use sensors
	
		std::vector<float> torsoCOM = motion.getCOM("Torso", frame, sensors);
		std::vector<float> lArmCOM  = motion.getCOM("LArm",  frame, sensors);
		std::vector<float> rArmCOM  = motion.getCOM("RArm",  frame, sensors);
		std::vector<float> headCOM  = motion.getCOM("Head",  frame, sensors);
		std::vector<float> lLegCOM  = motion.getCOM("LLeg",  frame, sensors);
		std::vector<float> rLegCOM  = motion.getCOM("RLeg",  frame, sensors);

		std::vector<float> upperCOM;
		std::vector<float> lowerCOM;

		for (int i = 0; i < 3; i++)
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

		// Transform COMs from torso frame to seat frame
		std::vector<float> upperSeatCOM;
		upperSeatCOM.push_back(upperCOM[0] * sin(hipAngle) + (upperCOM[3] + hipOffset) * cos(hipAngle));
		upperSeatCOM.push_back(upperCOM[0] * cos(hipAngle) - (upperCOM[3] + hipOffset) * sin(hipAngle));
		
		std::vector<float> lowerSeatCOM;
		lowerSeatCOM.push_back(lowerCOM[0] * sin(hipAngle) + (lowerCOM[3] - hipOffset) * cos(hipAngle));
		lowerSeatCOM.push_back(lowerCOM[0] * cos(hipAngle) - (lowerCOM[3] - hipOffset) * sin(hipAngle));

		// Convert COMs to angles
		std::vector<float> COMAngles;
		COMAngles.push_back(-atan(upperSeatCOM[0] / upperSeatCOM[3]));	// Upper body angle
		COMAngles.push_back( atan(lowerSeatCOM[0] / lowerSeatCOM[3]));	// Lower body angle

		return COMAngles;
	}
  	catch (const AL::ALError& e) 
	{
    		std::cerr << "Caught exception: " << e.what() << std::endl;
  	}
}