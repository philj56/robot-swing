/* movementtools.h
 * Generic module header file
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

#include <qi/os.hpp>

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

#endif /* MOVEMENTTOOLS_H */
