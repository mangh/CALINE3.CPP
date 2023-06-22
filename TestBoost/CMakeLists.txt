# Metrology Boost Tests
#
set(target "TestBoost")

# Find libboost_unit_test_framework-*:
set(component "unit_test_framework")
string(TOUPPER ${component} _COMPONENT)

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
if(MSVC_RUNTIME MATCHES "Static")
  set(Boost_USE_STATIC_RUNTIME   ON)
else()
  set(Boost_USE_STATIC_RUNTIME  OFF)
endif()

# FindBoost module (used below) may need Hints (e.g. BOOST_ROOT)
# to find Boost repository (include-directories and/or libraries):
find_package(Boost #[[REQUIRED]] COMPONENTS "${component}")
if(NOT Boost_${_COMPONENT}_FOUND)
  message(WARNING "Could not find Boost ${_COMPONENT} component: cannot build Boost Tests.")
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

# if(DIMENSIONAL_MODE)
#   target_compile_definitions(${target} PRIVATE DIMENSIONAL_ANALYSIS)
# endif()
target_compile_features(${target} PUBLIC cxx_std_17)
target_compile_options(${target} PRIVATE $<IF:$<C_COMPILER_ID:MSVC>,/W3,-Wall -Wextra>)
target_include_directories(${target}
    PRIVATE
  # BoostTest and METROLOGY_LIBRARY include-directories will get in here via
  # target_link_libraries linked in below (as their Usage Requirements).
  "${CALINE3_DIR}"
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

set_target_properties(${target} PROPERTIES OUTPUT_NAME "${target}${APP_VER_CFG}")

install(TARGETS ${target}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  # PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LUA_APP_VER_CFG}
  # FILE_SET public_headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LUA_APP_VER_CFG}
)

##########################################################################
#
#   TEST
#

enable_testing()
add_test(NAME "MetBoostTesting" COMMAND ${target} "--log_level=test_suite")
