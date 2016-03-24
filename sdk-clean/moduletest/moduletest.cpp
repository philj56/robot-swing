/* moduletest.cpp
 * Generic module source file
 */

#include "moduletest.h"

#include <iostream>
#include <alcommon/albroker.h>
#include <alcommon/alproxy.h>
#include <alproxies/altexttospeechproxy.h>

// Constructor
ModuleTest::ModuleTest(boost::shared_ptr<AL::ALBroker> broker,
		   const std::string &name)
	: AL::ALModule(broker, name)
{
	// Set description, and bind each function
	setModuleDescription("Simple generic testing module");

	functionName("printHello", getName(), "Print hello to the world");
	BIND_METHOD(ModuleTest::printHello);
	
	functionName("sayHello", getName(), "Say hello to the world");
	BIND_METHOD(ModuleTest::sayHello);

	functionName("printWord", getName(), "Print a given word.");
	addParam("word", "The word to print.");
	BIND_METHOD(ModuleTest::printWord);

	functionName("returnTrue", getName(), "Return true.");
	setReturn("boolean", "return true");
	BIND_METHOD(ModuleTest::returnTrue);

	// Set broker parent IP and port
	pip = broker->getParentIP();
	pport = broker->getParentPort();
}

// Destructor
ModuleTest::~ModuleTest()
{
}

// init() - called as soon as the module is constructed
void ModuleTest::init()
{
}

// Print hello to the command line
void ModuleTest::printHello()
{
	std::cout << "Hello" << std::endl;
}

// Say hello
void ModuleTest::sayHello()
{
  try
  {
  	// Create a proxy to TTS
	AL::ALTextToSpeechProxy tts(pip, pport);
  	// Say hello
  	tts.say("Hello");
  }
  catch(const AL::ALError&)
  {
	std::cerr << "Could not get proxy to ALTextToSpeech" << std::endl;
  }
}

// Print a word to the command line
void ModuleTest::printWord(const std::string &word)
{
	std::cout << word << std::endl;
}

// Return true
bool ModuleTest::returnTrue()
{
	return true;
}
