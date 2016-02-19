#ifndef CREATEMODULE_H
#define CREATEMODULE_H

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <getopt.h>
#include <boost/shared_ptr.hpp>
#include <qi/os.hpp>
#include <qi/path.hpp>
#include <qi/log.hpp>


#include <alcommon/almodule.h>
#include <alcommon/alproxy.h>
#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>
#include <alproxies/almotionproxy.h>
#include <alproxies/alrobotpostureproxy.h>

// Find and open a library, and create an instance of a module in that library
bool CreateModule(std::string libName, std::string moduleName, boost::shared_ptr<AL::ALBroker> broker, bool verb, bool find);

#endif /* CREATEMODULE_H */
