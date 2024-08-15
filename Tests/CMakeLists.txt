# Metrology and CALINE3 Tests
#

##########################################################################
#
#   catch2 unit testing framework
#

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.0     # August 2024
)

FetchContent_MakeAvailable(Catch2)

##########################################################################
#
#   Test app
#

set(target "Tests")

set(_source_files
  Quantities.cpp
  Levels.cpp
  CALINE3.cpp
  ${CALINE3_DIR}/Job.cpp
  ${CALINE3_DIR}/JobReader.cpp
  ${CALINE3_DIR}/Link.cpp
  ${CALINE3_DIR}/WindFlow.cpp
  ${CALINE3_DIR}/LinkElement.cpp
  ${CALINE3_DIR}/Maths.cpp
  ${CALINE3_DIR}/Meteo.cpp
  ${CALINE3_DIR}/Plume.cpp
  ${CALINE3_DIR}/Receptor.cpp
)

set_property(
  SOURCE ${_source_files}
  PROPERTY OBJECT_DEPENDS "${METROLOGY_CHANGE_TIP}"
)

add_executable(
  ${target}
  ${_source_files}
)

##########################################################################
#
#   COMPILE DEFINITIONS & OPTIONS
#

target_compile_features(${target} PUBLIC cxx_std_17)
target_compile_options(${target} PRIVATE $<IF:$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},MSVC>,/W3,-Wall -Wextra>)
target_include_directories(${target}
    PRIVATE
  "${CALINE3_DIR}"
  # The include directories required for Catch2 and METROLOGY libraries
  # will be provided via the target_link_libraries() command used below
  # (as the so-called Usage Requirements of these libraries)
  )

##########################################################################
#
#   LINK OPTIONS & LIBRARIES
#

target_link_libraries(${target}
    PRIVATE
  Catch2::Catch2WithMain
  METROLOGY_LIBRARY
)

set_target_properties(${target}
    PROPERTIES
  LINK_WHAT_YOU_USE ON
)

##########################################################################
#
#   INSTALL
#

set(testApp "${target}v${APP_VER_CFG}")
set_target_properties(${target} PROPERTIES OUTPUT_NAME "${testApp}")

##########################################################################
#
#   TEST
#
#   Use the following command (in the project's root directory) to
#   run this test:
#
#       ./build/${testApp} [--success]
#

add_test(NAME "Metrology and CALINE3 Tests" COMMAND ${target})

