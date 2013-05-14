
#ifndef __RESOURCE_PATH_CPP__
#define __RESOURCE_PATH_CPP__

#include "ResourcePath.hpp"

#ifndef __macos__

std::string resourcePath(void) {
	return "Resources/";
}

#endif

#endif