// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from ros2_serial_interfaces:msg/SerialString.idl
// generated code does not contain a copyright notice

#ifndef ROS2_SERIAL_INTERFACES__MSG__DETAIL__SERIAL_STRING__STRUCT_HPP_
#define ROS2_SERIAL_INTERFACES__MSG__DETAIL__SERIAL_STRING__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__ros2_serial_interfaces__msg__SerialString __attribute__((deprecated))
#else
# define DEPRECATED__ros2_serial_interfaces__msg__SerialString __declspec(deprecated)
#endif

namespace ros2_serial_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct SerialString_
{
  using Type = SerialString_<ContainerAllocator>;

  explicit SerialString_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->data = "";
    }
  }

  explicit SerialString_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    data(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->data = "";
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _data_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _data_type data;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__data(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->data = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ros2_serial_interfaces::msg::SerialString_<ContainerAllocator> *;
  using ConstRawPtr =
    const ros2_serial_interfaces::msg::SerialString_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ros2_serial_interfaces::msg::SerialString_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ros2_serial_interfaces::msg::SerialString_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ros2_serial_interfaces::msg::SerialString_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ros2_serial_interfaces::msg::SerialString_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ros2_serial_interfaces::msg::SerialString_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ros2_serial_interfaces::msg::SerialString_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ros2_serial_interfaces::msg::SerialString_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ros2_serial_interfaces::msg::SerialString_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ros2_serial_interfaces__msg__SerialString
    std::shared_ptr<ros2_serial_interfaces::msg::SerialString_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ros2_serial_interfaces__msg__SerialString
    std::shared_ptr<ros2_serial_interfaces::msg::SerialString_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SerialString_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->data != other.data) {
      return false;
    }
    return true;
  }
  bool operator!=(const SerialString_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SerialString_

// alias to use template instance with default allocator
using SerialString =
  ros2_serial_interfaces::msg::SerialString_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace ros2_serial_interfaces

#endif  // ROS2_SERIAL_INTERFACES__MSG__DETAIL__SERIAL_STRING__STRUCT_HPP_
