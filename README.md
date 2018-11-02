# Badgers

## Compiling
`cd` to `src/` and run `make`, making any changes to the `Makefile` depending on your system (it seems to work on Linux and OSX). This will create the `SImodel` executable and copy it to main directory.

## Running
The basic idea is to run `./SImodel ...` with the appropriate parameters. The easiest way to run this is to enter **GNU Octave** and run
```Octave
P = std_pars()
P.name = "d1"
generate(d1)
```
## Generating figures
Each of the figures can be generated by opening Octave and running e.g. `fig1()`. This will generate data in `data/` if necessary. If changes have been made and the data needs to be regenerated, either delete the appropriate data file, or run `fig1(12345)`.

Figures 2, 4, and 5 all require **Julia**, they can be made by running
```Julia
include("makefig23.jl")
makefig23(2)
```
and 
```Julia
include("makefig45.jl")
makefig45(4)
```


