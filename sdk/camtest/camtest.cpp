/* camtest.cpp
 * Smooth motion source file
 */

#include "camtest.h"
#include <alproxies/almotionproxy.h>
#include <alproxies/alrobotpostureproxy.h>

// Constructor
CamTest::CamTest(boost::shared_ptr<AL::ALBroker> broker,
			 const std::string &name)
	: AL::ALModule(broker, name)
{
	// Set description, and bind each function
	setModuleDescription("Camera testing module");

	functionName("SetRes", getName(), "Set the photo capture resolution");
	addParam("res", "The resolution to set");
	BIND_METHOD(CamTest::SetRes);

	functionName("GetRes", getName(), "Get the photo capture resolution");
	BIND_METHOD(CamTest::GetRes);
	
	// Set broker parent IP and port
	pip = broker->getParentIP();
	pport = broker->getParentPort();

	std::cout << "ip:   " << pip << std::endl;
	std::cout << "port: " << pport << std::endl;
}

// Destructor
CamTest::~CamTest()
{
}

// init() - called as soon as the module is constructed
void CamTest::init()
{
}

void CamTest::SetRes(const int &res)
{
	try
	{
		// Get brokers, and go to initial posture
		AL::ALPhotoCaptureProxy photo(pip, pport);
		photo.setResolution(res);
	}	
  	catch (const AL::ALError& e) 
	{
    		std::cerr << "Caught exception: " << e.what() << std::endl;
  	}
}

int CamTest::GetRes()
{
	try
	{
		// Get brokers, and go to initial posture
		AL::ALPhotoCaptureProxy photo(pip, pport);
		return photo.getResolution();
	}	
  	catch (const AL::ALError& e) 
	{
    		std::cerr << "Caught exception: " << e.what() << std::endl;
  	}
}

