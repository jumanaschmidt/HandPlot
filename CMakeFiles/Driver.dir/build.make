# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jumana/Documents/HandPlot

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jumana/Documents/HandPlot

# Include any dependencies generated for this target.
include CMakeFiles/Driver.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Driver.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Driver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Driver.dir/flags.make

CMakeFiles/Driver.dir/morphologica-1.0/dongle/driver.cpp.o: CMakeFiles/Driver.dir/flags.make
CMakeFiles/Driver.dir/morphologica-1.0/dongle/driver.cpp.o: morphologica-1.0/dongle/driver.cpp
CMakeFiles/Driver.dir/morphologica-1.0/dongle/driver.cpp.o: CMakeFiles/Driver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jumana/Documents/HandPlot/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Driver.dir/morphologica-1.0/dongle/driver.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Driver.dir/morphologica-1.0/dongle/driver.cpp.o -MF CMakeFiles/Driver.dir/morphologica-1.0/dongle/driver.cpp.o.d -o CMakeFiles/Driver.dir/morphologica-1.0/dongle/driver.cpp.o -c /home/jumana/Documents/HandPlot/morphologica-1.0/dongle/driver.cpp

CMakeFiles/Driver.dir/morphologica-1.0/dongle/driver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Driver.dir/morphologica-1.0/dongle/driver.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jumana/Documents/HandPlot/morphologica-1.0/dongle/driver.cpp > CMakeFiles/Driver.dir/morphologica-1.0/dongle/driver.cpp.i

CMakeFiles/Driver.dir/morphologica-1.0/dongle/driver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Driver.dir/morphologica-1.0/dongle/driver.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jumana/Documents/HandPlot/morphologica-1.0/dongle/driver.cpp -o CMakeFiles/Driver.dir/morphologica-1.0/dongle/driver.cpp.s

# Object files for target Driver
Driver_OBJECTS = \
"CMakeFiles/Driver.dir/morphologica-1.0/dongle/driver.cpp.o"

# External object files for target Driver
Driver_EXTERNAL_OBJECTS =

Driver: CMakeFiles/Driver.dir/morphologica-1.0/dongle/driver.cpp.o
Driver: CMakeFiles/Driver.dir/build.make
Driver: CMakeFiles/Driver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jumana/Documents/HandPlot/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Driver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Driver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Driver.dir/build: Driver
.PHONY : CMakeFiles/Driver.dir/build

CMakeFiles/Driver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Driver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Driver.dir/clean

CMakeFiles/Driver.dir/depend:
	cd /home/jumana/Documents/HandPlot && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jumana/Documents/HandPlot /home/jumana/Documents/HandPlot /home/jumana/Documents/HandPlot /home/jumana/Documents/HandPlot /home/jumana/Documents/HandPlot/CMakeFiles/Driver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Driver.dir/depend

