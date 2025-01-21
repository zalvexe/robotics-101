cmake_minimum_required(VERSION 3.5)
project(icp_node)

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(PCL REQUIRED COMPONENTS <components>)

include_directories(
  ${rclcpp_INCLUDE_DIRS}
  ${sensor_msgs_INCLUDE_DIRS}
  ${nav_msgs_INCLUDE_DIRS}
  ${geometry_msgs_INCLUDE_DIRS}
  ${pcl_conversions_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
)

add_executable(icp_node src/icp_node.cpp)
# add_executable(scan_localizer2 src/scan_localizer2.cpp)
# add_executable(map_builder_node src/map_builder.cpp)

ament_target_dependencies(icp_node rclcpp sensor_msgs nav_msgs geometry_msgs pcl_conversions PCL)
# ament_target_dependencies(scan_localizer2 rclcpp sensor_msgs nav_msgs geometry_msgs pcl_conversions PCL)
# install(TARGETS
#   map_builder_node
#   DESTINATION lib/${PROJECT_NAME}
# )

install(TARGETS
  icp_node
  DESTINATION lib/${PROJECT_NAME})


# install(TARGETS
#   scan_localizer2
#   DESTINATION lib/${PROJECT_NAME})

ament_export_include_directories(include)

ament_package()