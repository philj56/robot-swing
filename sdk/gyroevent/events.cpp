/**
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

const float forwardspos = M_PI * 7.0 / 4.0;
const float backwardspos = M_PI * 3.0 / 4.0;

GyroEvents::GyroEvents(
  boost::shared_ptr<AL::ALBroker> broker,
  const std::string& name):
      AL::ALExtractor(broker, name),
      fMemoryProxy(getParentBroker())
{
  setModuleDescription("");
  datapoints = 20.0;

  functionName("callback", getName(), "");
  BIND_METHOD(GyroEvents::callback)

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
    fMemoryProxy.declareEvent("Gyroevent", "GyroEvents");
    fMemoryProxy.declareEvent("AnglePeak", "GyroEvents");
    fMemoryProxy.declareEvent("Gyroaverageturn", "GyroEvents");
    fMemoryProxy.declareEvent("GyroZero", "GyroEvents");
    fMemoryProxy.declareEvent("GyroMoveForward", "GyroEvents");
    fMemoryProxy.declareEvent("GyroMoveBackward", "GyroEvents");
    
    gettimeofday(&startTime);
    gettimeofday(&currentTime);
    
    timer();

    fMemoryProxy.subscribeToEvent("Gyroevent", "GyroEvents", "callback");
    fMemoryProxy.subscribeToEvent("Gyroevent", "GyroEvents", "AverageTurn");
    fMemoryProxy.subscribeToEvent("Gyroevent", "GyroEvents", "AnglePeak");
    
    fMemoryProxy.subscribeToMicroEvent("ExampleMicroEvent", "GyroEvents", "AnotherUserDataToIdentifyEvent", "callback");

    fGyroY = (AL::ALValue*)(fMemoryProxy.getDataPtr("Device/SubDeviceList/InertialSensor/GyroscopeY/Sensor/Value"));
    
    for (int i = 0; i < datapoints; i++){
        fgyro = *fGyroY;
        ftotal += fgyro;
        lastvalues[i] = fgyro;
    }
    
    Average();
    
    while (true){
        //This needs to sleep during a movement
        //After movement is complete, reset average 
        
        //timer();
        fgyro = *fGyroY; //Dereferences the current gyro value from pointer for fast read speed
        ftotal -= lastvalues[0];
        lastvalues.erase(lastvalues.begin());
        ftotal += fgyro;
        lastvalues.push_back(fgyro);
        Average();
        
        //generateEvent(faverage);
        
        if (floor(faverage*10) == 0){
            newperiod = true;
            amp = (max + abs(min))/2.0;
            omin = min;
            omax = max;
            max = 0;
            min = 0;
        }
        else if(newperiod == true && faverage > max){
            max = faverage;
        }
        else if(newperiod == true && faverage < min){
            min = faverage;
//            amp = (max + min)/2.0;
        }
        else {
            newperiod = false;
        }
        
        position = asin(faverage/amp) + M_PI / 2;
        position = fmod(position + 2*M_PI, 2*M_PI);
        timer();
        if (time > 500 && std::abs(position - forwardspos)/forwardspos < 0.05){
            //Move to forwards position
            gettimeofday(&MovementTime);
            fMemoryProxy.raiseEvent("GyroMoveForward", true);
        }
        else if (time > 500 && std::abs(position - backwardspos)/backwardspos < 0.05){
            //Move to backwards position
            gettimeofday(&MovementTime);
            fMemoryProxy.raiseEvent("GyroMoveBackward", true);
        }
       // fMemoryProxy.raiseEvent("GyroMoveForward", false);
       // fMemoryProxy.raiseEvent("GyroMoveBackward", false);       
       
        gettimeofday(&currentTime);
        
       /* if (currentTime.tv_sec - startTime.tv_sec == 300){
            break;
        }*/
        
        
    }

    //generate a simple event for the test
    //generateEvent(42.0);
    //generateMicroEvent(42.0);
  }
  catch (const AL::ALError& e) {
    qiLogError("module.example") << e.what() << std::endl;
  }

}

void GyroEvents::xStartDetection(const int pPeriod, const float pPrecision)
{
	return;
}

void GyroEvents::xStopDetection()
{
	return;
}

void GyroEvents::timer(){
    
    time = 1000 * (currentTime.tv_sec - MovementTime.tv_sec) 
			       + 0.001 * static_cast<float>(static_cast<int>(currentTime.tv_usec) - static_cast<int>(MovementTime.tv_usec));
		
}

void GyroEvents::Average(){
    foldaverage = faverage; 
    faverage = ftotal/datapoints;
    
    if (abs(foldaverage) > abs(faverage)){
        fMemoryProxy.raiseEvent("Gyroaverageturn", foldaverage);
    }
    else if(abs(foldaverage) < abs(faverage) && floor(faverage*10) == 0){
        anglepeak = true;
        AnglePeak(anglepeak);
    }
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
  qiLogInfo("module.example") << "Msg     :" << msg << std::endl;
}
