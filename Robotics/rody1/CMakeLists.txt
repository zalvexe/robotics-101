cmake_minimum_required(VERSION 3.0.2)
project(rody1)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++17)

find_path(LIBUSBP_INCLUDE_DIR
        NAMES libusbp.hpp
        PATH_SUFFIXES "include" "libusbp" "libusbp-1")

find_library(LIBUSBP_LIBRARY
        NAMES libusbp-1.so)

message(STATUS "LIBUSBP_INCLUDE_DIR: ${LIBUSBP_INCLUDE_DIR}")

find_package(catkin REQUIRED COMPONENTS
      cmake_modules
      sensor_msgs
      roscpp
      std_msgs
      # message_generation
      rody1_messages
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

catkin_package(
  CATKIN_DEPENDS roscpp std_msgs rody1_messages
)

# add_message_files(
#   FILES
#   # controller.msg
#   Controller.msg
# )

# generate_messages(
#   DEPENDENCIES
#   std_msgs

# )

# catkin_package(
#   CATKIN_DEPENDS roscpp std_msgs rody1_messages
# )

add_executable(serial_node src/serial_node/serial.cpp)
# add_executable(main_node src/main_node/main.cpp src/main_node/move.cpp src/main_node/sequence.cpp)
add_executable(main_node src/main_node/main.cpp)
add_executable(controller_node src/controller_node/controller.cpp)

include_directories (
  "${CMAKE_SOURCE_DIR}/include"
)

target_include_directories(serial_node PRIVATE
  ${LIBUSBP_INCLUDE_DIR}
  include
)

target_include_directories(controller_node PRIVATE
  ${LIBUSBP_INCLUDE_DIR}
  include
)

target_link_libraries(serial_node PRIVATE
  ${catkin_LIBRARIES}
  ${LIBUSBP_LIBRARY}
)

target_link_libraries(controller_node PRIVATE
  ${catkin_LIBRARIES}
  ${LIBUSBP_LIBRARY}
)

target_include_directories(main_node PRIVATE
  ${LIBUSBP_INCLUDE_DIR}
  include
)

target_link_libraries(main_node PRIVATE
  ${catkin_LIBRARIES}
  ${LIBUSBP_LIBRARY}
)
# The following lines are commented out because the corresponding executables are also commented out.
# Uncomment these lines if you uncomment the corresponding add_executable lines.
# target_link_libraries(main_node ${catkin_LIBRARIES})
# add_dependencies(main_node romusa_messages_generate_messages_cpp)

# add_dependencies(serial_node rody1_generate_messages_cpp)
# # add_dependencies(controller_node rody1_generate_messages_cpp)
# add_dependencies(main_node rody1_generate_messages_cpp)

add_dependencies(main_node ${catkin_EXPORTED_TARGETS})
add_dependencies(serial_node ${catkin_EXPORTED_TARGETS})
add_dependencies(controller_node ${catkin_EXPORTED_TARGETS})
