cmake_minimum_required(VERSION 3.8)
project(laser_to_pc)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(laser_geometry REQUIRED)

# Include directories
include_directories(
  include
)

# Create executable
add_executable(laser_to_pointcloud src/laser_to_pointcloud.cpp)

# Link libraries
ament_target_dependencies(laser_to_pointcloud
  rclcpp
  sensor_msgs
  laser_geometry
)

# Install executable
install(TARGETS laser_to_pointcloud
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()


