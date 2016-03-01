/* main.cpp
 * landmarkswing main file - use landmark detection to detect changes in swinging direction, and move accordingly 
 */

#include "createmodule.h"
#include "average.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <getopt.h>
#include <boost/shared_ptr.hpp>
#include <qi/os.hpp>
#include <qi/path.hpp>
#include <qi/log.hpp>

#include <ctime>
#include <sys/time.h>
#include <fstream>


#include <alcommon/almodule.h>
#include <alcommon/alproxy.h>
#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>
#include <alproxies/almotionproxy.h>
#include <alproxies/almemoryproxy.h>
#include <alproxies/alrobotpostureproxy.h>

int vectorOrder (std::vector<float> vec);

AL::ALMotionProxy motproxy;

double timediff(double t1, double t2){

	return t1-t2;

}

bool grad(double v1, double v2, double timeDiff){
	double lowlimit = 0.01;
	double highlimit = 1.0;
	double diff;
	if(timeDiff>0.){
		diff = abs((v1 - v2)/timeDiff);
	}
	else{
		diff = abs((v1-v2)/0.01);
	}
	if(diff == 0){
		return false;
	}
	else if (diff > lowlimit && diff < highlimit){
		return true;
	}
	else return false;
}


// Wrong number of arguments
void argErr(void)
{
	// Standard usage message
	std::string usage = "Usage: gyroswing [--pip robot_ip] [--pport port]";
	std::cerr << usage << std::endl;
	exit(2);
}

