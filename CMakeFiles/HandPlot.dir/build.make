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
include CMakeFiles/HandPlot.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/HandPlot.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/HandPlot.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HandPlot.dir/flags.make

CMakeFiles/HandPlot.dir/HandPlot.cpp.o: CMakeFiles/HandPlot.dir/flags.make
CMakeFiles/HandPlot.dir/HandPlot.cpp.o: HandPlot.cpp
CMakeFiles/HandPlot.dir/HandPlot.cpp.o: CMakeFiles/HandPlot.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jumana/Documents/HandPlot/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/HandPlot.dir/HandPlot.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HandPlot.dir/HandPlot.cpp.o -MF CMakeFiles/HandPlot.dir/HandPlot.cpp.o.d -o CMakeFiles/HandPlot.dir/HandPlot.cpp.o -c /home/jumana/Documents/HandPlot/HandPlot.cpp

CMakeFiles/HandPlot.dir/HandPlot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HandPlot.dir/HandPlot.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jumana/Documents/HandPlot/HandPlot.cpp > CMakeFiles/HandPlot.dir/HandPlot.cpp.i

CMakeFiles/HandPlot.dir/HandPlot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HandPlot.dir/HandPlot.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jumana/Documents/HandPlot/HandPlot.cpp -o CMakeFiles/HandPlot.dir/HandPlot.cpp.s

# Object files for target HandPlot
HandPlot_OBJECTS = \
"CMakeFiles/HandPlot.dir/HandPlot.cpp.o"

# External object files for target HandPlot
HandPlot_EXTERNAL_OBJECTS =

HandPlot: CMakeFiles/HandPlot.dir/HandPlot.cpp.o
HandPlot: CMakeFiles/HandPlot.dir/build.make
HandPlot: /usr/lib/x86_64-linux-gnu/libfreetype.so
HandPlot: /usr/local/lib/libglfw3.a
HandPlot: libft4222.a
HandPlot: /usr/lib/x86_64-linux-gnu/libGLX.so
HandPlot: /usr/lib/x86_64-linux-gnu/libOpenGL.so
HandPlot: /usr/lib/x86_64-linux-gnu/librt.a
HandPlot: /usr/lib/x86_64-linux-gnu/libm.so
HandPlot: CMakeFiles/HandPlot.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jumana/Documents/HandPlot/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable HandPlot"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HandPlot.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HandPlot.dir/build: HandPlot
.PHONY : CMakeFiles/HandPlot.dir/build

CMakeFiles/HandPlot.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HandPlot.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HandPlot.dir/clean

CMakeFiles/HandPlot.dir/depend:
	cd /home/jumana/Documents/HandPlot && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jumana/Documents/HandPlot /home/jumana/Documents/HandPlot /home/jumana/Documents/HandPlot /home/jumana/Documents/HandPlot /home/jumana/Documents/HandPlot/CMakeFiles/HandPlot.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HandPlot.dir/depend

