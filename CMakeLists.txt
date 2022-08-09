#
# This is an example CMakeLists.txt file to compile a program with morphologica headers
#

# cmake version 3.1 provides the set(CMAKE_CXX_STANDARD 17) feature
cmake_minimum_required(VERSION 3.1)

# Give your project a name
project(HandGraph)

# From CMAKE_SYSTEM work out which of __OSX__, __GLN__, __NIX__ are required
if(CMAKE_SYSTEM MATCHES Linux.*)
  set(OS_FLAG "-D__GLN__")
elseif(CMAKE_SYSTEM MATCHES BSD.*)
  set(OS_FLAG "-D__NIX__")
elseif(APPLE)
  set(OS_FLAG "-D__OSX__")
else()
  message(ERROR "Operating system not supported: " ${CMAKE_SYSTEM})
endif()

# morphologica uses c++-17 language features
set(CMAKE_CXX_STANDARD 17)

# Add the host definition to CXXFLAGS along with other switches, depending on OS/Compiler
if (APPLE)
  set(CMAKE_CXX_FLAGS "${OS_FLAG} -Wall -Wfatal-errors -Wno-missing-braces -g -O3")
else()
  # This assumes a gcc compiler (or a gcc mimic like Clang)
  set(CMAKE_CXX_FLAGS "${OS_FLAG} -lft4222 -Wl,-rpath,/usr/local/lib -Wall -Wfatal-errors -Wno-missing-braces -g -O3 -Wno-unused-result -Wno-unknown-pragmas -march=native")
endif()

# Additional GL compiler flags.
set(OpenGL_GL_PREFERENCE "GLVND") # Following `cmake --help-policy CMP0072`
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGL3_PROTOTYPES -DGL_GLEXT_PROTOTYPES")
if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGL_SILENCE_DEPRECATION")
endif()

# Tell the program where the morph fonts are (in morphologica-1.0 that you just downloaded):
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMORPH_FONTS_DIR=\"\\\"${PROJECT_SOURCE_DIR}/morphologica-1.0/fonts\\\"\"")

# Find the 3 dependency libraries which are needed to draw graphs
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(Freetype REQUIRED)
#find_package (LibFT4222 REQUIRED)

# Tell compiler where to find the included dependencies
include_directories(${OpenCV_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})

# Tell compiler where to find the morphologica headers
set(MORPH_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/morphologica-1.0" CACHE PATH "The path to morphologica")
include_directories(BEFORE ${MORPH_INCLUDE_PATH}/include) # Allows GL3/gl3.h to be found
include_directories(BEFORE ${MORPH_INCLUDE_PATH})         # Allows morph/Header.h to be found\
#include_directories(BEFORE ${MORPH_INCLUDE_PATH}/dongle)
#include_directories(BEFORE /usr/include)
include_directories(${PROJECT_SOURCE_DIR}/LibFT4222/lib/amd64)
include_directories(${PROJECT_SOURCE_DIR}/ftd2xx/amd64)
#ADD_LIBRARY(LibsModule HandPlot.cpp dongle/driver.cpp)

#target_link_libraries(LibsModule /usr/include)

# Our program will be written in a single file called graph2d.cpp and compiled into the exe graph2d:
#add_executable(Driver ${MORPH_INCLUDE_PATH}/dongle/driver.cpp)
add_executable(HandPlot HandPlot.cpp)


# We need to tell the compiler to link the OpenGL and Freetype libraries to the graph2d executable
target_link_libraries(HandPlot OpenGL::GL Freetype::Freetype glfw)
target_link_libraries(HandPlot ${PROJECT_SOURCE_DIR}/libft4222.a)

#target_link_libraries(HandPlot ftd2xx::ftd2xx)
#target_link_libraries(HandPlot LibFT422::LibFT422)


