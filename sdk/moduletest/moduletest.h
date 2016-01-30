/* moduletest.h
 * Generic module header file
 */

#ifndef MODULETEST_H
#define MODULETEST_H

#include <iostream>
#include <cstdlib>
#include <alcommon/almodule.h>

// Forward declaration of ALBroker
namespace AL
{
	class ALBroker;
}

// ModuleTest - generic NAOqi module
class ModuleTest : public AL::ALModule
{
public:
	// Constructor
	ModuleTest(boost::shared_ptr<AL::ALBroker> broker,
		 const std::string &name);

	// Destructor
	virtual ~ModuleTest();

	// init() - called just after the constructor
	virtual void init();

	// Print "Hello" to the command line
	void printHello();
	
	// Say "Hello" via ALTextToSpeech
	void sayHello();

	// Print arbitrary word to command line
	void printWord(const std::string &word);

	// Return true
	bool returnTrue();
		
private:
	// Broker Parent IP and port
	std::string pip;
	int pport;
};

#endif /* MODULETEST_H */
