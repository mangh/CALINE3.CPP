# Unit tests based on [Google  Testing Framework](https://google.github.io/googletest/)

* Tests of the [CALINE3](https://github.com/mangh/CALINE3.CPP/tree/main/CALINE3) application and the [Metrology (units of measurement)](https://github.com/mangh/CALINE3.CPP/tree/main/Metrology) library.

* The same tests are provided by the [TestBoost](https://github.com/mangh/CALINE3.CPP/tree/main/TestBoost) project.

* You can disable the test by commenting out the
  ```cmake
  add_subdirectory ("TestGoogle")
  ```
  command line in the top-level [CMakeLists.txt](https://github.com/mangh/CALINE3.CPP/blob/main/CMakeLists.txt) script.