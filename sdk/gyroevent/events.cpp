/**
 *
 *
 */

#include "events.h"

#include <alvalue/alvalue.h>
#include <alcommon/alproxy.h>
#include <alcommon/albroker.h>
  
#include <vector>
#include <cmath>

#include <qi/os.hpp>
#include <qi/log.hpp>

const float forwardspos = - M_PI  / 4.0 ;
const float backwardspos = - M_PI * 3.0 / 4.0 ;

GyroEvents::GyroEvents(
  boost::shared_ptr<AL::ALBroker> broker,
  const std::string& name):
      AL::ALModule(broker, name),
      fMemoryProxy(getParentBroker())
{
  setModuleDescription("");
  datapoints = 20.0;

  functionName("callback", getName(), "");
  BIND_METHOD(GyroEvents::callback);
  
  functionName("Run", getName(), "Start gyro reading");
  BIND_METHOD(GyroEvents::Run);

  lastvalues.resize(20);
}

GyroEvents::~GyroEvents() {
  fMemoryProxy.unsubscribeToEvent("Gyroevent", "Events");
  fMemoryProxy.unsubscribeToMicroEvent("ExampleMicroEvent", "Events");
}

void GyroEvents::init() {
  try {
    //Allow ALMemory to start the Events module when someone subscribe to Gyroevent
    //This module should inherit from ALExtractor
    //It's not necessary to call this function when you dont want the autostart feature
/*    fMemoryProxy.declareEvent("Gyroevent", "GyroEvents");
    fMemoryProxy.declareEvent("AnglePeak", "GyroEvents");
    fMemoryProxy.declareEvent("Gyroaverageturn", "GyroEvents");
    fMemoryProxy.declareEvent("GyroZero", "GyroEvents");
    fMemoryProxy.declareEvent("GyroMoveForward", "GyroEvents");
    fMemoryProxy.declareEvent("GyroMoveBackward", "GyroEvents");
  */  
    gettimeofday(&startTime);
    gettimeofday(&currentTime);
    gettimeofday(&MovementTime);
    timer();
/*
    fMemoryProxy.subscribeToEvent("Gyroevent", "GyroEvents", "callback");
    fMemoryProxy.subscribeToEvent("Gyroevent", "GyroEvents", "AverageTurn");
    fMemoryProxy.subscribeToEvent("Gyroevent", "GyroEvents", "AnglePeak");
  */  
    //fMemoryProxy.subscribeToMicroEvent("ExampleMicroEvent", "GyroEvents", "AnotherUserDataToIdentifyEvent", "callback");

//    fGyroY = (AL::ALValue*)(fMemoryProxy.getDataPtr("Device/SubDeviceList/InertialSensor/GyroscopeY/Sensor/Value"));
	
       
	
	//fGyroY = (AL::ALValue*)(fMemoryProxy.getDataPtr("Device/SubDeviceList/InertialSensor/GyrY/Sensor/Value"));
	
/*	AL::ALValue ttt = *fGyroY;
	temp = float(ttt);	

    for (int i = 0; i < datapoints; i++){
	fCurrentGyro = *fGyroY;         
	fgyro = float(fCurrentGyro);
    
        ftotal += fgyro;
        lastvalues[i] = fgyro;
	qi::os::msleep(5);
    }*/
            max = 0;
            min = 0;
    
  //  Average();
	Run(); 
    
  }
  catch (const AL::ALError& e) {
    qiLogError("module.example") << e.what() << std::endl;
  }

}

