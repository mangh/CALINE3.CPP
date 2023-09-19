# CALINE3 and Metrology (Boost) Tests
#
set(target "TestBoost")

# Find libboost_unit_test_framework-* libraries:
set(component "unit_test_framework")
string(TOUPPER ${component} _COMPONENT)

# set(Boost_USE_STATIC_LIBS    ON)  # only find static libs
# set(Boost_USE_DEBUG_LIBS     OFF) # ignore debug libs and
# set(Boost_USE_RELEASE_LIBS   ON)  # only find release libs
# set(Boost_USE_MULTITHREADED  ON)
# set(Boost_USE_STATIC_RUNTIME OFF)

# FindBoost module (used below) may need Hints (e.g. BOOST_ROOT)
# to find Boost repository (include-directories and/or libraries):
find_package(Boost #[[REQUIRED]] COMPONENTS "${component}")
if(NOT Boost_${_COMPONENT}_FOUND)
  message(WARNING "Boost Tests unavailable: ${_COMPONENT} component cannot be found.")
  return()
endif()

list(APPEND CMAKE_MESSAGE_INDENT "  ")
message(DEBUG "Boost_VERSION = ${Boost_VERSION}")
message(DEBUG "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
message(DEBUG "Boost_LIBRARY_DIRS = ${Boost_LIBRARY_DIRS}")
message(DEBUG "Boost_LIBRARIES = ${Boost_LIBRARIES}")
message(DEBUG "Boost_${_COMPONENT}_LIBRARY = ${Boost_${_COMPONENT}_LIBRARY}")
message(DEBUG "Boost_LIB_DIAGNOSTIC_DEFINITIONS = ${Boost_LIB_DIAGNOSTIC_DEFINITIONS}")
list(POP_BACK CMAKE_MESSAGE_INDENT)

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
  # The include directories required for BoostTest and METROLOGY libraries
  # will be provided via the target_link_libraries() command used below
  # (as the so-called Usage Requirements of these libraries)
)

##########################################################################
#
#   LINK OPTIONS & LIBRARIES
#

target_link_libraries(${target}
    PRIVATE
  "Boost::${component}"
  METROLOGY_LIBRARY
)

set_target_properties(${target} PROPERTIES
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
#       ./build/${testApp} --log_level=test_suite
#

add_test(NAME "CALINE3 and Metrology (Boost) Tests" COMMAND ${target} "--log_level=test_suite")
