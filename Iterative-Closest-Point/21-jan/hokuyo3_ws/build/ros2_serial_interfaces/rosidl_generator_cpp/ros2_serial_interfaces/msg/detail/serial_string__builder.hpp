// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ros2_serial_interfaces:msg/SerialString.idl
// generated code does not contain a copyright notice

#ifndef ROS2_SERIAL_INTERFACES__MSG__DETAIL__SERIAL_STRING__BUILDER_HPP_
#define ROS2_SERIAL_INTERFACES__MSG__DETAIL__SERIAL_STRING__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ros2_serial_interfaces/msg/detail/serial_string__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ros2_serial_interfaces
{

namespace msg
{

namespace builder
{

class Init_SerialString_data
{
public:
  explicit Init_SerialString_data(::ros2_serial_interfaces::msg::SerialString & msg)
  : msg_(msg)
  {}
  ::ros2_serial_interfaces::msg::SerialString data(::ros2_serial_interfaces::msg::SerialString::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ros2_serial_interfaces::msg::SerialString msg_;
};

class Init_SerialString_header
{
public:
  Init_SerialString_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SerialString_data header(::ros2_serial_interfaces::msg::SerialString::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_SerialString_data(msg_);
  }

private:
  ::ros2_serial_interfaces::msg::SerialString msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::ros2_serial_interfaces::msg::SerialString>()
{
  return ros2_serial_interfaces::msg::builder::Init_SerialString_header();
}

}  // namespace ros2_serial_interfaces

#endif  // ROS2_SERIAL_INTERFACES__MSG__DETAIL__SERIAL_STRING__BUILDER_HPP_
