# SEIRD

The model is available in https://github.com/SimulationEverywhere-Models/Cadmium-SEIRD

This instructions assume that:

1. Cadmium has been installed following the instructions in the manual:
http://www.sce.carleton.ca/courses/sysc-5104/lib/exe/fetch.php?media=cadmiumusermanual.pdf

2. The SEIR-Asyn model is cloned inside the folder: Cadmium-Simulation-Environment/DEVS-Models
(see the installation manual)

## Compile the model

1. Install Cadmium following the instructions in the manual:
http://www.sce.carleton.ca/courses/sysc-5104/lib/exe/fetch.php?media=cadmiumusermanual.pdf

2. Clone the repository inside the folder: *Cadmium-Simulation-Environment/DEVS-Models*
https://github.com/SimulationEverywhere-Models/Cadmium-SEIR-Asyn

3. Compile the model using the make file

## Run the model

### OPTION 1 - Default parameters

> ./bin/SEIRD (linux)

> ./bin/SEIRD.exe (windows)

### OPTION 2 - Define your own parameters

Place a text file (e.g. my_data.txt) with the input parameters in the folder *input_data*

Run the program normally using

> ./bin/SEIRD my_data.txt (linux)

> ./bin/SEIRD.exe my_data.txt (windows)

**NOTE**
Follow the instructions in input.txt for how to lay out your data

## Visualize the results

runner.py and plotter.py are python3 files that assume that you are in a linux like environment and that they are run from the root of this repo.

They both require modifying python3 sourcecode to use.

runner.py has 2 options for a main-ish function. They both run the simulator with a cartesian product of possible inputs.
main() starts at time=0, where main_c(csv_file_path, time) starts at the specified time with the new constants but the old populations and creates an output that resembles what would happen if the first sim had it's constants changed at some point in it's run.

plotter.py has a number of functions that read some or all of the .csv files that runner.py puts in ./csv and produces mathplotlib charts of them.
