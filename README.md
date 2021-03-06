# Badgers

## Compiling
`cd` to `src/` and run `make`, making any changes to the `Makefile` depending on your system (it seems to work on Linux and OSX). This will create the `SImodel` executable and copy it to main directory.

## Running
The basic idea is to run `./SImodel ...` with the appropriate parameters. The easiest way to run this is to enter **GNU Octave** and run
```octave
P = std_pars()
P.name = "d1"
generate(P)
```
This creates an object `P` containing the model parameters. `generate(P)` then takes `P`, converts it into a command `"nice ./SImodel ..."`, and runs it.

## Generating figures
Each of the figures can be generated by opening Octave and running e.g. `fig1()`. This will generate a file `data/fig1-data.mat` if it does not already exist. If changes have been made and the data needs to be regenerated, either delete the appropriate data file, or run `fig1(12345)`.

Some figures require a considerable amount of data. In those script files, there is an additional function `summarise_data` which extracts only the information necessary to draw the figure and saves it as e.g. `data/fig2-summary.mat`. Once this has been done, it is safe to delete the `data/fig2-data.mat`

Figure 3, Figures 4-5, and Figs S1-S4 have better quality versions generated in **Julia**, they can be made by running
```Julia
include("makefig3.jl")
makefig3()
```
and 
```Julia
include("makefig45.jl")
makefig45(4)       # make Fig4
makefig45(1, true) # make FigS1
```


