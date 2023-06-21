# Copyright (©) MAN. This program is provided to you under the terms
# of the license as published at https://github.com/mangh/Metrology.

#[=======================================================================[.rst:
Units of Measurement C++ Header-only Library
--------------------------------------------

Generate unit of measurement header files:

  Units/*.h,

based on unit definitions from the file:

  Templates/definitions.txt

and XSLT-templates for unit/scale structures:

  Templates/unit.xslt,
  Templates/scale.xslt,
  Templates/report.xslt

that use XSLT helper functions:

  Templates/replace-string.xslt,
  Templates/math-constants.xslt.

This project (i.e the above folders and files, including this script)
is intended to be used from within a parent project using CMake
add_subdirectory() command.


Targets defined
^^^^^^^^^^^^^^^

  METROLOGY_LIBRARY - interface (header-only) library (main target),

  METROLOGY_HEADERS - custom target that triggers rebuild (custom command).


Result Variables
^^^^^^^^^^^^^^^^

  METROLOGY_NAMESPACE - namespace for units of measurement.

  METROLOGY_CHANGE_TIP - a file that serves as an indicator
                        (guard) of changes in metrological units.
                        It triggers a rebuild when tracked as a
                        file-level dependency (see below).

#]=======================================================================]

cmake_minimum_required (VERSION ${CMAKE_VERSION})

##########################################################################
#
#   METROLOGY project
#

project(
  METROLOGY
  LANGUAGES C CXX
  DESCRIPTION "Unit of measurement headers-only library."
)

set(target METROLOGY_LIBRARY)

##########################################################################
#
#   METROLOGY_SOURCE_DIR subfolders
#

set(_templates_subdir "Templates")
set(_units_subdir "Units")

##########################################################################
#
#   METROLOGY namespace
#

set(METROLOGY_NAMESPACE "CALINE3::Metrology" CACHE STRING "Namespace for units of measurement.")

##########################################################################
#
#   METROLOGY_CHANGE_TIP
#
#   Each time the metrological units are reconstructed, 
#   the "Units/generator_report.txt" file is created anew. 
#   Therefore, it can serve as an indicator (guard) of changes
#   in metrological units and trigger a rebuild when tracked
#   as a file-level dependency for source file(s) that depend
#   on units.
#
#   You can explicitly set this file-level dependency with
#   the following command:
#
#     set_property(
#       SOURCE file... 
#       PROPERTY OBJECT_DEPENDS
#       ${METROLOGY_CHANGE_TIP}
#     )
#
#   for each source file(s) that need to be recompiled when
#   unit definitions and/or templates change.
#

set(
  METROLOGY_CHANGE_TIP
  "${METROLOGY_SOURCE_DIR}/${_units_subdir}/generator_report.txt"
  CACHE FILEPATH
  "File that serves as an indicator (guard) of changes in metrological units."
  FORCE
)

##########################################################################
#
#   METROLOGY rebuild command (UnitGenerator)
#
#   NOTE: Please amend the command:
#
#       "cmake -E chdir ... UnitGenerator ..."
#
#   (bellow) depending on how the Mangh.Metrology.UnitGenerator tool
#   is installed on your computer: as a "global tool" (as assumed here), 
#   or a "global tool in a custom location" or as a "local tool").
#   For details, see article "How to manage .NET tools",
#   https://learn.microsoft.com/en-us/dotnet/core/tools/global-tools.
#

add_custom_command(
  OUTPUT
    "${METROLOGY_CHANGE_TIP}"
  COMMAND cmake -E echo
  COMMAND cmake -E rm -f "${METROLOGY_SOURCE_DIR}/${_units_subdir}/*.h"
  COMMAND cmake -E chdir "${METROLOGY_SOURCE_DIR}" UnitGenerator CPP "${METROLOGY_NAMESPACE}" "${_templates_subdir}" "${_units_subdir}"
  COMMAND cmake -E echo
  DEPENDS
    "${METROLOGY_SOURCE_DIR}/${_templates_subdir}/definitions.txt"
    "${METROLOGY_SOURCE_DIR}/${_templates_subdir}/unit.xslt"
    "${METROLOGY_SOURCE_DIR}/${_templates_subdir}/scale.xslt"
    "${METROLOGY_SOURCE_DIR}/${_templates_subdir}/report.xslt"
    "${METROLOGY_SOURCE_DIR}/${_templates_subdir}/replace-string.xslt"
    "${METROLOGY_SOURCE_DIR}/${_templates_subdir}/math-constants.xslt"

  COMMENT "Generate METROLOGY (headers-only) Library."
  USES_TERMINAL
)

##########################################################################
#
#   METROLOGY_HEADERS target
#
#   Custom target that triggers the REBUILD COMMAND
#

add_custom_target(METROLOGY_HEADERS DEPENDS "${METROLOGY_CHANGE_TIP}")

##########################################################################
#
#   METROLOGY_LIBRARY target
#
#   METROLOGY_LIBRARY (as an INTERFACE library) does not build itself
#   but target(s) it depends on (such as METROLOGY_HEADERS) are followed
#   (transitively) in its place and trigger a rebuild when needed.
#

add_library(${target} INTERFACE)
target_include_directories(${target} INTERFACE Units)
add_dependencies(${target} METROLOGY_HEADERS)
