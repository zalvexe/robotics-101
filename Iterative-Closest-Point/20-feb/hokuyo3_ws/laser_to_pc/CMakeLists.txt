cmake_minimum_required(VERSION 3.8)
project(laser_to_pc)

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(laser_geometry REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(PCL REQUIRED COMPONENTS common io)
find_package(Boost REQUIRED COMPONENTS system)
find_package(romusa2_messages REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)

include_directories(
  ${rclcpp_INCLUDE_DIRS}
  ${sensor_msgs_INCLUDE_DIRS}
  ${laser_geometry_INCLUDE_DIRS}
  ${pcl_conversions_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${romusa2_messages_INCLUDE_DIRS}
  ${geometry_msgs_INCLUDE_DIRS}
  ${tf2_INCLUDE_DIRS}
  ${tf2_geometry_msgs_INCLUDE_DIRS}
  ${nav_msgs_INCLUDE_DIRS}
)

add_executable(laser_to_pointcloud src/laser_to_pointcloud.cpp)
add_executable(icp_publish src/icp_publish.cpp)
add_executable(fixed_map_rviz src/fixed_map_rviz.cpp)
add_executable(fuse_odom_lidar src/fuse_odom_lidar.cpp)

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

ament_target_dependencies(icp_publish
  rclcpp
  sensor_msgs
  laser_geometry
  pcl_conversions 
  PCL
  geometry_msgs
  romusa2_messages
)

ament_target_dependencies(fuse_odom_lidar
  rclcpp
  nav_msgs
  geometry_msgs
  tf2_geometry_msgs
  tf2
  romusa2_messages
)

# Install executables
install(TARGETS laser_to_pointcloud
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS fixed_map_rviz
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS icp_publish
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS fuse_odom_lidar
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()