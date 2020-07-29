# SEIR-Asyn

The model is based on a paper available here https://www.ncbi.nlm.nih.gov/pmc/articles/PMC7074281/

Tang, B., Wang, X., Li, Q., Bragazzi, N. L., Tang, S., Xiao, Y., & Wu, J. (2020). <br>
Estimation of the Transmission Risk of the 2019-nCoV and Its Implication for <br>
Public Health Interventions. *Journal of clinical medicine*, 9(2), 462. https://doi.org/10.3390/jcm9020462 <br>

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

## Model implimentation
### Parameters used in the model

| Symbol | Value from paper | Description  |
|--------|------------------|--------------|
| c      | 14.781           | Contact rate per person per day |
| b      | 2.1011e-8        | Probability of transmission per contact |
| q      | 1.8887e-7        | Probability of being quarantined per contact |
| e      | 0.142857         | Proportion of exposed individuals becoming infective per day |
| l      | 0.071428         | Proportion of quarantined uninfected individuals being released per day |
| n      | 0.86834          | Probability of having symptoms among infected individuals |
| di     | 0.13266          | Proportion of symptomatic infective individuals going into quarantine per day |
| dq     | 0.1259           | Proportion of quarantined exposed individuals progressing to quarantined infective per day |
| yi     | 0.33029          | Proportion of symptomatic infective individuals recovering per day |
| ya     | 0.13978          | Proportion of asymptomatic infective individuals recovering per day |
| yh     | 0.11624          | Proportion of quarantined infective individuals recovering per day |
| a      | 1.7826e-5        | Proportion of symptomatic infective and quarantined infective individuals dieing per day |
|   |
| S      | 11081000         | Initial susceptible population |
| E      | 105.1            | Initial exposed population |
| I      | 27.679           | Initial symptomatic infective population |
| A      | 53.839           | Initial asymptomatic infective population |
| Sq     | 739              | Initial quarantined susceptible population |
| Eq     | 1.1642           | Initial quarantined exposed population |
| H      | 1                | Initial quarantined infective population |
| R      | 2                | Initial recovered population |
| D      | 0                | Initial deceased population |

### Equations used in the model

|     |                      |                      |                      |          |
|-----|----------------------|----------------------|----------------------|----------|
| S'  | -(c×(I+A)×S×b×(1-q)) | -(c×(I+A)×S×b×q)     | -(c×(I+A)×S×(1-b)×q) | +(l×Sq)  |
| E'  | -(e×n×E)             | -(e×(1-n)×E)         | +(c×(I+A)×S×b×(1-q)) |          |
| I'  | -(di×I)              | -(yi×I)              | -(a×I)               | +(e×n×E) |
| A'  | -(ya×A)              | +(e×(1-n)×E)         |                      |          |
| Sq' | -(l×Sq)              | +(c×(I+A)×S×(1-b)×q) |                      |          |
| Eq' | -(dq×Eq)             | +(c×(I+A)×S×b×q)     |                      |          |
| H'  | -(yh×H)              | -(a×H)               | +(di×I)              | +(dq×Eq) |
| R'  | +(yi×I)              | +(ya×A)              | +(yh×H)              |          |
| D'  | +(a×I)               | +(a×H)               |                      |          |


## Run the model

### OPTION 1 - Default parameters

> ./bin/SEIR_Asyn (linux)

> ./bin/SEIR_Asyn.exe (windows)

### OPTION 2 - Define your own parameters

Place a text file (e.g. my_data.txt) with the input parameters in the folder *input_data*

Run the program normally using

> ./bin/SEIR_Asyn my_data.txt (linux)

> ./bin/SEIR_Asyn.exe my_data.txt (windows)

**NOTE**
Follow the instructions in input.txt for how to lay out your data

## Visualize the results

runner.py and plotter.py are python3 files that assume that you are in a linux like environment and that they are run from the root of this repo.

They both require modifying python3 sourcecode to use.

runner.py has 2 options for a main-ish function. They both run the simulator with a cartesian product of possible inputs.
main() starts at time=0, where main_c(csv_file_path, time) starts at the specified time with the new constants but the old populations and creates an output that resembles what would happen if the first sim had it's constants changed at the specified time in it's run.

plotter.py has a number of functions that read some or all of the .csv files that runner.py puts in ./csv and produces mathplotlib charts of them.
