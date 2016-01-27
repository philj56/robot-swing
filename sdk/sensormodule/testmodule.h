#ifndef TESTMODULE_H
#define TESTMODULE_H

#include <iostream>
#include <string>
#include <alcommon/almodule.h>

namespace AL {
    class ALBroker;
}

//Inherits from ALModule so it can be run as a remote executable
class TestModule : public AL::ALModule{

public:
    TestModule(boost::shared_ptr<AL::ALBroker> broker, const std::string &name);
    virtual ~TestModule();
    virtual void init();

    void printWord(const std::string &word);
    bool returnTrue();


};


#endif // TESTMODULE_H
