# Genetic Diophant
===================

Simple implementation of the genetic algorithm for solving linear diophantine equations on C++11.

## Source structure

* `src/genetic_diophant` contains all the implementation into `genetic.h` file and the example into main.cpp;
* `src/test` contains Google C++ Testing Framework tests.

## Build

Use CMake:

```
> mkdir build
> cd build
> cmake ..
> make -j8
> genetic_diophant
...
```

## Tests

For testing purposes this project uses Google C++ Testing Framework. To enable tests you should define `ENABLE_TESTS=ON` variable:

```
> cmake -DENABLE_TESTS=ON ..
> make -j8
> ./genetic_diophant_test
...
`
