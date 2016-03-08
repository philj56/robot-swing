/**
 * Copyright (c) 2011 Aldebaran Robotics
 */

#include "bumper.h"

#include <alvalue/alvalue.h>
#include <alcommon/alproxy.h>
#include <alcommon/albroker.h>
#include <qi/log.hpp>
#include <althread/alcriticalsection.h>

Bumper::Bumper(
  boost::shared_ptr<AL::ALBroker> broker,
  const std::string& name): AL::ALModule(broker, name),
    fCallbackMutex(AL::ALMutex::createALMutex())
{
  setModuleDescription("This module presents how to subscribe to a simple event (here RightBumperPressed) and use a callback method.");

  //functionName("onRightBumperPressed", getName(), "Method called when the right bumper is pressed. Makes a LED animation.");
  functionName("onMoveBackward", getName(), "MoveBackward");
  //BIND_METHOD(Bumper::onRightBumperPressed)
  BIND_METHOD(Bumper::onMoveBackward)
  functionName("onMoveForward", getName(), "MoveForward");
  BIND_METHOD(Bumper::onMoveForward)
}

Bumper::~Bumper() {
  //fMemoryProxy.unsubscribeToEvent("onRightBumperPressed", "Bumper");
}

void Bumper::init() {
  try {
    /** Create a proxy to ALMemory.
    */
    fMemoryProxy = AL::ALMemoryProxy(getParentBroker());

    //fState = fMemoryProxy.getData("RightBumperPressed");
    /** Subscribe to event LeftBumperPressed
    * Arguments:
    * - name of the event
    * - name of the module to be called for the callback
    * - name of the bound method to be called on event
    */
    //fMemoryProxy.subscribeToEvent("RightBumperPressed", "Bumper", "onRightBumperPressed");
    fMemoryProxy.subscribeToEvent("GyroMoveForward", "Bumper", "onMoveForward");
    fMemoryProxy.subscribeToEvent("GyroMoveBackward", "Bumper", "onMoveBackward");
  }
  catch (const AL::ALError& e) {
    qiLogError("module.example") << e.what() << std::endl;
  }
}
/*
void Bumper::onRightBumperPressed() {
  qiLogInfo("module.example") << "Executing callback method on right bumper event" << std::endl;
  


  //fState =  fMemoryProxy.getData("RightBumperPressed");
  if (fState  > 0.5f) {
    return;
  }
  try {
    fTtsProxy = AL::ALTextToSpeechProxy(getParentBroker());
    fTtsProxy.say("Right bumper pressed");
  }
  catch (const AL::ALError& e) {
    qiLogError("module.example") << e.what() << std::endl;
  }
}
*/
void Bumper::onMoveBackward(){
  qiLogInfo("bumper.gyroswing") << "Executing callback method on move backward" << std::endl;
  AL::ALCriticalSection section(fCallbackMutex);

  try {
    std::cout << "Back" << std::endl;
    fTtsProxy = AL::ALTextToSpeechProxy(getParentBroker());
    fTtsProxy.say("Back");
  }
  catch (const AL::ALError& e) {
    qiLogError("module.example") << e.what() << std::endl;
  }
}

void Bumper::onMoveForward(){
     qiLogInfo("bumper.gyroswing") << "Executing callback method on move forward" << std::endl;
  AL::ALCriticalSection section(fCallbackMutex);

   try {
    std::cout << "Forward" << std::endl;
    fTtsProxy = AL::ALTextToSpeechProxy(getParentBroker());
    fTtsProxy.say("Forward");
  }
  catch (const AL::ALError& e) {
    qiLogError("module.example") << e.what() << std::endl;
  }
}
