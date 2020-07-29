import numpy
import os
import decimal
from datetime import datetime
import itertools
import re
import matplotlib.pyplot as plt

def cad_time(line_time):
    if line_time in [None, '', '\n']:
        return None
    time_parts = line_time.split(':')
    return int(time_parts[0])*(60*60*1000) + int(time_parts[1])*(60*1000) + int(time_parts[2])*(1000) + int(time_parts[3])


def seird_state_output(state_path="./simulation_results/SEIRD_output_state.txt"):
    with open(state_path, "r") as state_file:
        while True:
            line_time     = state_file.readline()
            lines = []
            for _ in range(9):
                lines.append(state_file.readline())
            lines.sort()

            if('\n' in lines or '' in lines):
                return

            time                    = float(line_time)
            asymptomatic_infective  = float(lines[0][len('State for model asymptomatic_infective is {"asymptomatic_infective":')  :-len('}\n')])
            deceased                = float(lines[1][len('State for model deceased is {"deceased":')                              :-len('}\n')])
            exposed                 = float(lines[2][len('State for model exposed is {"exposed":')                                :-len('}\n')])
            quarantined_exposed     = float(lines[3][len('State for model quarantined_exposed is {"quarantined_exposed":')        :-len('}\n')])
            quarantined_infective   = float(lines[4][len('State for model quarantined_infective is {"quarantined_infected":')     :-len('}\n')])
            quarantined_susceptible = float(lines[5][len('State for model quarantined_susceptible is {"quarantined_susceptible":'):-len('}\n')])
            recovered               = float(lines[6][len('State for model recovered is {"recovered":')                            :-len('}\n')])
            susceptible             = float(lines[7][len('State for model susceptible is {"susceptible":')                        :-len('}\n')])
            symptomatic_infective   = float(lines[8][len('State for model symptomatic_infective is {"symptomatic_infective":')    :-len('}\n')])

            yield { "time":time,
                    "susceptible":susceptible,
                    "exposed":exposed,
                    "symptomatic_infective":symptomatic_infective,
                    "asymptomatic_infective":asymptomatic_infective,
                    "quarantined_susceptible":quarantined_susceptible,
                    "quarantined_exposed":quarantined_exposed,
                    "quarantined_infective":quarantined_infective,
                    "recovered":recovered,
                    "deceased":deceased,
                    }

'''
c = 14.781      0.904      Contact rate
b = 2.1011e-8   1.1886e-9  Probability of transmission per contact
q = 1.8887e-7   6.3654e-8  Quarantined rate of exposed individuals
e = 1/7         0          Transition rate of exposed individuals to the infected class
l = 1/14        0          Rate at which the quarantined uninfected contacts were released into the wider community
n = 0.86834     0.049227   Probability of having symptoms among infected individuals
di= 0.13266     0.021315   Transition rate of symptomatic infected individuals to the quarantined infected class
dq= 0.1259      0.052032   Transition rate of quarantined exposed individuals to the quarantined infected class
yi= 0.33029     0.052135   Recovery rate of symptomatic infected individuals
ya= 0.13978     0.034821   Recovery rate of asymptomatic infected individuals
yh= 0.11624     0.038725   Recovery rate of quarantined infected individuals
a = 1.7826e-5   6.8221e-6  Disease-induced death rate
'''

def continue_running(time, i_file, o_file, c, b, q, e, l, n, di, dq, yi, ya, yh, a):
    o_file.write(i_file.readline())
    line = None
    while True:
        line = i_file.readline()
        if time > float(line.split(',',1)[0]):
            o_file.write(line)
        else:
            break
    line_nums = [float(num) for num in line.split(',')]
    run(o_file, c, b, q, e, l, n, di, dq, yi, ya, yh, a, *(line_nums[1:]), offset_time = line_nums[0])
    #s['time']}, {s['susceptible']}, {s['exposed']}, {s['symptomatic_infective']}, {s['asymptomatic_infective']}, {s['quarantined_susceptible']}, {s['quarantined_exposed']}, {s['quarantined_infective']}, {s['recovered']}, {s['deceased']}



def run(o_file, c, b, q, e, l, n, di, dq, yi, ya, yh, a, S, E, I, A, Sq, Eq, H, R, D, *, offset_time = 0.0):
    with open("./input_data/auto_input.txt", "w") as input_file:
        input_file.write(f"c  = {c};\n")
        input_file.write(f"b  = {b};\n")
        input_file.write(f"q  = {q};\n")
        input_file.write(f"e  = {e};\n")
        input_file.write(f"l  = {l};\n")
        input_file.write(f"n  = {n};\n")
        input_file.write(f"di = {di};\n")
        input_file.write(f"dq = {dq};\n")
        input_file.write(f"yi = {yi};\n")
        input_file.write(f"ya = {ya};\n")
        input_file.write(f"yh = {yh};\n")
        input_file.write(f"a  = {a};\n")

        input_file.write(f"S  = {S};\n")
        input_file.write(f"E  = {E};\n")
        input_file.write(f"I  = {I};\n")
        input_file.write(f"A  = {A};\n")
        input_file.write(f"Sq = {Sq};\n")
        input_file.write(f"Eq = {Eq};\n")
        input_file.write(f"H  = {H};\n")
        input_file.write(f"R  = {R};\n")
        input_file.write(f"D  = {D};\n")


    os.system("./bin/SEIR-Asyn auto_input.txt")
    for s in seird_state_output():
        o_file.write(f"{offset_time+s['time']}, {s['susceptible']}, {s['exposed']}, {s['symptomatic_infective']}, {s['asymptomatic_infective']}, {s['quarantined_susceptible']}, {s['quarantined_exposed']}, {s['quarantined_infective']}, {s['recovered']}, {s['deceased']}\n")


