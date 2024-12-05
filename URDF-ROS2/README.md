# ROS 2-JAZZY SETTING UP  

Making workspace  
```mkdir ros2_ws/src```   

Making package   
```ros2 pkg create --build-type ament_cmake <pkg name>```   

Sourcing  
```source ./install/setup.bash```  

Build  
```colcon build```  

Run  
```ros2 run pkg_name node_name```

## EXAMPLE CODE
```talker.cpp```  
```cpp
#include <sstream>
// #include "ros/ros.h"
#include "rclcpp/rclcpp.hpp"
// #include "std_msgs/String.h"
#include "std_msgs/msg/string.hpp"
int main(int argc, char **argv)
{
//  ros::init(argc, argv, "talker");
//  ros::NodeHandle n;
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("talker");
//  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
//  ros::Rate loop_rate(10);
  auto chatter_pub = node->create_publisher<std_msgs::msg::String>("chatter", 1000);
  rclcpp::Rate loop_rate(10);
  int count = 0;
//  std_msgs::String msg;
  std_msgs::msg::String msg;
//  while (ros::ok())
  while (rclcpp::ok())
  {
    std::stringstream ss;
    ss << "hello world " << count++;
    msg.data = ss.str();
//    ROS_INFO("%s", msg.data.c_str());
    RCLCPP_INFO(node->get_logger(), "%s\n", msg.data.c_str());
    chatter_pub->publish(msg);
//    ros::spinOnce();
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  return 0;
}
```

```package.xml```  
```xml
<?xml version="1.0"?>
<?xml-model href="http://download.ros.org/schema/package_format3.xsd" schematypens="http://www.w3.org/2001/XMLSchema"?>
<package format="2">
  <name>first_package</name>
  <version>0.0.0</version>
  <description>TODO: Package description</description>
  <maintainer email="zalv@todo.todo">zalv</maintainer>
  <license>TODO: License declaration</license>

  <buildtool_depend>ament_cmake</buildtool_depend>

  <test_depend>ament_lint_auto</test_depend>
  <test_depend>ament_lint_common</test_depend>
  <depend>rclcpp</depend>
  <depend>std_msgs</depend>
  <export>
    <build_type>ament_cmake</build_type>
  </export>
</package>
```
```CMakeLists.txt```  
```cmake
cmake_minimum_required(VERSION 3.8)
project(first_package)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
include_directories(include)  
add_executable(talker src/talker.cpp)
ament_target_dependencies(talker
  rclcpp
  std_msgs)

install(TARGETS talker
  DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY include/
  DESTINATION include)

ament_export_include_directories(include)
ament_export_dependencies(std_msgs)
# if(BUILD_TESTING)
#   find_package(ament_lint_auto REQUIRED)
#   # the following line skips the linter which checks for copyrights
#   # comment the line when a copyright and license is added to all source files
#   set(ament_cmake_copyright_FOUND TRUE)
#   # the following line skips cpplint (only works in a git repo)
#   # comment the line when this package is in a git repo and when
#   # a copyright and license is added to all source files
#   set(ament_cmake_cpplint_FOUND TRUE)
#   ament_lint_auto_find_test_dependencies()
# endif()

ament_package()
```
