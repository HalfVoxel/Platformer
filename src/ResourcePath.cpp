
#ifndef __RESOURCE_PATH_CPP__
#define __RESOURCE_PATH_CPP__



#ifndef __APPLE__

#include "ResourcePath.hpp"

std::string resourcePath(void) {
	return "Resources/";
}

#endif

#endif