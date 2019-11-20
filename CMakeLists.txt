#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(D2TB_Calo)

SET(CMAKE_INSTALL_RPATH "${PROJECT_SOURCE_DIR}/lib")

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
find_package(Geant4 REQUIRED ui_all vis_all)
else()
find_package(Geant4 REQUIRED)
endif()

message(STATUS "Geant4_CXXSTD=${Geant4_CXXSTD}")

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include
${ROOT_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-W -Wall -pedantic -Wno-non-virtual-dtor -Wno-long-long -Wwrite-strings -Wpointer-arith -Wno-variadic-macros -Wshadow -pipe -DG4USE_STD11 -D_LIBCPP_ENABLE_CXX17_REMOVED_FEATURES=1 -std=c++1z")
message(STATUS "CXX Flags:=${CMAKE_CXX_FLAGS}")

# Build the dictionary for the i/o classes.
ROOT_GENERATE_DICTIONARY(G__root_io LINKDEF include/LinkDef.hh)
# Build the library.
add_library(root_io SHARED G__root_io.cxx)
target_link_libraries(root_io PUBLIC ${ROOT_LIBRARIES})

# Install the library
install(TARGETS root_io
LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/lib
RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)

if(${ROOT_VERSION} VERSION_GREATER 6)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libroot_io.rootmap
DESTINATION  ${PROJECT_SOURCE_DIR}/lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libroot_io_rdict.pcm
DESTINATION  ${PROJECT_SOURCE_DIR}/lib)
endif(${ROOT_VERSION} VERSION_GREATER 6)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(D2TB_Calo main/D2TB_Calo.cc ${sources} ${headers})
target_link_libraries(D2TB_Calo ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} root_io)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS D2TB_Calo RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)
