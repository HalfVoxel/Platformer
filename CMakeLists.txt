cmake_minimum_required(VERSION 2.6)

set(CMAKE_PROJECT_NAME "Platformer")
project(${CMAKE_PROJECT_NAME})

set(Box2D_SOURCE_DIR "/usr/local/include/")
include_directories (${Box2D_SOURCE_DIR})

set(HDRS
    ./
)


if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    file (GLOB_RECURSE HDRS src/*.h src/*.hpp)
    file (GLOB_RECURSE SRCS src/*.cpp src/*.mm)
else(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    file (GLOB_RECURSE HDRS src/*.h)
    file (GLOB_RECURSE SRCS src/*.cpp)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} PRE_BUILD COMMAND export LD_LIBRARY_PATH=/usr/local/lib )

set (CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib)
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

    FIND_LIBRARY(SFML_LIBRARY_SYSTEM NAMES sfml-system)
    FIND_LIBRARY(SFML_LIBRARY_WINDOW sfml-window)
    FIND_LIBRARY(SFML_LIBRARY_GRAPHICS sfml-graphics)
    FIND_LIBRARY(SFML_LIBRARY_AUDIO sfml-audio)
    FIND_LIBRARY(SFML_LIBRARY_NETWORK sfml-network)
    FIND_LIBRARY(BOX2D_LIBRARY Box2D)

    
else(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(SFML_LIBRARY_SYSTEM ${CMAKE_LIBRARY_PATH}/libsfml-system.so)
    set(SFML_LIBRARY_WINDOW ${CMAKE_LIBRARY_PATH}/libsfml-window.so)
    set(SFML_LIBRARY_GRAPHICS ${CMAKE_LIBRARY_PATH}/libsfml-graphics.so)
    set(SFML_LIBRARY_AUDIO ${CMAKE_LIBRARY_PATH}/libsfml-audio.so)
    set(SFML_LIBRARY_NETWORK ${CMAKE_LIBRARY_PATH}/libsfml-network.so)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

set (EXTRA_LIBS ${BOX2D_LIBRARY} ${SFML_LIBRARY_SYSTEM} ${SFML_LIBRARY_WINDOW} ${SFML_LIBRARY_GRAPHICS} ${SFML_LIBRARY_AUDIO} ${SFML_LIBRARY_NETWORK})
MESSAGE (${EXTRA_LIBS})
#FIND_LIBRARY(GLEW_LIBRARY glew)

#set (EXTRA_LIBS ${EXTRA_LIBS} ${GLEW_LIBRARY})

#------------------------------------------------------------------------------
#------------------------------------------------- Initialize Compiler flags --

set(CMAKE_CXX_COMPILER "g++")                              #define the compiler to use 
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x ")      #define the C++0X flag
#set(CMAKE_VERBOSE_MAKEFILE ON)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Volumes/Dev/OgreSDK/lib/release/Ogre.framework/Versions/1.7.3/Ogre ")      # not sure it's still required !
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ObjC++ -mmacosx-version-min=10.6")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if(WIN32)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-static-libgcc -static-libstdc++")
endif(WIN32)



# ---- Executable ------


add_executable(${CMAKE_PROJECT_NAME} ${HDRS} ${SRCS})


target_link_libraries (${CMAKE_PROJECT_NAME} ${EXTRA_LIBS})

 


file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)


if(MINGW OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/bin)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


# Install required files into the OSX Bundle (once again I placed all the stuff required into my project folder/data/etc.)
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY MACOSX_BUNDLE TRUE)

	set (OSX_BUNDLE_CONTENTS ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/${CMAKE_PROJECT_NAME}.app/Contents)
    set (RESOURCES_DESTINATION ${OSX_BUNDLE_CONTENTS})

    #INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/data/media DESTINATION bin/${CMAKE_PROJECT_NAME}.app/Contents/Resources)
   	INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/data/osx/Frameworks DESTINATION ${OSX_BUNDLE_CONTENTS})

   	set (TARGET_RESOURCES_DIR ${OSX_BUNDLE_CONTENTS})

   	INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/data/osx/Frameworks DESTINATION ${OSX_BUNDLE_CONTENTS})

    MESSAGE (STATUS "Cofiguring file to ${EXECUTABLE_OUTPUT_PATH}/${OSX_BUNDLE_CONTENTS}")
	configure_file("${CMAKE_SOURCE_DIR}/data/osx/Info.plist" "${CMAKE_BINARY_DIR}")
	INSTALL(FILES "${CMAKE_BINARY_DIR}/Info.plist" DESTINATION ${OSX_BUNDLE_CONTENTS})
	
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (RESOURCES_DESTINATION ${EXECUTABLE_OUTPUT_PATH})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/data/Resources DESTINATION ${RESOURCES_DESTINATION})
