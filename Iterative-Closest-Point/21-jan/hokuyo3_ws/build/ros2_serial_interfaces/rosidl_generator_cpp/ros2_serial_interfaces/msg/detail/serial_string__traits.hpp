// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from ros2_serial_interfaces:msg/SerialString.idl
// generated code does not contain a copyright notice

#ifndef ROS2_SERIAL_INTERFACES__MSG__DETAIL__SERIAL_STRING__TRAITS_HPP_
#define ROS2_SERIAL_INTERFACES__MSG__DETAIL__SERIAL_STRING__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "ros2_serial_interfaces/msg/detail/serial_string__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace ros2_serial_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const SerialString & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: data
  {
    out << "data: ";
    rosidl_generator_traits::value_to_yaml(msg.data, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SerialString & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: data
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "data: ";
    rosidl_generator_traits::value_to_yaml(msg.data, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SerialString & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace ros2_serial_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use ros2_serial_interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const ros2_serial_interfaces::msg::SerialString & msg,
  std::ostream & out, size_t indentation = 0)
{
  ros2_serial_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use ros2_serial_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const ros2_serial_interfaces::msg::SerialString & msg)
{
  return ros2_serial_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<ros2_serial_interfaces::msg::SerialString>()
{
  return "ros2_serial_interfaces::msg::SerialString";
}

template<>
inline const char * name<ros2_serial_interfaces::msg::SerialString>()
{
  return "ros2_serial_interfaces/msg/SerialString";
}

template<>
struct has_fixed_size<ros2_serial_interfaces::msg::SerialString>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<ros2_serial_interfaces::msg::SerialString>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<ros2_serial_interfaces::msg::SerialString>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // ROS2_SERIAL_INTERFACES__MSG__DETAIL__SERIAL_STRING__TRAITS_HPP_
