# Metrology Benchmarks
#

set(target "Benchmark")

set(_source_files
  Benchmark.cpp
  Bullet_Plain.cpp
  Bullet_Measured.cpp
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

target_compile_features(${target} PRIVATE cxx_std_17)
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

# IPO/LTO optimization makes the application just as efficient
# whether it uses units of measure or simple numbers.
if(LTO_OPTIMIZATION_SUPPORTED)
  set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(WARNING "IPO/LTO is not supported: ${LTO_OPTIMIZATION_ERROR}")
endif()

# target_link_libraries() command below imports the so called Usage Requirements of
# the nanobench as well as METROLOGY_LIBRARY (header-only) libraries.
# The requirements include (among other things) their include directories
# (so there is no need to use the target_include_directories() command):
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
#       ./build/${testApp} "[!benchmark]" [--benchmark-no-analysis]
#

add_test(NAME "Metrology Benchmarks" COMMAND ${target} Bullet --benchmark-no-analysis)

