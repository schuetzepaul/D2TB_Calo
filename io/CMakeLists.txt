# Configure the dependencies
find_package(ROOT REQUIRED
COMPONENTS Geom Physics Matrix MathCore Tree RIO)
if(ROOT_FOUND)
  include(${ROOT_USE_FILE})
endif(ROOT_FOUND)

set(source
  TG4PhotonDetHit.cxx
TG4Event.cxx)

set(includes
  TG4PhotonDetHit.hh
TG4Event.hh)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ROOT_GENERATE_DICTIONARY(G__root_io
  TG4PhotonDetHit.hh TG4Event.hh
  OPTIONS -inlineInputHeader
LINKDEF LinkDef.hh)

# Build the library.
add_library(root_io SHARED ${source} G__root_io.cxx)

target_include_directories(root_io PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
"$<INSTALL_INTERFACE:include>")

target_link_libraries(root_io PUBLIC ${ROOT_LIBRARIES})

# Install the library for edep-sim
install(TARGETS root_io
  LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/lib
  RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
INCLUDES DESTINATION ${PROJECT_SOURCE_DIR}/include )

# Install the header files.
install(FILES ${includes} DESTINATION ${PROJECT_SOURCE_DIR}/include)

# If this is ROOT6 or later, then install the rootmap and pcm files.
if(${ROOT_VERSION} VERSION_GREATER 6)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libroot_io.rootmap
  DESTINATION ${PROJECT_SOURCE_DIR}/lib)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libroot_io_rdict.pcm
  DESTINATION ${PROJECT_SOURCE_DIR}/lib)
endif(${ROOT_VERSION} VERSION_GREATER 6)
