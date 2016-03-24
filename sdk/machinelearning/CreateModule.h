/**
 * @file CreateModule.h
 *
 * @brief Prototype file for CreateModule function.
 *
 * @author Coding Team 2015-2016
 * @date February, 2016
 */

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

/**
 * @brief Finds and opens a library, creating an instance of a module in that library.
 *
 * @param libName Name of library
 * @param moduleName Name of module
 * @param broker Shared pointer of ALBroker instance
 * @param verb Specifies whether to load the library and symbol (true) or not (false)
 * @param find Specifies whether to find library (true) or use libName as default library name (false)
 * @return True if successful, false otherwise
 */
bool CreateModule(std::string libName, std::string moduleName, boost::shared_ptr<AL::ALBroker> broker, bool verb, bool find);

#endif /* CREATEMODULE_H */