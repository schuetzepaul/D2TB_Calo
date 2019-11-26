cmake_minimum_required(VERSION 3.0)

project(D2TB_Calo VERSION 1.0.0)

SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_INSTALL_RPATH "${PROJECT_SOURCE_DIR}/lib")

SET(CMAKE_CXX_FLAGS "-W -Wall -pedantic -Wno-non-virtual-dtor -Wno-long-long -Wwrite-strings -Wpointer-arith -Wno-variadic-macros -Wshadow -pipe -DG4USE_STD11 -D_LIBCPP_ENABLE_CXX17_REMOVED_FEATURES=1 -std=c++1z")

MESSAGE(STATUS "CXX Flags:=${CMAKE_CXX_FLAGS}")

#----------------------------------------------------------------------------
# Find ROOT package
#
FIND_PACKAGE(ROOT REQUIRED COMPONENTS Geom Physics Matrix MathCore Tree RIO)
IF(ROOT_FOUND)
  INCLUDE(${ROOT_USE_FILE})
ENDIF(ROOT_FOUND)

#----------------------------------------------------------------------------
# Find Geant4 package
FIND_PACKAGE(Geant4 10.0 REQUIRED ui_all vis_all)
IF(Geant4_FOUND)
  INCLUDE(${Geant4_USE_FILE})
ENDIF(Geant4_FOUND)

#IO
add_subdirectory(io)

#src
add_subdirectory(sources)

#app
add_subdirectory(main)
