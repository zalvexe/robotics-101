// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from ros2_serial_interfaces:msg/SerialString.idl
// generated code does not contain a copyright notice
#include "ros2_serial_interfaces/msg/detail/serial_string__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `data`
#include "rosidl_runtime_c/string_functions.h"

bool
ros2_serial_interfaces__msg__SerialString__init(ros2_serial_interfaces__msg__SerialString * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    ros2_serial_interfaces__msg__SerialString__fini(msg);
    return false;
  }
  // data
  if (!rosidl_runtime_c__String__init(&msg->data)) {
    ros2_serial_interfaces__msg__SerialString__fini(msg);
    return false;
  }
  return true;
}

void
ros2_serial_interfaces__msg__SerialString__fini(ros2_serial_interfaces__msg__SerialString * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // data
  rosidl_runtime_c__String__fini(&msg->data);
}

bool
ros2_serial_interfaces__msg__SerialString__are_equal(const ros2_serial_interfaces__msg__SerialString * lhs, const ros2_serial_interfaces__msg__SerialString * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // data
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->data), &(rhs->data)))
  {
    return false;
  }
  return true;
}

bool
ros2_serial_interfaces__msg__SerialString__copy(
  const ros2_serial_interfaces__msg__SerialString * input,
  ros2_serial_interfaces__msg__SerialString * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // data
  if (!rosidl_runtime_c__String__copy(
      &(input->data), &(output->data)))
  {
    return false;
  }
  return true;
}

ros2_serial_interfaces__msg__SerialString *
ros2_serial_interfaces__msg__SerialString__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ros2_serial_interfaces__msg__SerialString * msg = (ros2_serial_interfaces__msg__SerialString *)allocator.allocate(sizeof(ros2_serial_interfaces__msg__SerialString), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(ros2_serial_interfaces__msg__SerialString));
  bool success = ros2_serial_interfaces__msg__SerialString__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
ros2_serial_interfaces__msg__SerialString__destroy(ros2_serial_interfaces__msg__SerialString * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    ros2_serial_interfaces__msg__SerialString__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
ros2_serial_interfaces__msg__SerialString__Sequence__init(ros2_serial_interfaces__msg__SerialString__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ros2_serial_interfaces__msg__SerialString * data = NULL;

  if (size) {
    data = (ros2_serial_interfaces__msg__SerialString *)allocator.zero_allocate(size, sizeof(ros2_serial_interfaces__msg__SerialString), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = ros2_serial_interfaces__msg__SerialString__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        ros2_serial_interfaces__msg__SerialString__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
ros2_serial_interfaces__msg__SerialString__Sequence__fini(ros2_serial_interfaces__msg__SerialString__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      ros2_serial_interfaces__msg__SerialString__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

ros2_serial_interfaces__msg__SerialString__Sequence *
ros2_serial_interfaces__msg__SerialString__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ros2_serial_interfaces__msg__SerialString__Sequence * array = (ros2_serial_interfaces__msg__SerialString__Sequence *)allocator.allocate(sizeof(ros2_serial_interfaces__msg__SerialString__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = ros2_serial_interfaces__msg__SerialString__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
ros2_serial_interfaces__msg__SerialString__Sequence__destroy(ros2_serial_interfaces__msg__SerialString__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    ros2_serial_interfaces__msg__SerialString__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
ros2_serial_interfaces__msg__SerialString__Sequence__are_equal(const ros2_serial_interfaces__msg__SerialString__Sequence * lhs, const ros2_serial_interfaces__msg__SerialString__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!ros2_serial_interfaces__msg__SerialString__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
ros2_serial_interfaces__msg__SerialString__Sequence__copy(
  const ros2_serial_interfaces__msg__SerialString__Sequence * input,
  ros2_serial_interfaces__msg__SerialString__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(ros2_serial_interfaces__msg__SerialString);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    ros2_serial_interfaces__msg__SerialString * data =
      (ros2_serial_interfaces__msg__SerialString *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!ros2_serial_interfaces__msg__SerialString__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          ros2_serial_interfaces__msg__SerialString__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!ros2_serial_interfaces__msg__SerialString__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
