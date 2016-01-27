/*
 * Copyright (c) 2012-2015 Aldebaran Robotics. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the COPYING file.
 */
#include <iostream>
#include <vector>
#include <alproxies/almotionproxy.h>
//#include <alproxies/alaudiodeviceproxy.h>
int main(int argc, char **argv)
{
   //Throws error is IP address not specified
 /*  if(argc != 2)
   {
     std::cerr << "Wrong number of arguments!" << std::endl;
     std::cerr << "Usage: movehead NAO_IP" << std::endl;
     exit(2);
   }*/
   argv[1] = "10.20.90.177:9559";
   AL::ALMotionProxy motion(argv[1]);


   //String of which components to get CoM
   std::string name = "Body";
   int pSpace = 0; //Robot frame, 1 = world, 0 = torso
   bool pUseSensors = false;

   std::vector<float> com = motion.getCOM(name, pSpace, pUseSensors);

   std::cout << name << " CoM:" << std::endl;
   std::cout << "x: " << com[0] << ", y: " << com[1] <<  ", z: " << com[2] << std::endl;

   //Moves arms to change CoM
   AL::ALValue names = "RShoulderPitch"; //String is the limb name
   AL::ALValue names2 = "LShoulderPitch";
   AL::ALValue angles = 1.0f; //Angle is in robot unit, f
   AL::ALValue angles2 = 0.0f;
   AL::ALValue times = 1.0f;
   bool isAbsolute = true; // Tells robot the angle values are absolute
   std::cout << "Shoulder Pitch:\n";


    //Function calls robot to move
    motion.angleInterpolation(names, angles, times, isAbsolute);
    motion.angleInterpolation(names2, angles, times, isAbsolute);
    //angles = 0.0f;

    com = motion.getCOM(name, pSpace, pUseSensors);
    std::cout << name << " CoM:" << std::endl;
    std::cout << "x: " << com[0] << ", y: " << com[1] <<  ", z: " << com[2] << std::endl;

    motion.angleInterpolation(names, angles2, times, isAbsolute);
    motion.angleInterpolation(names2, angles2, times, isAbsolute);


  //std::cout << "Hello, world" << std::endl;
  return 0;
}
