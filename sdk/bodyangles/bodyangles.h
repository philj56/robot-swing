/* bodyangles.h
 * Generic module header file
 */

#ifndef BODYANGLES_H
#define BODYANGLES_H

#include <iostream>
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

// BodyAngles
class BodyAngles : public AL::ALModule
{
public:
	// Constructor
	BodyAngles(boost::shared_ptr<AL::ALBroker> broker,
		 const std::string &name);

	// Destructor
	virtual ~BodyAngles();

	// init() - called just after the constructor
	virtual void init();

	// Get angles
	void getAngles();

private:
	// Broker Parent IP and port
	std::string pip;
	int pport;
};

#endif /* BODYANGLES_H */
