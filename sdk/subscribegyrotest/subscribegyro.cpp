/**
 * Copyright (c) 2011 Aldebaran Robotics
 */

#include "subscribegyro.h"
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

//std::fstream file;
file.open("/home/nao/data/gyroswingdata.txt");

}

SubscribeGyro::~SubscribeGyro() {
  //fMemoryProxy.unsubscribeToEvent("onRightSubscribeGyroPressed", "SubscribeGyro");
}

void SubscribeGyro::init() {
  try {
    /** Create a proxy to ALMemory.
    */
    fMemoryProxy = AL::ALMemoryProxy(getParentBroker());

    //fState = fMemoryProxy.getData("RightSubscribeGyroPressed");
    /** Subscribe to event LeftSubscribeGyroPressed
    * Arguments:
    * - name of the event
    * - name of the module to be called for the callback
    * - name of the bound method to be called on event
    */
    //fMemoryProxy.subscribeToEvent("RightSubscribeGyroPressed", "SubscribeGyro", "onRightSubscribeGyroPressed");
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
void SubscribeGyro::onMoveBackward(){
  qiLogInfo("subscribegyro.gyroswing") << "Executing callback method on move backward" << std::endl;
  AL::ALCriticalSection section(fCallbackMutex);
  gettimeofday(&currentTime);
  file << "Back\t" << currentTime.tv_sec << std::endl;
  try {
    //fTtsProxy = AL::ALTextToSpeechProxy(getParentBroker());
    //fTtsProxy.say("Back");
  }
  catch (const AL::ALError& e) {
    qiLogError("module.example") << e.what() << std::endl;
  }
}

void SubscribeGyro::onMoveForward(){
     qiLogInfo("subscribegyro.gyroswing") << "Executing callback method on move forward" << std::endl;
     AL::ALCriticalSection section(fCallbackMutex);
  gettimeofday(&currentTime);
  file << "Forward\t" << std::endl;

   try {

    //fTtsProxy = AL::ALTextToSpeechProxy(getParentBroker());
    //fTtsProxy.say("Forward");
  }
  catch (const AL::ALError& e) {
    qiLogError("module.example") << e.what() << std::endl;
  }
}