void GyroEvents::Run(){
	bool ev = false;
	AL::ALValue temp;
  //  int count = 0;
	qiLogInfo("GyroEvents.Run") << "Begining Main Loop" << std::endl;
try{
	while (true){
//        count++;
        //This needs to sleep during a movement
        //After movement is complete, reset average 
        
        //timer();
 //       fgyro = *fGyroY; //Dereferences the current gyro value from pointer for fast read speed
		/*fMemoryProxy.raiseEvent("GyroMoveForward", true);
		qi::os::sleep(1.5);
		std::cout << "Slept" << std::endl;	
    
		fMemoryProxy.raiseEvent("GyroMoveForward", false);
		qi::os::sleep(1.5);*/
        fCurrentGyro = (fMemoryProxy.getData("Device/SubDeviceList/InertialSensor/GyroscopeY/Sensor/Value"));

        
//	fCurrentGyro = *fGyroY;         
	    fgyro = float(fCurrentGyro);
//	fgyro = *fGyroY;
        ftotal -= lastvalues[0];
        lastvalues.erase(lastvalues.begin());
        ftotal += fgyro;
        lastvalues.push_back(fgyro);
        Average();
        
/*        if(newperiod  && Time > 5000){
            min = 0;
            max = 0;
        }
        
  */      
        if ((pTime - oTime > 500) && floor(faverage*100) == 0){
	    qiLogInfo("GyroEvents.Loop") << "New Period" << std::endl;
            newperiod = true;
            pTime = oTime;
            //amp = (max + abs(min))/2.0;
            omin = min;
            omax = max;
            //max = 0;
            //min = 0;
	 /*   if (faverage < 0){
		max = 0;
		}
	    else {
		min = 0;
		}
*/
//	    amp = (omax + abs(omin))/2.0;

        }
        if (faverage > 0 && faverage > max){
            max = faverage;
        }
        else if(faverage < 0 && faverage < min){
            min = faverage;
        }
        else {//if (faverage > min && faverage < max){
            newperiod = false;
            amp = (max + std::abs(min))/2.0;
  //          amp = (omax + abs(omin))/2.0;

        }
        
        position = asin(faverage/amp) + M_PI / 2;
        
	   while (position > M_PI){position -= 2*M_PI;};
	   while (position < -M_PI){position += 2*M_PI;};
       
//        position = fmod(position + 2*M_PI, 2*M_PI);

        timer();
        if (time > 500 && std::abs(position - forwardspos)/forwardspos < 0.05){
            //Move to forwards position
            gettimeofday(&MovementTime);
            
            if (faverage < 0){      
                fMemoryProxy.raiseEvent("GyroMoveForward", true);
                qiLogInfo("GyroEvents.Loop") << "Msg: " << "Forwards" << std::endl;
//		max = 0.0;
            }            
            else if (faverage > 0){
                 fMemoryProxy.raiseEvent("GyroMoveBackward", true);
                 qiLogInfo("GyroEvents.Loop") << "Msg: " << "Backwards" << std::endl;
            }
	        
			ev = true;
        }
        /*else if (time > 500 && std::abs(position - backwardspos)/backwardspos < 0.10){
            //Move to backwards position
            gettimeofday(&MovementTime);
            fMemoryProxy.raiseEvent("GyroMoveBackward", true);
	        qiLogInfo("GyroEvents.Loop") << "Msg: " << "Backwards" << std::endl;
			ev = true;
        }*/
		
       // fMemoryProxy.raiseEvent("GyroMoveForward", false);
       // fMemoryProxy.raiseEvent("GyroMoveBackward", false);       
       
        gettimeofday(&currentTime);
//        qiLogInfo("GyroEvents.Loop") << "Msg: " << Time  << " " <<   fgyro << " " << faverage << " " << amp << " " << position << " " << (faverage/amp)  << " " << max << " " << min   << std::endl;
        
		if (time > 400 && ev == true){
			fMemoryProxy.raiseEvent("GyroMoveBackward", false);
			fMemoryProxy.raiseEvent("GyroMoveForward", false);
			ev = false;
		}  

	qi::os::msleep(20);
    }
}
 catch (const AL::ALError& e) {
    qiLogError("module.example") << e.what() << std::endl;
  }

}

/*
void GyroEvents::xStartDetection(const int pPeriod, const float pPrecision)
{
	return;
}

void GyroEvents::xStopDetection()
{
	return;
}
*/


void GyroEvents::timer(){
    
    time = 1000 * (currentTime.tv_sec - MovementTime.tv_sec) 
			       + 0.001 * static_cast<float>(static_cast<int>(currentTime.tv_usec) - static_cast<int>(MovementTime.tv_usec));
    Time = 1000 * (currentTime.tv_sec - startTime.tv_sec) 
			       + 0.001 * static_cast<float>(static_cast<int>(currentTime.tv_usec) - static_cast<int>(startTime.tv_usec));
	pTime = Time;	
}

void GyroEvents::Average(){
    foldaverage = faverage; 
    faverage = ftotal/datapoints;
    
   /* if (abs(foldaverage) > abs(faverage)){
        fMemoryProxy.raiseEvent("Gyroaverageturn", foldaverage);
    }
    else if(abs(foldaverage) < abs(faverage) && floor(faverage*10) == 0){
        anglepeak = true;
        AnglePeak(anglepeak);
    }*/
}

void GyroEvents::AnglePeak(const bool& value){
    //Needs to check that it didnt just reach peak;
    fMemoryProxy.raiseEvent("AnglePeak", value);
   //Motiontools proxy -> do movement;
}



void GyroEvents::generateEvent(const float& value) {
  /** Raise an event with its value (here a float, but could be something else.*/
  fMemoryProxy.raiseEvent("Gyroevent", value);
}

void GyroEvents::generateMicroEvent(const float& value) {
  /** Raise an event with its value (here a float, but could be something else.*/
  fMemoryProxy.raiseMicroEvent("ExampleMicroEvent", value);
}

void GyroEvents::callback(const std::string &key, const AL::ALValue &value, const AL::ALValue &msg) {
  
  
  qiLogInfo("module.example") << "Callback:" << key << std::endl;
  qiLogInfo("module.example") << "Value   :" << value << std::endl;
}
