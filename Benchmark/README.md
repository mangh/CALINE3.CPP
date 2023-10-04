# Metrology Benchmark

* This test has nothing to do with the CALINE3 application.
It only aims to compare the performance of a solution that uses units with one that uses only ordinary numbers.
Thus, it is more of a test of the [Metrology](https://github.com/mangh/CALINE3.CPP/tree/main/Metrology) concept itself.

* You can disable the test by commenting out the
  ```cmake
  add_subdirectory ("Benchmark")
  ```
  command line in the top-level [CMakeLists.txt](https://github.com/mangh/CALINE3.CPP/blob/main/CMakeLists.txt) script.
* Example results (abridged) obtained with Debian GNU/Linux 12 (bookworm):
  ```sh
  ~/CALINE3.CPP/build$ grep -m 1 'model name' /proc/cpuinfo
  model name      : 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
  ```
  ```sh
  ~/CALINE3.CPP/build$ ./Benchmark/Benchmarkv22  --benchmark-no-analysis "[!benchmark]"
  Filters: [!benchmark]
  Randomness seeded to: 4270263455
  
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Benchmarkv22 is a Catch2 v3.4.0 host application.
  Run with -? for options
  
  benchmark name                            samples    iterations          mean
  -------------------------------------------------------------------------------
  bullet plain                                   100             1    126.467 us
  bullet measured                                100             1    127.498 us
  
  addition plain                                 100         27947   0.453414 ns
  addition measured                              100         29146   0.448403 ns
  
  subtraction plain                              100         28716   0.451188 ns
  subtraction measured                           100         28877   0.457345 ns
  
  multiplication plain                           100         28432   0.449223 ns
  multiplication measured                        100         29481   0.446999 ns
  
  division plain                                 100         14244   0.981959 ns
  division measured                              100         13327   0.862213 ns
  
  conversion plain                               100         43434   0.276759 ns
  conversion measured                            100         43114    0.28994 ns
  
  assignment plain                               100         28931   0.431553 ns
  assignment measured                            100         27970   0.437731 ns
  
  mix plain                                      100          7652    1.79398 ns
  mix measured                                   100          7598    1.71877 ns
  
  ===============================================================================
  All tests passed (3612 assertions in 2 test cases)
  
  ~/CALINE3.CPP/build$
  ```
---
