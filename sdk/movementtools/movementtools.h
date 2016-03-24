/* movementtools.h
 * Various movement tools header file
 */

#ifndef MOVEMENTTOOLS_H
#define MOVEMENTTOOLS_H

#include <iostream>
#include <cstdlib>
#include <alcommon/almodule.h>
#include <alcommon/albroker.h>
#include <alcommon/alproxy.h>
#include <alproxies/almotionproxy.h>
#include <alproxies/alrobotpostureproxy.h>
#include <alvalue/alvalue.h>

#include <qi/os.hpp>
#include <qi/log.hpp>

// Forward declaration of ALBroker
namespace AL
{
	class ALBroker;
}

// MovementTools - generic NAOqi module
class MovementTools : public AL::ALModule
{
public:
	// Constructor
	MovementTools(boost::shared_ptr<AL::ALBroker> broker,
		 const std::string &name);

	// Destructor
	virtual ~MovementTools();

	// init() - called just after the constructor
	virtual void init();

	void goLimp();
	void goStiff();

	// Seated movement
	void swingForwards();
	void swingBackwards();

	// Move to part way between forwards and backwards
	void lerpSwing(const float &t);

	// Human-style movement
	void humanSwing(const float &theta, const bool &forwards);
	std::vector<float> humanPosition(const float &t, const bool &forwards);

	void setSpeed(const float &newSpeed);
	//void setSpeed(const AL::ALValue &newSpeed);

private:
	// Broker Parent IP and port
	std::string pip;
	int pport;

	// Speed of movement
	float speed;

	// Proxies to relevant modules
	AL::ALMotionProxy motion;
	AL::ALRobotPostureProxy posture;

	// Arrays of names and angles for movement
	AL::ALValue humanAngleNames; 
	AL::ALValue angleNames; 
	AL::ALValue sitForwardAngles; 
	AL::ALValue sitBackwardAngles; 
};

#endif /* MOVEMENTTOOLS_H */
