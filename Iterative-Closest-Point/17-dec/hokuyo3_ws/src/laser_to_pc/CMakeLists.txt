cmake_minimum_required(VERSION 3.8)
project(laser_to_pc)

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(laser_geometry REQUIRED)
# find_package(PCL REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(PCL REQUIRED COMPONENTS <components>)
# Include directories
include_directories(
  # include
  ${rclcpp_INCLUDE_DIRS}
  ${sensor_msgs_INCLUDE_DIRS}
  ${laser_geometry_INCLUDE_DIRS}
  ${pcl_conversions_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
)

# Create executable
add_executable(laser_to_pointcloud src/laser_to_pointcloud.cpp)
add_executable(fixed_map_rviz src/fixed_map_rviz.cpp)

# Link libraries
ament_target_dependencies(laser_to_pointcloud
  rclcpp
  sensor_msgs
  laser_geometry
  pcl_conversions 
  PCL
)
ament_target_dependencies(fixed_map_rviz
  rclcpp
  sensor_msgs
  laser_geometry
  pcl_conversions 
  PCL
)
# Install executable
install(TARGETS laser_to_pointcloud
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS fixed_map_rviz
  DESTINATION lib/${PROJECT_NAME}
)
ament_package()


