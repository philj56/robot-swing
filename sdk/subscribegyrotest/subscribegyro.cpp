/**
 * Copyright (c) 2011 Aldebaran Robotics
 */

#include "subscribegyro.h"
#include <qi/os.hpp>
#include <fstream>
#include <alvalue/alvalue.h>
#include <alcommon/alproxy.h>
#include <alcommon/albroker.h>
#include <qi/log.hpp>
#include <althread/alcriticalsection.h>

SubscribeGyro::SubscribeGyro(
  boost::shared_ptr<AL::ALBroker> broker,
  const std::string& name): AL::ALModule(broker, name),
    fCallbackMutex(AL::ALMutex::createALMutex())
{
  setModuleDescription("This module presents how to subscribe to a simple event (here RightSubscribeGyroPressed) and use a callback method.");

  //functionName("onRightSubscribeGyroPressed", getName(), "Method called when the right subscribegyro is pressed. Makes a LED animation.");
  functionName("onMoveBackward", getName(), "MoveBackward");
  //BIND_METHOD(SubscribeGyro::onRightSubscribeGyroPressed)
  BIND_METHOD(SubscribeGyro::onMoveBackward)
  functionName("onMoveForward", getName(), "MoveForward");
  BIND_METHOD(SubscribeGyro::onMoveForward)
  functionName("GyroStart", getName(), "StartMovement");
  BIND_METHOD(SubscribeGyro::GyroStart)

//std::fstream file;
file.open("/home/nao/data/gyroswingdata.txt");

}

SubscribeGyro::~SubscribeGyro() {
  //fMemoryProxy.unsubscribeToEvent("onRightSubscribeGyroPressed", "SubscribeGyro");
}

void SubscribeGyro::init() {

}

void SubscribeGyro::GyroStart () {
    try {
    /** Create a proxy to ALMemory.
    */
  	gettimeofday(&startTime);
    fMemoryProxy = AL::ALMemoryProxy(getParentBroker());

    bool test = CreateModule("/home/nao/lib/libmovementtools.so", "MovementTools", getParentBroker(), true, false);
    //fState = fMemoryProxy.getData("RightSubscribeGyroPressed");
    /** Subscribe to event LeftSubscribeGyroPressed
    * Arguments:
    * - name of the event
    * - name of the module to be called for the callback
    * - name of the bound method to be called on event
    */
    //fMemoryProxy.subscribeToEvent("RightSubscribeGyroPressed", "SubscribeGyro", "onRightSubscribeGyroPressed");

	AL::ALProxy genericProxy ("MovementTools", "127.0.0.1" , 9559);
	genericProxy.callVoid("setSpeed", 0.8f);
   
		timer();
        gettimeofday(&currentTime);
     gettimeofday(&lastTime);
    
/*	while (time < 180000){
		qiLogInfo("subscribegyro.gyroswing") << "Time: " << time <<  std::endl;
		
		genericProxy.callVoid("swingForwards");
		qi::os::msleep(1250);
		genericProxy.callVoid("swingBackwards");			
		qi::os::msleep(1250);
	        gettimeofday(&currentTime);
		timer();
	}
*/

    fMemoryProxy.subscribeToEvent("GyroMoveForward", "SubscribeGyro", "onMoveForward");
    fMemoryProxy.subscribeToEvent("GyroMoveBackward", "SubscribeGyro", "onMoveBackward");
  }
  catch (const AL::ALError& e) {
    qiLogError("module.example") << e.what() << std::endl;
  }
    
}

/*
void SubscribeGyro::onRightSubscribeGyroPressed() {
  qiLogInfo("module.example") << "Executing callback method on right subscribegyro event" << std::endl;




  //fState =  fMemoryProxy.getData("RightSubscribeGyroPressed");
  if (fState  > 0.5f) {
    return;
  }
  try {
    fTtsProxy = AL::ALTextToSpeechProxy(getParentBroker());
    fTtsProxy.say("Right subscribegyro pressed");
  }
  catch (const AL::ALError& e) {
    qiLogError("module.example") << e.what() << std::endl;
  }
}
*/
void SubscribeGyro::onMoveBackward(const std::string &key, const AL::ALValue &value, const AL::ALValue &msg){
  qiLogInfo("subscribegyro.gyroswing") << "Executing callback method on move backward" << std::endl;
  AL::ALCriticalSection section(fCallbackMutex);
  gettimeofday(&currentTime);
  file << "Back\t" << currentTime.tv_sec << std::endl;
 bool test =  value;
 		timer();
  try { 
       AL::ALProxy genericProxy ("MovementTools", "127.0.0.1" , 9559);
       genericProxy.callVoid("setSpeed", 0.8f);

	if (test == true && t > 300) {
		genericProxy.callVoid("swingBackwards");

		 qiLogInfo("subscribegyro.gyroswing") << "Back! " << currentTime.tv_sec  << std::endl;
		 //qi::os::sleep(0.5);
		gettimeofday(&lastTime); 

	}
    //fTtsProxy = AL::ALTextToSpeechProxy(getParentBroker());
    //fTtsProxy.say("Back");
  }
  catch (const AL::ALError& e) {
    qiLogError("module.example") << e.what() << std::endl;
  }
}

void SubscribeGyro::onMoveForward(const std::string &key, const AL::ALValue &value, const AL::ALValue &msg){
     AL::ALCriticalSection section(fCallbackMutex);
    gettimeofday(&currentTime);
    file << "Forward\t" << std::endl;
     bool test =  value;
	 timer();
     qiLogInfo("subscribegyro.gyroswing") << "Executing callback method on move forward: " << test  << std::endl;

   try {
       AL::ALProxy genericProxy ("MovementTools", "127.0.0.1" , 9559);
       genericProxy.callVoid("setSpeed", 0.8f);

 	 if (test == true && t > 300) {
		genericProxy.callVoid("swingForwards");

                qiLogInfo("subscribegyro.gyroswing") << "Forward! " << currentTime.tv_sec <<  std::endl;
		//qi::os::sleep(0.5);
		gettimeofday(&lastTime);
        }

    //fTtsProxy = AL::ALTextToSpeechProxy(getParentBroker());
    //fTtsProxy.say("Forward");
  }
  catch (const AL::ALError& e) {
    qiLogError("module.example") << e.what() << std::endl;
  }
}

void SubscribeGyro::timer(){
    
    time = 1000 * (currentTime.tv_sec - startTime.tv_sec) 
			       + 0.001 * static_cast<float>(static_cast<int>(currentTime.tv_usec) - static_cast<int>(startTime.tv_usec));
	t = 1000 * (currentTime.tv_sec - lastTime.tv_sec) 
			       + 0.001 * static_cast<float>(static_cast<int>(currentTime.tv_usec) - static_cast<int>(lastTime.tv_usec));
}