def main():
    os.system("mkdir -p csv csv.old")
    os.system("mv ./csv/* ./csv.old/")

    set_c            = [14.781]#*s for s in [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]]         #
    set_b            = (2.1011e-8,)   #
    set_q            = (1.8887e-7,)#(numpy.logspace(-9, -3, num=35, endpoint=True, base=10.0))#1/50000,)#(numpy.arange(0, 0.76, 0.05))       #change of each contact being traced
    set_e            = (1/7,)       #E to (I || A)
    set_l            = (1/14,)      #Sq to S
    set_n            = (0.86834,)#(numpy.arange(0, 1, 0.1))         #I and not A
    set_di           = (0.13266,)#0.05,)         #I to H
    set_dq           = (0.1259,)          #Eq to H
    set_yi           = (0.33029,)         #I to R
    set_ya           = (0.13978,)         #A to R
    set_yh           = (0.11624,)         #H to R
    set_a            = (1.7826e-5,)       #(I || H) to D
    set_S            = (11081000,)        #
    set_E            = (105.1,)           #
    set_I            = (27.679,)          #
    set_A            = (53.839,)          #
    set_Sq           = (739,)
    set_Eq           = (1.1642,)
    set_H            = (1,)
    set_R            = (2,)
    set_D            = (0,)

    sets = (set_c, set_b, set_q, set_e, set_l, set_n, set_di, set_dq, set_yi, set_ya, set_yh, set_a, set_S, set_E, set_I, set_A, set_Sq, set_Eq, set_H, set_R, set_D)


    #count and total_count are used by the progress indicator
    count = 0
    total_count = sum(1 for _ in itertools.product(*sets))
    #return
    for args in itertools.product(*sets):
        o_file_name = f"./csv/{','.join([str(arg) for arg in args])},seird.csv"
        print("next: "+o_file_name)
        with open(o_file_name, "w") as o_file:
            o_file.write("time, susceptible, exposed, symptomatic_infective, asymptomatic_infective, quarantined_susceptible, quarantined_exposed, quarantined_infective, recovered, deceased\n");
            run(o_file, *args)
            count += 1
            print(f"\nProgress: {count}/{total_count} : {count/total_count}\n")


def c_main(i_file_name, offset_time):
    os.system("mkdir -p csv csv.old")
    os.system("mv ./csv/* ./csv.old/")

    set_c            = [14.781]#*s for s in [1, 0.8, 0.5, 0.3, 0.1]]         #
    set_b            = (2.1011e-8,)   #
    set_q            = [1.8887e-7*s for s in [1, 5, 10, 15, 20]]#(numpy.logspace(-9, -3, num=35, endpoint=True, base=10.0))#1/50000,)#(numpy.arange(0, 0.76, 0.05))       #change of each contact being traced
    set_e            = (1/7,)       #E to (I || A)
    set_l            = (1/14,)      #Sq to S
    set_n            = (0.86834,)#(numpy.arange(0, 1, 0.1))         #I and not A
    set_di           = (0.13266,)#0.05,)         #I to H
    set_dq           = (0.1259,)          #Eq to H
    set_yi           = (0.33029,)         #I to R
    set_ya           = (0.13978,)         #A to R
    set_yh           = (0.11624,)         #H to R
    set_a            = (1.7826e-5,)       #(I || H) to D

    sets = (set_c, set_b, set_q, set_e, set_l, set_n, set_di, set_dq, set_yi, set_ya, set_yh, set_a)

    count = 0
    total_count = sum(1 for _ in itertools.product(*sets))
    #return
    for args in itertools.product(*sets):
        o_file_name = f"./csv/{','.join([str(arg) for arg in args])},seird_continued.csv"
        print("next: "+o_file_name)
        with open(i_file_name) as i_file, open(o_file_name, "w") as o_file:
            #o_file.write("time, susceptible, exposed, symptomatic_infective, asymptomatic_infective, quarantined_susceptible, quarantined_exposed, quarantined_infective, recovered, deceased\n");
            continue_running(offset_time, i_file, o_file, *args)
            count += 1
            print(f"\nProgress: {count}/{total_count} : {count/total_count}\n")


c_main('base.csv', 10)
#main()

'''
with open(f"./csv/paper.csv", 'w') as o_file:
    o_file.write("time, susceptible, exposed, symptomatic_infective, asymptomatic_infective, quarantined_susceptible, quarantined_exposed, quarantined_infective, recovered, deceased\n");
    for s in seird_state_output():
        o_file.write(f"{s['time']}, {s['susceptible']}, {s['exposed']}, {s['symptomatic_infective']}, {s['asymptomatic_infective']}, {s['quarantined_susceptible']}, {s['quarantined_exposed']}, {s['quarantined_infective']}, {s['recovered']}, {s['deceased']}\n")
'''


