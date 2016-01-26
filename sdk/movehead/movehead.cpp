/**
* Copyright (c) 2011 Aldebaran Robotics. All Rights Reserved
*
* \file movehead.cpp
* \brief Move NAO's head.
*
* A simple example showing how to move NAO's head by using ALMotionProxy.
* This example will make NAO turn its head left and right slowly.
* We use here a specialized proxy to ALMotion.
*/

#include <iostream>
#include <alerror/alerror.h>
#include <alproxies/almotionproxy.h>

int main(int argc, char* argv[]) {

  if(argc != 2)
  {
    std::cerr << "Wrong number of arguments!" << std::endl;
    std::cerr << "Usage: movehead NAO_IP" << std::endl;
    exit(2);
  }

  /** The name of the joint to be moved. */
  const AL::ALValue jointName = "HeadYaw";


  try {
    /** Create a ALMotionProxy to call the methods to move NAO's head.
    * Arguments for the constructor are:
    * - IP adress of the robot
    * - port on which NAOqi is listening, by default 9559
    */
    AL::ALMotionProxy motion(argv[1], 9559);

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
//    AL::ALValue targetAngles = AL::ALValue::array(-1.5f, 1.5f, 0.0f);
    /** Set the corresponding time lists, in seconds. */
//    AL::ALValue targetTimes = AL::ALValue::array(3.0f, 6.0f, 9.0f);
    /** Specify that the desired angles are absolute. */
 //   bool isAbsolute = true;

    /** Call the angle interpolation method. The joint will reach the
    * desired angles at the desired times.
    */
//    motion.angleInterpolation(jointName, targetAngles, targetTimes, isAbsolute);

    /** Remove the stiffness on the head. */
    stiffness = 0.0f;
    time = 1.0f;
    motion.stiffnessInterpolation(jointName, stiffness, time);


    AL::ALValue result = motion.getWalkArmsEnabled();
 //   std::cout << "Left: " << result[0] << "\nRight: " << result[1];

    //motion.getWalkArmsEnabled;
//        motion.getWalkArmsEnable();
 //   std::cout << "\nWalkArmsEnabled";

//    motion.moveTo(0.5, 0, 0);

   /*---------Harry's Edits-------*/
    AL::ALValue names = "RShoulderPitch"; //String is the limb name
    AL::ALValue names2 = "LShoulderPitch";
    AL::ALValue angles = 1.0f; //Angle is in robot unit, f
    AL::ALValue angles2 = 0.0f;
    AL::ALValue times = 1.0f;
    bool isAbsolute = true; // Tells robot the angle values are absolute
    std::cout << "\nShoulder Pitch:\n";


    for (int i = 0; i < 3; i++){
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
  catch (const AL::ALError& e) {
    std::cerr << "Caught exception: " << e.what() << std::endl;
    exit(1);
  }
  exit(0);
}
