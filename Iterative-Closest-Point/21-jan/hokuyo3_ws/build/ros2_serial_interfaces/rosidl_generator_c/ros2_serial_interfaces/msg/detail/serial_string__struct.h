// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from ros2_serial_interfaces:msg/SerialString.idl
// generated code does not contain a copyright notice

#ifndef ROS2_SERIAL_INTERFACES__MSG__DETAIL__SERIAL_STRING__STRUCT_H_
#define ROS2_SERIAL_INTERFACES__MSG__DETAIL__SERIAL_STRING__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'data'
#include "rosidl_runtime_c/string.h"

/// Struct defined in msg/SerialString in the package ros2_serial_interfaces.
typedef struct ros2_serial_interfaces__msg__SerialString
{
  std_msgs__msg__Header header;
  rosidl_runtime_c__String data;
} ros2_serial_interfaces__msg__SerialString;

// Struct for a sequence of ros2_serial_interfaces__msg__SerialString.
typedef struct ros2_serial_interfaces__msg__SerialString__Sequence
{
  ros2_serial_interfaces__msg__SerialString * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ros2_serial_interfaces__msg__SerialString__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROS2_SERIAL_INTERFACES__MSG__DETAIL__SERIAL_STRING__STRUCT_H_
