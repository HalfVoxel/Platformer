# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/aron/programming/cpp/Platformer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aron/programming/cpp/Platformer

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running interactive CMake command-line interface..."
	/usr/bin/cmake -i .
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: install/local
.PHONY : install/local/fast

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: install/strip
.PHONY : install/strip/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components
.PHONY : list_install_components/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/aron/programming/cpp/Platformer/CMakeFiles /home/aron/programming/cpp/Platformer/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/aron/programming/cpp/Platformer/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named Platformer

# Build rule for target.
Platformer: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 Platformer
.PHONY : Platformer

# fast build rule for target.
Platformer/fast:
	$(MAKE) -f CMakeFiles/Platformer.dir/build.make CMakeFiles/Platformer.dir/build
.PHONY : Platformer/fast

src/PhysicsSprite.o: src/PhysicsSprite.cpp.o
.PHONY : src/PhysicsSprite.o

# target to build an object file
src/PhysicsSprite.cpp.o:
	$(MAKE) -f CMakeFiles/Platformer.dir/build.make CMakeFiles/Platformer.dir/src/PhysicsSprite.cpp.o
.PHONY : src/PhysicsSprite.cpp.o

src/PhysicsSprite.i: src/PhysicsSprite.cpp.i
.PHONY : src/PhysicsSprite.i

# target to preprocess a source file
src/PhysicsSprite.cpp.i:
	$(MAKE) -f CMakeFiles/Platformer.dir/build.make CMakeFiles/Platformer.dir/src/PhysicsSprite.cpp.i
.PHONY : src/PhysicsSprite.cpp.i

src/PhysicsSprite.s: src/PhysicsSprite.cpp.s
.PHONY : src/PhysicsSprite.s

# target to generate assembly for a file
src/PhysicsSprite.cpp.s:
	$(MAKE) -f CMakeFiles/Platformer.dir/build.make CMakeFiles/Platformer.dir/src/PhysicsSprite.cpp.s
.PHONY : src/PhysicsSprite.cpp.s

src/ResourceManager.o: src/ResourceManager.cpp.o
.PHONY : src/ResourceManager.o

# target to build an object file
src/ResourceManager.cpp.o:
	$(MAKE) -f CMakeFiles/Platformer.dir/build.make CMakeFiles/Platformer.dir/src/ResourceManager.cpp.o
.PHONY : src/ResourceManager.cpp.o

src/ResourceManager.i: src/ResourceManager.cpp.i
.PHONY : src/ResourceManager.i

# target to preprocess a source file
src/ResourceManager.cpp.i:
	$(MAKE) -f CMakeFiles/Platformer.dir/build.make CMakeFiles/Platformer.dir/src/ResourceManager.cpp.i
.PHONY : src/ResourceManager.cpp.i

src/ResourceManager.s: src/ResourceManager.cpp.s
.PHONY : src/ResourceManager.s

# target to generate assembly for a file
src/ResourceManager.cpp.s:
	$(MAKE) -f CMakeFiles/Platformer.dir/build.make CMakeFiles/Platformer.dir/src/ResourceManager.cpp.s
.PHONY : src/ResourceManager.cpp.s

src/ResourcePath.o: src/ResourcePath.cpp.o
.PHONY : src/ResourcePath.o

# target to build an object file
src/ResourcePath.cpp.o:
	$(MAKE) -f CMakeFiles/Platformer.dir/build.make CMakeFiles/Platformer.dir/src/ResourcePath.cpp.o
.PHONY : src/ResourcePath.cpp.o

src/ResourcePath.i: src/ResourcePath.cpp.i
.PHONY : src/ResourcePath.i

# target to preprocess a source file
src/ResourcePath.cpp.i:
	$(MAKE) -f CMakeFiles/Platformer.dir/build.make CMakeFiles/Platformer.dir/src/ResourcePath.cpp.i
.PHONY : src/ResourcePath.cpp.i

src/ResourcePath.s: src/ResourcePath.cpp.s
.PHONY : src/ResourcePath.s

# target to generate assembly for a file
src/ResourcePath.cpp.s:
	$(MAKE) -f CMakeFiles/Platformer.dir/build.make CMakeFiles/Platformer.dir/src/ResourcePath.cpp.s
.PHONY : src/ResourcePath.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) -f CMakeFiles/Platformer.dir/build.make CMakeFiles/Platformer.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) -f CMakeFiles/Platformer.dir/build.make CMakeFiles/Platformer.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) -f CMakeFiles/Platformer.dir/build.make CMakeFiles/Platformer.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

src/world.o: src/world.cpp.o
.PHONY : src/world.o

# target to build an object file
src/world.cpp.o:
	$(MAKE) -f CMakeFiles/Platformer.dir/build.make CMakeFiles/Platformer.dir/src/world.cpp.o
.PHONY : src/world.cpp.o

src/world.i: src/world.cpp.i
.PHONY : src/world.i

# target to preprocess a source file
src/world.cpp.i:
	$(MAKE) -f CMakeFiles/Platformer.dir/build.make CMakeFiles/Platformer.dir/src/world.cpp.i
.PHONY : src/world.cpp.i

src/world.s: src/world.cpp.s
.PHONY : src/world.s

# target to generate assembly for a file
src/world.cpp.s:
	$(MAKE) -f CMakeFiles/Platformer.dir/build.make CMakeFiles/Platformer.dir/src/world.cpp.s
.PHONY : src/world.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... Platformer"
	@echo "... edit_cache"
	@echo "... install"
	@echo "... install/local"
	@echo "... install/strip"
	@echo "... list_install_components"
	@echo "... rebuild_cache"
	@echo "... src/PhysicsSprite.o"
	@echo "... src/PhysicsSprite.i"
	@echo "... src/PhysicsSprite.s"
	@echo "... src/ResourceManager.o"
	@echo "... src/ResourceManager.i"
	@echo "... src/ResourceManager.s"
	@echo "... src/ResourcePath.o"
	@echo "... src/ResourcePath.i"
	@echo "... src/ResourcePath.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/world.o"
	@echo "... src/world.i"
	@echo "... src/world.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

