# Metrology Benchmark

* This benchmark has nothing to do with the CALINE3 application. It only aims to compare the performance of a solution that uses units with one that uses only ordinary numbers. Thus, it is more of a test of the [Metrology](https://github.com/mangh/CALINE3.CPP/tree/main/Metrology) concept itself.

* You can disable the test by commenting out the command line:
  ```cmake
  add_subdirectory ("Benchmark")
  ```
  in the top-level [CMakeLists.txt](https://github.com/mangh/CALINE3.CPP/blob/main/CMakeLists.txt) script.

---

* Benchmark results obtained with __MSVC__ compiler (MSVC 19.37.32824.0 on Windows 11):
  ```cmd
  ~\CALINE3.CPP\build>.\Benchmark\Benchmarkv22.exe Bullet
  Filters: "Bullet"
  Randomness seeded to: 982254072
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Benchmarkv22.exe is a Catch2 v3.4.0 host application.
  Run with -? for options
  -------------------------------------------------------------------------------
  Bullet
  -------------------------------------------------------------------------------
  ~\CALINE3.CPP\Benchmark\Benchmark.cpp(19)
  ...............................................................................
  benchmark name                       samples       iterations    estimated
                                       mean          low mean      high mean
                                       std dev       low std dev   high std dev
  -------------------------------------------------------------------------------
  bullet plain                                   100             1    12.6635 ms
                                          126.343 us    126.083 us    126.678 us
                                          1.50209 us    1.23833 us    2.10914 us
  
  bullet measured                                100             1    12.5384 ms
                                          126.556 us      126.1 us    127.334 us
                                          2.97105 us    1.93035 us    4.75491 us
  ===============================================================================
  All tests passed (3605 assertions in 1 test case)
  ```

* Benchmark results obtained with __Clang__ compiler (Clang 16.0.6 on Windows 11):
  ```cmd
  ~\CALINE3.CPP\build>.\Benchmark\Benchmarkv22.exe Bullet
  Filters: "Bullet"
  Randomness seeded to: 342051353
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Benchmarkv22.exe is a Catch2 v3.4.0 host application.
  Run with -? for options
  -------------------------------------------------------------------------------
  Bullet
  -------------------------------------------------------------------------------
  ~/CALINE3.CPP/Benchmark/Benchmark.cpp(19)
  ...............................................................................
  benchmark name                       samples       iterations    estimated
                                       mean          low mean      high mean
                                       std dev       low std dev   high std dev
  -------------------------------------------------------------------------------
  bullet plain                                   100             1    12.4103 ms
                                          124.856 us    124.366 us    125.514 us
                                          2.86998 us    2.25729 us    3.97257 us
  
  bullet measured                                100             1    12.0921 ms
                                           121.44 us    121.033 us    121.884 us
                                          2.17203 us    1.92607 us     2.6183 us
  ===============================================================================
  All tests passed (3605 assertions in 1 test case)
  ```

* Benchmark results obtained obtained with __GCC__ compiler (GNU 12.2.0 on WSL Debian (bookworm)):
  ```sh
  ~/CALINE3.CPP/build$ ./Benchmark/Benchmarkv22 Bullet
  Filters: "Bullet"
  Randomness seeded to: 4047988734
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Benchmarkv22 is a Catch2 v3.4.0 host application.
  Run with -? for options
  -------------------------------------------------------------------------------
  Bullet
  -------------------------------------------------------------------------------
  ~/CALINE3.CPP/Benchmark/Benchmark.cpp:19
  ...............................................................................
  benchmark name                       samples       iterations    estimated
                                       mean          low mean      high mean
                                       std dev       low std dev   high std dev
  -------------------------------------------------------------------------------
  bullet plain                                   100             1    12.7065 ms
                                          128.622 us    128.112 us    129.778 us
                                          3.72473 us    1.89253 us    7.27877 us
  
  bullet measured                                100             1     12.685 ms
                                           129.35 us    128.597 us    130.527 us
                                           4.7449 us    3.44519 us    6.87878 us
  ===============================================================================
  All tests passed (3605 assertions in 1 test case)
  ```

---
