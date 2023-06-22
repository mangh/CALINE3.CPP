# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.24)

##########################################################################
#
#   PROJECT
#

# VERSION default "2.0.0":
if(NOT DEFINED APP_VERSION)

  if(NOT DEFINED APP_VERSION_MAJOR)
    set(APP_VERSION_MAJOR 2)
  endif()
  if(NOT DEFINED APP_VERSION_MINOR)
    set(APP_VERSION_MINOR 0)
  endif()
  if(NOT DEFINED APP_VERSION_PATCH)
    set(APP_VERSION_PATCH 0)
  endif()

  set(APP_VERSION "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}")

endif()

project(
    CALINE3_CPP
  VERSION "${APP_VERSION}"
  LANGUAGES C CXX
  DESCRIPTION "CALINE3 Model Algorithm (rewritten in C++)"
)

# Naming apps & libraries:
set(APP_VER "${APP_VERSION_MAJOR}${APP_VERSION_MINOR}")
set(APP_VER_CFG "${APP_VER}$<$<CONFIG:Debug>:d>")

##########################################################################
#
#   OPTIONS
#

# # Dimensional mode option:
# set(DIMENSIONAL_MODE ON CACHE BOOL "Dimensional mode: On/Off")

# GNU standard directories for installation
include(GNUInstallDirs)

# # helper modules:
# list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Check whether the compiler supports an interprocedural optimization (IPO/LTO):
include(CheckIPOSupported OPTIONAL RESULT_VARIABLE check_ipo_module_found)
if(check_ipo_module_found)
  check_ipo_supported(
    RESULT LTO_OPTIMIZATION_SUPPORTED
    OUTPUT LTO_OPTIMIZATION_ERROR
    LANGUAGES C CXX
  )
else()
  set(LTO_OPTIMIZATION_SUPPORTED NO)
  set(LTO_OPTIMIZATION_ERROR "CheckIPOSupported module not found.")
endif()

# USE_ASAN option:
option(USE_ASAN "Use Address Sanitizer: OFF/ON" OFF)
message(DEBUG "Use Address Sanitizer: ${USE_ASAN}")

# MSVC_RUNTIME option
# for compilers targeting the MSVC ABI (other compilers ignore it).
# Dynamic RT is the default but Address Sanitizer requires the Static one:
set(_msvc_runtime_docstring "MSVC Runtime Library: Dynamic/Static (for compilers targeting the MSVC ABI)")
if(USE_ASAN)
  set(MSVC_RUNTIME "Static" CACHE STRING "${_msvc_runtime_docstring}" FORCE)
else()
  set(MSVC_RUNTIME "Dynamic" CACHE STRING "${_msvc_runtime_docstring}")
endif()

set_property(CACHE MSVC_RUNTIME PROPERTY STRINGS Dynamic Static)

if(MSVC_RUNTIME MATCHES "^Dynamic$")
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE STRING "MSVC runtime library for use by compilers targeting the MSVC ABI" FORCE)
elseif(MSVC_RUNTIME MATCHES "^Static$")
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "MSVC runtime library for use by compilers targeting the MSVC ABI" FORCE)
else()
  get_property(_msvc_rt_options CACHE MSVC_RUNTIME PROPERTY STRINGS)
  message(FATAL_ERROR "Invalid option MSVC Runtime Library: ${MSVC_RUNTIME} (allowed options: ${_msvc_rt_options}).")
endif()
message(DEBUG "MSVC Runtime Library: ${MSVC_RUNTIME}")

##########################################################################
#
#   SOURCE PATHS
#

cmake_path(SET ROOT_DIR "${PROJECT_SOURCE_DIR}")
cmake_path(SET CALINE3_DIR "${ROOT_DIR}/CALINE3")

##########################################################################
#
#   TARGETS to BUILD
#

# Metrology library (units of measurement):
add_subdirectory ("Metrology")

# CALINE3 app:
add_subdirectory ("CALINE3")

# Boost Test app:
add_subdirectory ("TestBoost")