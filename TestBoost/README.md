# Unit tests based on Boost Unit Test Framework

* Tests of the [CALINE3](https://github.com/mangh/CALINE3.CPP/tree/main/CALINE3) application and the [Metrology (units of measurement)](https://github.com/mangh/CALINE3.CPP/tree/main/Metrology) library.

* These tests require the [Boost Unit Test Framework](https://www.boost.org/) to be installed on the system (:exclamation:).

* The same tests are provided by the [TestGoogle](https://github.com/mangh/CALINE3.CPP/tree/main/TestGoogle) project.

* You can disable these tests by commenting out the
  ```cmake
  add_subdirectory ("TestBoost")
  ```
  command line in the top-level [CMakeLists.txt](https://github.com/mangh/CALINE3.CPP/blob/main/CMakeLists.txt) script.