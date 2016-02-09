/* main.cpp
 * smoothmove main file - compiles as a library if local,
 * or an executable that calls the SmoothMove::sayHello() function if remote.
 */

#include <iostream>
#include <cstdlib>
#include <boost/shared_ptr.hpp>
#include <qi/os.hpp>

#include "smoothmove.h"

#include <alcommon/almodule.h>
#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>
#include <alcommon/alproxy.h>
#include <alproxies/almotionproxy.h>
#include <alproxies/alrobotpostureproxy.h>

// Need to export entry point if on Windows
#ifdef _WIN32
	#define ALCALL __declspec(dllexport)
#else
	#define ALCALL
#endif

// Setup for local module
extern "C"
{
	ALCALL int _createModule(boost::shared_ptr<AL::ALBroker> broker)
	{
		// Initiate broker with main broker instance
		AL::ALBrokerManager::setInstance(broker->fBrokerManager.lock());
		AL::ALBrokerManager::getInstance()->addBroker(broker);

		// Create instance of module
		AL::ALModule::createModule<SmoothMove>(broker, "SmoothMove");
	}

	ALCALL int _closeModule()
	{
		return 0;
	}
}; /* extern "C" */

int main()
{	
  /** The name of the joint to be moved. */
  const AL::ALValue jointName = "HeadYaw";


  try {
    /** Create a ALMotionProxy to call the methods to move NAO's head.
    * Arguments for the constructor are:
    * - IP adress of the robot
    * - port on which NAOqi is listening, by default 9559
    */
    AL::ALMotionProxy motion("127.0.0.1", 9559);

    /** Make sure the head is stiff to be able to move it.
    * To do so, make the stiffness go to the maximum in one second.
    */
    /** Target stiffness. */
    AL::ALValue stiffness = 1.0f;
    /** Time (in seconds) to reach the target. */
    AL::ALValue time = 1.0f;
    /** Call the stiffness interpolation method. */
    motion.stiffnessInterpolation(jointName, stiffness, time);

    /** Set the target angle list, in radians. */
    AL::ALValue targetAngles = AL::ALValue::array(-1.5f, 1.5f, 0.0f);
    /** Set the corresponding time lists, in seconds. */
    AL::ALValue targetTimes = AL::ALValue::array(3.0f, 6.0f, 9.0f);
    /** Specify that the desired angles are absolute. */
    bool isAbsolute = true;

    /** Call the angle interpolation method. The joint will reach the
    * desired angles at the desired times.
    */
    motion.angleInterpolation(jointName, targetAngles, targetTimes, isAbsolute);

    /** Remove the stiffness on the head. */
    stiffness = 0.0f;
    time = 1.0f;
    motion.stiffnessInterpolation(jointName, stiffness, time);

  }
  catch (const AL::ALError& e) {
    std::cerr << "Caught exception: " << e.what() << std::endl;
  }
  return 0;
}

