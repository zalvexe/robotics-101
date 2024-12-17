# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_laser_to_pc_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED laser_to_pc_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(laser_to_pc_FOUND FALSE)
  elseif(NOT laser_to_pc_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(laser_to_pc_FOUND FALSE)
  endif()
  return()
endif()
set(_laser_to_pc_CONFIG_INCLUDED TRUE)

# output package information
if(NOT laser_to_pc_FIND_QUIETLY)
  message(STATUS "Found laser_to_pc: 0.0.0 (${laser_to_pc_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'laser_to_pc' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${laser_to_pc_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(laser_to_pc_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${laser_to_pc_DIR}/${_extra}")
endforeach()
