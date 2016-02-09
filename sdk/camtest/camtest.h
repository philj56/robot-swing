/* camtest.h
 * Generic module header file
 */

#ifndef CAMTEST_H
#define CAMTEST_H

#include <iostream>
#include <cstdlib>
#include <alcommon/almodule.h>
#include <alcommon/albroker.h>
#include <alcommon/alproxy.h>
#include <alproxies/alphotocaptureproxy.h>

#include <qi/os.hpp>

// Forward declaration of ALBroker
namespace AL
{
	class ALBroker;
}

// CamTest - generic NAOqi module
class CamTest : public AL::ALModule
{
public:
	// Constructor
	CamTest(boost::shared_ptr<AL::ALBroker> broker,
		 const std::string &name);

	// Destructor
	virtual ~CamTest();

	// init() - called just after the constructor
	virtual void init();

	// Testing camera resolution
	void SetRes(const int &res);
	int GetRes();

	// Testing camera resolution
	void SetRate(const int &rate);
	int GetRate();
	
private:
	// Broker Parent IP and port
	std::string pip;
	int pport;
};

#endif /* CAMTEST_H */
