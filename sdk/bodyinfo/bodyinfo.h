/* bodyinfo.h
 * Generic module header file
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

	std::vector<float> getSittingCOMAngles();

	std::vector<float> getInitAngles();
	
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
