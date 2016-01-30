#ifndef MOVETEST_H
#define MOVETEST_H

#include <iostream>
#include <cstdlib>
#include <alcommon/almodule.h>

namespace AL
{
	class ALBroker;
}

class ModuleTest : public AL::ALModule
{
public:
	ModuleTest(boost::shared_ptr<AL::ALBroker> broker,
		 const std::string &name);

	virtual ~ModuleTest();

	virtual void init();

	void printHello();
	
	void sayHello();

	void printWord(const std::string &word);

	bool returnTrue();
		
private:
	std::string pip;

	int pport;
};

#endif /* MOVETEST_H */
