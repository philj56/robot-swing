#include "testmodule.h"

#include <iostream>
#include <alcommon/albroker.h>

TestModule::TestModule(boost::shared_ptr<AL::ALBroker> broker, const std::string &name)
 : AL::ALModule(broker, name)
{
    setModuleDescription("Custom Sensor Data");

    //Callback methods and descriptions
    //Method become available to other modules

    functionName("printWord", getName(), "Prints given word");
    addParam("word", "Word to be printed");
    BIND_METHOD(TestModule::printWord);

    functionName("returnTrue", getName(), "Returns true");
    BIND_METHOD(TestModule::returnTrue);

    //Other methods bind here
}

TestModule::~TestModule(){}

//init is called just after construction
void TestModule::init(){

    std::cout << returnTrue() << std::endl;
}

void TestModule::printWord(const std::string &word){

    std::cout << word <<std::endl;
}

bool TestModule::returnTrue()
{
    return true;
}
