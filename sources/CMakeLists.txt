file(GLOB_RECURSE source src/*.cc)
file(GLOB_RECURSE includes include/*.hh)

# Build the library.
add_library(d2tb SHARED ${source})

target_include_directories(d2tb PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}../io>"
"$<INSTALL_INTERFACE:include>")

target_link_libraries(d2tb PUBLIC
root_io ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

# Install the library for edep-sim
install(TARGETS d2tb
  LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/lib
  RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
INCLUDES DESTINATION ${PROJECT_SOURCE_DIR}/include)

# Install the header files.
install(FILES ${includes} DESTINATION ${PROJECT_SOURCE_DIR}/include)
