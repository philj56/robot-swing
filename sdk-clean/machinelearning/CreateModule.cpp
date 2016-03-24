/**
 * @file CreateModule.cpp
 *
 * @brief Implementation/Definition file for CreateModule function.
 *
 * @author Coding Team 2015-2016
 * @date February, 2016
 */

#include "CreateModule.h"

bool CreateModule(std::string libName, std::string moduleName, boost::shared_ptr<AL::ALBroker> broker, bool verb, bool find) {
	std::string library;

	if (find) {
		// Find the desired library
		if (verb)
			std::cout << "Finding " << libName << "..." << std::endl;
		library = qi::path::findLib(libName.c_str());

	}
	else {
		// Use libName as library path
		library = libName;
	}

	// Open the library
	if (verb)
		std::cout << "Loading " << library << "..." << std::endl;
	void* handle = qi::os::dlopen(library.c_str());
	if (!handle) {
		qiLogWarning(moduleName.c_str()) << "Could not load library:"
			<< qi::os::dlerror()
			<< std::endl;
		return -1;
	}

	// Load the create symbol
	if (verb)
		std::cout << "Loading _createModule symbol..." << std::endl;
	int(*createModule)(boost::shared_ptr<AL::ALBroker>) = (int(*)(boost::shared_ptr<AL::ALBroker>))(qi::os::dlsym(handle, "_createModule"));
	if (!createModule) {
		qiLogWarning(moduleName.c_str()) << "Could not load symbol _createModule: "
			<< qi::os::dlerror()
			<< std::endl;
		return -1;
	}

	// Check if module is already present
	if (verb) {
		std::cout << "Module " << moduleName << " is ";
		if (!(broker->isModulePresent(moduleName))) {
			std::cout << "not ";
		}
		std::cout << "present" << std::endl;
	}

	// Create an instance of the desired module
	if (verb)
		std::cout << "Creating " << moduleName << " instance..." << std::endl;
	createModule(broker);

	// Check for module creation
	if (verb) {
		std::cout << "Module " << moduleName.c_str() << " is ";
		if (!(broker->isModulePresent(moduleName))) {
			std::cout << "not ";
		}
		std::cout << "present" << std::endl;
	}
	if (broker->isModulePresent(moduleName))
		return true;
	else
		return false;
}