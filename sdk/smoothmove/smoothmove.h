/* smoothmove.h
 * Generic module header file
 */

#ifndef SMOOTHMOVE_H
#define SMOOTHMOVE_H

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

// SmoothMove - generic NAOqi module
class SmoothMove : public AL::ALModule
{
public:
	// Constructor
	SmoothMove(boost::shared_ptr<AL::ALBroker> broker,
		 const std::string &name);

	// Destructor
	virtual ~SmoothMove();

	// init() - called just after the constructor
	virtual void init();

	// Testing movement
	void moveTest();
	
	// Do a dance
	void harryDance();

	// SDK example move head
	void moveHead();

private:
	// Broker Parent IP and port
	std::string pip;
	int pport;
};

#endif /* SMOOTHMOVE_H */
