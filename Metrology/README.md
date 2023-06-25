# C++ unit of measurement library for the [CALINE3](https://github.com/mangh/CALINE3.CPP/tree/main/CALINE3) application

* The project is created from the [CPPUnits](https://www.nuget.org/packages/Mangh.Metrology.CPPUnits) project template using the dotnet command (run from the top-level directory):
  ```sh
  dotnet new cppunits -n METROLOGY -o Metrology -ns "CALINE3::Metrology"
  ```
  This creates a `METROLOGY` project, in the `Metrology` folder, for the units of measure in the `CALINE3::Metrology` namespace.
  
  The project provides:
  *  [Templates/definitions.txt](https://github.com/mangh/CALINE3.CPP/blob/main/Metrology/Templates/definitions.txt) - initial unit definitions,
  *  [Templates/\*.xslt](https://github.com/mangh/CALINE3.CPP/blob/main/Metrology/Templates) - templates for the target C++ unit structures,
  *  and a [CMakeLists.txt](https://github.com/mangh/CALINE3.CPP/blob/main/Metrology/CMakeLists.txt) script for building the library.

* The initial unit definitions should be edited to get the set of units required by the application (the initial set may not be sufficient).
---
* Before a project created like this can be built,
the [UnitGenerator](https://www.nuget.org/packages/Mangh.Metrology.UnitGenerator)
[dotnet tool](https://learn.microsoft.com/en-us/dotnet/core/tools/global-tools) must be installed on the system.
This is a key component (run from the CMakeLists.txt script) that generates C++ units (as Units/*.h header files). The tool must be installed using the following command:
  ```sh
  dotnet tool install --global Mangh.Metrology.UnitGenerator
  ```
  