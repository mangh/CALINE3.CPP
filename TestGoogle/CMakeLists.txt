# Metrology Google Tests
#

##########################################################################
#
#   GoogleTest External Libs
#

set(CMAKE_CXX_STANDARD 14)

# For Windows: Prevent overriding the parent project's compiler/linker settings
if(MSVC_RUNTIME MATCHES "Static")
  set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)
else()
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

include(FetchContent)

# GoogleTest standard release
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1  # 58d77fa8070e8cec2dc1ed015d66b454c8d78850
  # GIT_TAG v1.13.0
)

FetchContent_MakeAvailable(googletest)

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
target_compile_options(${target} PRIVATE $<IF:$<OR:$<CXX_COMPILER_ID:MSVC>,$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},MSVC>>,/W3,-Wall -Wextra>)
target_include_directories(${target}
    PRIVATE
  # GoogleTest and METROLOGY_LIBRARY include-directories will get in here via
  # target_link_libraries() used below (as the so called Usage Requirements of those libraries).
  "${CALINE3_DIR}"
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

set_target_properties(${target} PROPERTIES OUTPUT_NAME "${target}${APP_VER_CFG}")

install(TARGETS ${target}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  # PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LUA_APP_VER_CFG}
  # FILE_SET public_headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LUA_APP_VER_CFG}
)

##########################################################################
#
#   Test
#

enable_testing()
include(GoogleTest)
gtest_discover_tests(${target})
