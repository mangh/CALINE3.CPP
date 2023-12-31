# CALINE3 application
#
set(target "Caline3")

set(_source_files
  CALINE3.cpp
  Job.cpp
  JobReader.cpp
  Link.cpp
  LinkElement.cpp
  Maths.cpp
  Meteo.cpp
  Plume.cpp
  Receptor.cpp
  Report.cpp
  WindFlow.cpp
)

# Set explicit dependency of source files on METROLOGY headers
# (for simplicity, it is assumed here that all source files depend on those headers):
set_property(
  SOURCE ${_source_files}
  PROPERTY OBJECT_DEPENDS "${METROLOGY_CHANGE_TIP}"
)

# Add executable target to the project using the specified source files:
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

##########################################################################
#
#   ADDRESS SANITIZER
#

if(USE_ASAN)
  # The following is required to avoid: lld-link: error: /INFERASANLIBS is not allowed in .drectve
  target_compile_definitions(${target} PRIVATE _DISABLE_STRING_ANNOTATION _DISABLE_VECTOR_ANNOTATION)
  target_compile_options(${target} PRIVATE -fsanitize=address)
  target_link_options(${target} PRIVATE -fsanitize=address)
endif()

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
# the METROLOGY_LIBRARY, which include (among other things) its include directories
# (so there is no need to use the target_include_directories() command):
target_link_libraries(${target}
    PRIVATE
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
#   Use the following command (in the project's root directory) to
#   run the application:
#
#       ./build/${testApp} /path/to/input_data.txt
#
#   For example:
#
#       ./build/${testApp} ./CALINE3.EXP
#

set(testApp "${target}v${APP_VER_CFG}")
set_target_properties(${target} PROPERTIES OUTPUT_NAME "${testApp}")

install(TARGETS ${target}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