int main(int argc, char* argv[])
{
	// Whether to print verbose output - default off
	bool verb = false;

	// Set broker name, ip and port, finding first available port from 54000
	const std::string brokerName = "GyroswingBroker";
	int brokerPort = qi::os::findAvailablePort(54000);
	const std::string brokerIp = "0.0.0.0";
	
	// Default parent port and ip
	int pport = 9559;
	std::string pip = "127.0.0.1";

	// Get any arguments
	while (true)
	{
		static int index = 0;

		// Struct of options
		// Columns are:
		// 	{Option name,	Option required?,	flag(not needed here),	return value/character}
		static const struct option longopts[] =
		{
			{"pip", 	1, 	0, 	'i'},
			{"pport",	1,	0,	'p'},
			{"verb",	0,	0,	'v'},
			{"help",	0,	0,	'h'},
			{0,		0,	0,	 0 }
		};

		// Get next option, and check return value
		switch(index = getopt_long(argc, argv, "i:p:t:vh", longopts, &index))
		{
			// Print usage and quit
			case 'h':
				argErr();
				break;
			// Set parent IP
			case 'i':
				if (optarg)
					pip = std::string(optarg);
				else
					argErr();
				break;
			// Set parent port
			case 'p':
				if (optarg)
					pport = atoi(optarg);
				else
					argErr();
				break;
			case 'v':
				verb = true;
				break;
		}
		if (index == -1)
			break;
	}

	// Need this for SOAP serialisation of floats to work
	setlocale(LC_NUMERIC, "C");

	// Create a broker
	if(verb)
		std::cout << "Creating broker..." << std::endl;
	boost::shared_ptr<AL::ALBroker> broker;
	try
	{
		broker = AL::ALBroker::createBroker(
				brokerName,
				brokerIp,
				brokerPort,
				pip,
				pport,
				0);
	}
	// Throw error and quit if a broker could not be created
	catch(...)
	{
		std::cerr << "Failed to connect broker to: "
			  << pip
			  << ":"
			  << pport
			  << std::endl;
		AL::ALBrokerManager::getInstance()->killAllBroker();
		AL::ALBrokerManager::kill();

		return 1;
	}

    try{

	   // Add the broker to NAOqi
	   AL::ALBrokerManager::setInstance(broker->fBrokerManager.lock());
	   AL::ALBrokerManager::getInstance()->addBroker(broker);

	
    //----------------------------------
       AL::ALValue angleNames; 
	   AL::ALValue sitForwardAngles; 
	   AL::ALValue sitBackwardAngles; 
       
       float speed = 0.6;

	   std::string angleNamesArray[] = { "LAnklePitch",		
				    "LAnkleRoll",		
			    	 "LElbowRoll",		
			 	    "LElbowYaw",		
				    "LHipPitch",		
				    "LHipRoll",		
				    "LHipYawPitch",	
				    "LKneePitch",		
				    "LShoulderPitch",	
				    "LShoulderRoll",	
				    "LWristYaw",		
				    "RAnklePitch",		
				    "RAnkleRoll",		
				    "RElbowRoll",		
				    "RElbowYaw",		
				    "RHipPitch",		
				    "RHipRoll",		
				    "RHipYawPitch",	
				    "RKneePitch",	
				    "RShoulderPitch",	
				    "RShoulderRoll",	
				    "RWristYaw"};	

	   float sitForwardAnglesArray[] = { 0.922581f,
				    0.00310993f,
				   -0.90962f,
				   -0.31758f,
				   -0.822182f,
				   -0.0137641f,
				   -0.0337059f,
				    1.57384f,
				    0.636568f,
				    0.665714f,
				   -1.76874f,
				    0.92351f,
				   -0.00609398f,
				    1.22878f,
				    0.697928f,
			 	   -0.837606f,
				    0.0337899f,
				   -0.0337059f,
				    1.55245f,
				    0.978734f,
				   -0.42496f,
				    0.351244f};

	   float sitBackwardAnglesArray[] = { 0.922581f,
				    0.00609398f,
				   -0.049046f,
				   -0.32525f,
				   -0.41107f,
				   -0.0168321f,
				   -0.032172f,
				   -0.0923279f,
				    0.765424f,
				    0.1733f,
				   -1.73039f,
				    0.926578f,
				    0.00157595f,
				    0.131966f,
				    0.645772f,
				   -0.428028f,
				    0.021518f,
				   -0.032172f,
				   -0.0843279f,
				    0.825334f,
				    0.078192f,
				    0.44942f};
	
		angleNames = AL::ALValue(std::vector<std::string> (angleNamesArray, angleNamesArray + 22));
		sitForwardAngles = AL::ALValue(std::vector<float> (sitForwardAnglesArray, sitForwardAnglesArray + 22));
		sitBackwardAngles = AL::ALValue(std::vector<float> (sitBackwardAnglesArray, sitBackwardAnglesArray + 22));
    
        AL::ALMemoryProxy memproxy(pip, pport);
       // AL::ALMotionProxy motproxy(pip, pport);
        AL::ALMotionProxy motion(pip, pport);
	motproxy = motion;
        AL::ALValue val1;
        AL::ALValue val2;
        double v1;
        double v2;
        
        val1 = memproxy.getData("Device/SubDeviceList/InertialSensor/GyroscopeY/Sensor/Value");
        val2 = val1;
        
        timeval t1;
        timeval t2;
        double elapsedTime;
        
        gettimeofday(&t1, NULL);
        
        sleep(1);
        
        gettimeofday(&t2, NULL);
        
        elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
        elapsedTime = (t2.tv_sec - t1.tv_sec) / 1000.0;
       
        std::cout << elapsedTime << std::endl;
        
        motproxy.setStiffnesses("Body", 1.0f);
        
        while ((t2.tv_sec - t1.tv_sec) < 60){
            std::cout << (t2.tv_sec - t1.tv_sec);
            motproxy.setAngles(angleNames, sitForwardAngles, speed);
            qi::os::sleep(1.3);
            motproxy.setAngles(angleNames, sitBackwardAngles, speed);
            qi::os::sleep(1.3);
            gettimeofday(&t2, NULL);
            std::cout << "Looped" << std::endl;
        }
        
        val1 = memproxy.getData("Device/SubDeviceList/InertialSensor/GyroscopeY/Sensor/Value");
        v1 = val1;
        v2 = val1;
        
        SMA gyro(20);
        
        AL::ALValue gyrodata [20];
        double gyrodouble [20];
        double old, current;
        current = 0;
        old = current;
        bool eq = false;
		
        std::cout << "Average: ";
        /*while ((t2.tv_sec - t1.tv_sec) < 180){
			
			//Gets the moving average
            for (int i = 0; i < 20; i++){
                gyrodata[i] = memproxy.getData("Device/SubDeviceList/InertialSensor/GyroscopeY/Sensor/Value");
                gyrodouble[i] = gyrodata[i];
				if (floor(gyrodouble[i]) == 0){
					eq = true;
				}
            }
            for(double * itr = gyrodouble; itr < gyrodouble + 20; itr++){
                gyro.add(*itr);
                std::cout << gyro.avg();                
            }
			current = gyro.avg(); 
			
			if (current < abs(old) && eq == true){
				if (current < 0){
					motproxy.setAngles(angleNames, sitBackwardAngles, speed);
				}
				else if(current > 0){
					motproxy.setAngles(angleNames, sitForwardAngles, speed);
				}
				eq = false;
			}
            
            old = current;
              
            std::cout << "\nOverall Average: " << gyro.avg();
            
        }*/
        
        std::ofstream outfile;
	outfile.open("gyroOutputUsingAngVell0.01h1.0.txt");
        outfile << "Time \t GyroX \t v1-v2" << std::endl;
        std::cout << "Moving onto gyroscope" << std::endl;
	double currentTime = t2.tv_sec;
	double lastTime = t2.tv_sec-0.01;
	
        while ((t2.tv_sec - t1.tv_sec) < 180){
           double deltat = timediff(currentTime,lastTime);
	   bool deltav = grad(v1,v2,deltat);
	   if ( (v1 > v2) && deltav){
              motproxy.setAngles(angleNames, sitBackwardAngles, speed);
              qi::os::msleep(700);
           }
           
           if ( (v1 < v2) && deltav){
              motproxy.setAngles(angleNames, sitForwardAngles, speed);
              qi::os::msleep(700);
           }
           
           v2 = v1;
           val1 = memproxy.getData("Device/SubDeviceList/InertialSensor/GyroscopeY/Sensor/Value");
           v1 = val1;
           std::cout << "GyroscopeY " << v1 << std::endl;
           gettimeofday(&t2, NULL);
	   lastTime = currentTime;
	   currentTime = t2.tv_sec;
	   outfile << currentTime << "\t" << v1 << "\t" << (v1-v2)/deltat<<std::endl;
        }
        
    
    
    //----------------------------------

    }
    catch(const AL::ALError& e){
        std::cerr << "Exception: " << e.what() << std::endl;
    }
	// Close the broker
	if(verb)
		std::cout << "Closing broker..." << std::endl;
	broker->shutdown();

	// Exit program
	if(verb)
		std::cout << "Exiting..." << std::endl;

	return 0;
}

// Order of vector - 1 = ascending, 0 = neither, -1 = descending
// Non-zero order requires entire vector to follow that order
// Default to 0 (no order)
int vectorOrder (std::vector<float> vec)
{
	int order = 0;
	if (vec.size() > 1)
	{
		if (vec[1] > vec[0])
			order = 1;
		else if (vec[1] < vec[0])
			order = -1;
		else
			order = 0;
	}
	for (unsigned int i = 2; i < vec.size(); i++)
	{
		if (vec[i] > vec[i-1] && order == 1)
			continue;
		else if (vec[i] < vec[i-1] && order == -1)
			continue;
		else
			return 0;
	}
	return order;
}
