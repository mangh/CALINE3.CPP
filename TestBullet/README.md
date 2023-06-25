# Metrology Performance Test

* This test has nothing to do with the CALINE3 application.
It only aims to compare the performance of a solution that uses units with one that uses only ordinary numbers.
Thus, it is more of a test of the [Metrology](https://github.com/mangh/CALINE3.CPP/tree/main/Metrology) concept itself.

* You can disable the test by commenting out the
  ```cmake
  add_subdirectory ("TestBullet")
  ```
  command line in the top-level [CMakeLists.txt](https://github.com/mangh/CALINE3.CPP/blob/main/CMakeLists.txt) script.