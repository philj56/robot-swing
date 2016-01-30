#include "moduletest.h"

#include <iostream>
#include <alcommon/albroker.h>
#include <alcommon/alproxy.h>
#include <alproxies/altexttospeechproxy.h>

ModuleTest::ModuleTest(boost::shared_ptr<AL::ALBroker> broker,
		   const std::string &name)
	: AL::ALModule(broker, name)
{
	setModuleDescription("Simple movement testing module");

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

	pip = broker->getParentIP();
	pport = broker->getParentPort();
}

ModuleTest::~ModuleTest()
{
}

void ModuleTest::init()
{
	std::cout << "Parent: " << pip << ":" << pport << std::endl;
}

void ModuleTest::printHello()
{
	std::cout << "Hello" << std::endl;
}

void ModuleTest::sayHello()
{
  try
  {
  	/** Create a proxy to TTS.*/
	AL::ALTextToSpeechProxy tts(pip, pport);
  	/** Call the say method. */
  	tts.say("Hello");
  }
  catch(const AL::ALError&) // no object name given to avoid warning
  {
	std::cerr << "Could not get proxy to ALTextToSpeech" << std::endl;
  }
}

void ModuleTest::printWord(const std::string &word)
{
	std::cout << word << std::endl;
}

bool ModuleTest::returnTrue()
{
	return true;
}
