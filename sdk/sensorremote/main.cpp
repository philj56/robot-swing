/*
 * Copyright (c) 2012-2015 Aldebaran Robotics. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the COPYING file.
 */
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>

#include <alcommon/alproxy.h>
#include <alerror/alerror.h>
#include <alproxies/almemoryproxy.h>
#include <alproxies/alproxies.h>
#include <alproxies/almotionproxy.h>


int main(int argc, char **argv)
{
    std::cout <<"\nhere" ;
   //Throws error is IP address not specified
   if(argc != 2)
   {
     std::cerr << "Wrong number of arguments!" << std::endl;
     std::cerr << "Usage: movehead NAO_IP" << std::endl;
     exit(2);
   }
   //argv[1] = "10.20.90.177:9559";
   //std::cout <<"\nhere" << argv[0] <<  " " << argv[1] << std::endl;

   try{
   //Attempts to connect to the robot on the local network
   AL::ALProxy proxy("ALMemory", "nao.local", 9559);
   //std::cout <<"\nWe got here" << argv[0] <<  " " << argv[1] << std::endl;

   AL::ALValue accel_val, gyro_val;
   //Makes them into an AL Array to get data
   accel_val.arraySetSize(3);
   gyro_val.arraySetSize(3);

   //List of of values to get
   std::string gyro_list[3] = {"Device/SubDeviceList/InertialSensor/GyrX/Sensor/Value",
                            "Device/SubDeviceList/InertialSensor/GyroscopeY/Sensor/Value",
                            "Device/SubDeviceList/InertialSensor/GyroscopeZ/Sensor/Value"};
   std::string accel_list[3] =  {"Device/SubDeviceList/InertialSensor/AccelerometerX/Sensor/Value",
                                "Device/SubDeviceList/InertialSensor/AccelerometerY/Sensor/Value",
                                "Device/SubDeviceList/InertialSensor/AccelerometerZ/Sensor/Value"};

   std::ofstream output;
   output.open ("output.txt");

   if (output.is_open()){
    output << "Stamp\tGyroX\tY\tZ\tAccelX\tY\tZ";
    std::cout << "\nBefore loop\n\n";

    time_t t = time(0);

    for(int i = 0; i < 300; i++){
        for(int j = 0; j < 3; j++){
            t = time(0);
            //Sends a call for the ALMemory method 'getData' along with the gyroscope
            gyro_val[j] = proxy.genericCall("getData", gyro_list[j]);
            accel_val[j] = proxy.genericCall("getData", accel_list[j]);
            }
        output << t << "\t" << gyro_val[0] << "\t" << gyro_val[1] << "\t" << gyro_val[2] << "\t" << accel_val[0] << "\t" << accel_val[1] << "\t" << accel_val[2] << std::endl;
        }
    output.close();
   }
   else {
       std::cout << "\nFile not open";
   }
   std::cout << "\nAfter Loop";
   }
   catch(const AL::ALError& e)
   {
       std::cerr << "Exception: " << e.what() << std::endl;
       exit(1);
   }
    return 0;
}
