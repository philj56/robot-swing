/* bodyinfo.h
 * various body information header file
 */

#ifndef BODYINFO_H
#define BODYINFO_H

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <alcommon/almodule.h>
#include <alcommon/albroker.h>
#include <alcommon/alproxy.h>
#include <alproxies/almotionproxy.h>
#include <alproxies/alrobotpostureproxy.h>

#include <qi/os.hpp>

// Forward declaration of ALBroker
namespace AL
{
	class ALBroker;
}

// BodyInfo
class BodyInfo : public AL::ALModule
{
public:
	// Constructor
	BodyInfo(boost::shared_ptr<AL::ALBroker> broker,
		 const std::string &name);

	// Destructor
	virtual ~BodyInfo();

	// init() - called just after the constructor
	virtual void init();

	// Get angles
	float getHipPitch();

	// Get angles of upper and lower COM angles
	std::vector<float> getSittingCOMAngles();

	// Calibrate upper and lower COM angles
	void calibrateAngles();
	
	void printAngles();

private:
	// Broker Parent IP and port
	std::string pip;
	int pport;
	
	AL::ALMotionProxy motion;

	float torsoMass;
	float lArmMass;
	float rArmMass;
	float headMass;
	float lLegMass;
	float rLegMass;
	
	float hipOffset;

	std::vector<float> initAngles;
};

#endif /* BODYINFO_H */
