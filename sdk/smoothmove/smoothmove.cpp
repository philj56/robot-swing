/* smoothmove.cpp
 * Smooth motion source file
 */

#include "smoothmove.h"
#include <alproxies/almotionproxy.h>
#include <alproxies/alrobotpostureproxy.h>

// Constructor
SmoothMove::SmoothMove(boost::shared_ptr<AL::ALBroker> broker,
			 const std::string &name)
	: AL::ALModule(broker, name)
{
	// Set description, and bind each function
	setModuleDescription("Smooth movement testing module");

	functionName("harryDance", getName(), "Do a little dance");
	BIND_METHOD(SmoothMove::harryDance);
	
	functionName("moveTest", getName(), "Testing movement");
	BIND_METHOD(SmoothMove::moveTest);
	
	functionName("moveHead", getName(), "SDK movehead example");
	BIND_METHOD(SmoothMove::moveHead);

	// Set broker parent IP and port
	pip = broker->getParentIP();
	pport = broker->getParentPort();

	std::cout << "ip:   " << pip << std::endl;
	std::cout << "port: " << pport << std::endl;
}

// Destructor
SmoothMove::~SmoothMove()
{
}

// init() - called as soon as the module is constructed
void SmoothMove::init()
{
}

void SmoothMove::moveTest()
{
	std::cout<<"MoveTest "<<pip<<":"<<pport<<std::endl;
	try
	{
		std::cout<<"MotionProxy "<<pip<<":"<<pport<<std::endl;
		// Get brokers, and go to initial posture
		AL::ALMotionProxy motion(pip, pport);
		std::cout<<"Done"<<std::endl;
		AL::ALRobotPostureProxy posture(pip, pport);
	
		posture.goToPosture("StandInit", 0.5f);
	
		int space       =  2; // FRAME_ROBOT
		int axisMask    = 63; // control all the effector's axes
		bool isAbsolute = false;
	
		// Lower the Torso and move to the side
		std::string torso = "Torso";
		std::string lArm  = "LArm";
		std::string rArm  = "RArm";
		std::vector<float> position(6, 0.0f);
	
		position[0] = 0.0f;
		position[1] = 0.0f;
		position[2] = 0.3f;
		position[3] = 0.0f;
		position[4] = 0.0f;
		position[5] = 0.0f;
		
		// Amplitude of oscillation
		float amplitude = 0.05f;
	
		// Oscillation time in seconds
		float period 	= 4.0f;
	
		// Update time in milliseconds
		unsigned int update	= 50;
	
		// Max speed of robot movement
		float speed = 1.0f;
	
		// Go to initial position
		motion.setPosition(torso, space, position, speed, axisMask);
		qi::os::sleep(1.0f);
	
		for (float t = 0; t < period * 2.0; t += 0.001 * update)
		{
			speed = 0.5 * (1.0 + cos(2.0 * M_PI * t / period));
			position[0] = amplitude * (sin(2.0 * M_PI * t / period) > 0 ? 1 : -1);
			motion.setPosition(torso, space, position, speed, axisMask);
			qi::os::msleep(update);
		}
	}	
  	catch (const AL::ALError& e) 
	{
    		std::cerr << "Caught exception: " << e.what() << std::endl;
  	}
}

void SmoothMove::harryDance()
{
  	/** The name of the joint to be moved. */
  	const AL::ALValue jointName = "HeadYaw";


  	try {
    		/** Create a ALMotionProxy to call the methods to move NAO's head.
    		* Arguments for the constructor are:
    		* - IP adress of the robot
    		* - port on which NAOqi is listening, by default 9559
    		*/
    		AL::ALMotionProxy motion(pip, pport);

    		/** Make sure the head is stiff to be able to move it.
    		* To do so, make the stiffness go to the maximum in one second.
    		*/
    		/** Target stiffness. */
    		AL::ALValue stiffness = 1.0f;
    		/** Time (in seconds) to reach the target. */
    		AL::ALValue time = 1.0f;

    		/** Call the stiffness interpolation method. */
    		motion.stiffnessInterpolation(jointName, stiffness, time);

    		/** Remove the stiffness on the head. */
    		stiffness = 0.0f;
    		time = 1.0f;
    		motion.stiffnessInterpolation(jointName, stiffness, time);


    		AL::ALValue result = motion.getWalkArmsEnabled();

   		/*---------Harry's Edits-------*/
   		AL::ALValue names = "RShoulderPitch"; //String is the limb name
    		AL::ALValue names2 = "LShoulderPitch";
    		AL::ALValue angles = 1.0f; //Angle is in robot unit, f
    		AL::ALValue angles2 = 0.0f;
    		AL::ALValue times = 1.0f;
    		bool isAbsolute = true; // Tells robot the angle values are absolute
    		std::cout << "\nShoulder Pitch:\n";


    		for (int i = 0; i < 3; i++)
		{
        		//Function calls robot to move
        		motion.angleInterpolation(names, angles, times, isAbsolute);
        		motion.angleInterpolation(names2, angles2, times, isAbsolute);
        		//angles = 0.0f;
        		motion.angleInterpolation(names, angles2, times, isAbsolute);
        		motion.angleInterpolation(names2, angles, times, isAbsolute);
    		}

  		std::cout << "Shoulder Roll";
    		//Creates arrays of values to send to robot.
    		AL::ALValue angle_list, time_list;
    		angle_list = AL::ALValue::array(1.0f);
    		time_list = AL::ALValue::array(1.0f);

    		angle_list.clear();
    		time_list.clear();

    		angle_list.arraySetSize(2);
    		//Array lists angles to move through
    		angle_list[0] = AL::ALValue::array(-0.5f, 0.5f); //From -0.5 to 0.5
    		angle_list[1] = AL::ALValue::array(0.5f, -0.5f);

    		time_list.arraySetSize(2);
    		//Time arrays - unsure of what this does...
    		time_list[0] = AL::ALValue::array(1.0f, 2.0f);
    		time_list[1] = AL::ALValue::array(1.0f, 2.0f);

    		motion.angleInterpolation(names, angles, times, isAbsolute);
    		//Array tells robot what limbs to move
    		//All array[0] (angle, time) are assigned to the first limb
    		names = AL::ALValue::array("RShoulderRoll", "LShoulderRoll");

    		motion.angleInterpolation(names, angle_list, time_list, isAbsolute);
    		/*-------------------*/
  	}
  	catch (const AL::ALError& e) 
	{
    		std::cerr << "Caught exception: " << e.what() << std::endl;
  	}
}

void SmoothMove::moveHead()
{
  std::cout<<"MoveHead "<<pip<<":"<<pport<<std::endl;
  /** The name of the joint to be moved. */
  const AL::ALValue jointName = "HeadYaw";


  try {
    /** Create a ALMotionProxy to call the methods to move NAO's head.
    * Arguments for the constructor are:
    * - IP adress of the robot
    * - port on which NAOqi is listening, by default 9559
    */
    AL::ALMotionProxy motion(pip, pport);

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
}
