cmake_minimum_required(VERSION 3.0.2)
project(rody1_messages)

find_package(catkin REQUIRED COMPONENTS
  # geometry_msgs
  message_generation
  roscpp
  std_msgs
)

add_message_files(
  FILES
  Controller.msg
)

generate_messages(
  DEPENDENCIES
  # geometry_msgs
  std_msgs
)


catkin_package(
  # INCLUDE_DIRS include
  CATKIN_DEPENDS message_runtime
)

include_directories(
include
  ${catkin_INCLUDE_DIRS}
)