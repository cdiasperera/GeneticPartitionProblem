# A Genetic Algorithm Applied to the Partition Problem

## About
This program attempts to solve the partition problem for a given input sequence, via a genetic algorithm.

## Requirements
* `make` (Makefile)
* `gcc` (Or a `C` compiler of your choice)
* All Makefile commands are to be run in the `src` folder.

## Compiling
To create the program, execute:

1) In `./src/testing.h`, set `TESTING` to `false`.
2) `make`

## Changing the simulation parameter
To change the performance of the program, by modifying the parameters of the genetic algorithm, modify the constants `src/simconfig.h` file.

## Testing 
* To run the program against 5 testcases: `make test`
  1) In `./src/testing.h`, set `TESTING` to `false`

* To find the optimal t_2 for a given configuration
  1) In `./src/testing.h`, set `TESTING` to `true`
  2) In `./src/testing.h`, set `FINDT2` to `true`
  3) `make run`

* To find the optimal configuration for a given t_2
  1) In `./src/testing.h`, set `TESTING` to `true`
  2) In `./src/testing.h`, set `FINDOPTIMALCONFIG` to `true`
  3) `make run`

* To find the optimal t_2 for a given configuration
  1) In `./src/testing.h`, set `TESTING` to `true`
  2) In `./src/testing.h`, set `TESTPARAMS` to `true`
  3) `make run`