# CALINE3 and Metrology (Google) Tests
#

##########################################################################
#
#   GoogleTest External Libs
#

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# GoogleTest main (master) release branch
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  # GIT_TAG release-1.12.1  # the latest version that compiles without errors or warnings
  GIT_TAG main
)

FetchContent_MakeAvailable(googletest)

# Clang compiler issues the following warnings for "GIT_TAG main" (v1.14.0 as of September 15, 2023) on Windows:
#
# googletest-src/googletest/include\gtest/internal/gtest-port.h:832:5: warning: 'GTEST_LINKED_AS_SHARED_LIBRARY' is not defined, evaluates to 0 [-Wundef]
# googletest-src/googletest/include\gtest/internal/gtest-port.h:834:7: warning: 'GTEST_CREATE_SHARED_LIBRARY' is not defined, evaluates to 0 [-Wundef]
#
# These warnings are harmless (Clang guesses the correct values and CMake builds the correct executable),
# but you can suppress them with the following definitions:
# 
# if (NOT BUILD_SHARED_LIBS)
#   target_compile_definitions(gtest
#     PUBLIC GTEST_LINKED_AS_SHARED_LIBRARY=1
#     PUBLIC GTEST_CREATE_SHARED_LIBRARY=1
#   )
# endif()
#
# NOTE: All of the above refers to the CMake setting BUILD_SHARED_LIBS=OFF.
#       BUILD_SHARED_LIBS=ON is even more troublesome: better stay away from it.
#

##########################################################################
#
#   Metrology Unit Testing App
#

set(target "TestGoogle")

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
# Warning options set differently for MSVC-like compiler (/W3) and all others (-Wall -Wextra):
target_compile_options(${target} PRIVATE $<IF:$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},MSVC>,/W3,-Wall -Wextra>)
target_include_directories(${target}
    PRIVATE
  "${CALINE3_DIR}"
  # The include directories required for GoogleTest and METROLOGY libraries
  # will be provided via the target_link_libraries() command used below
  # (as the so-called Usage Requirements of these libraries)
)

##########################################################################
#
#   LINK OPTIONS & LIBRARIES
#

target_link_libraries(${target}
    PRIVATE
  GTest::gtest
  GTest::gtest_main
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

set(testApp "${target}-v${APP_VER_CFG}")
set_target_properties(${target} PROPERTIES OUTPUT_NAME "${testApp}")

##########################################################################
#
#   TEST
#
#   Use the following command (in the project's root directory) to
#   run this test:
#
#       ./build/${testApp}
#

add_test(NAME "# CALINE3 and Metrology (Google) Tests" COMMAND ${target})

