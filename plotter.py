#!/bin/python3

import matplotlib.pyplot as plt
import runner
import re
import math
import itertools
import os
import json



def parse_file(in_file):
    time = -1.0
    pops = {}
    for line in in_file:
        if re.match(r'^-?\d+(?:\.\d+)?$', line):
            new_time = float(line)
            if time != new_time:
                yield time, pops
                pops = {}
                time = new_time
        elif '[' in line:
            try:
                name, params, pop, _, travel = json.loads(line[line.find('['):])
                pops[name] = pop
                #print(time, name, pop)
            except:
                print("####")
                print("")
                print(line.strip())
                print("")
                print("####")
                exit()
    yield time, pops


def plot_file(in_file):
    plt.figure(figsize=(11, 8.5))
    times = []
    vals = {}
    peeks = {}

    for time, pops in parse_file(in_file):
        #print(time, len(pops))
        times.append(time)
        for name, pop in pops.items():
            infective = pop[4]
            if name not in vals:
                vals[name] = []
            if (name not in peeks) or (peeks[name][1] < infective):
                peeks[name] = (time, infective)
            vals[name].append(infective)

    for name, val in vals.items():
        plt.plot(times[:len(val)], [math.log10(i) for i in val], label=name)

    plt.title("Figure A, Recreated")
    plt.xlim(0, 55)
    plt.ylim(1, 5.5)
    plt.legend()

    figname = "contact_rate.png"#f"{file_number:0{1+len(str(len(files)))}},{name[:-len('.csv')]+'.png'}"
    print(figname)
    plt.savefig(figname)
    plt.close()
    for name, p in peeks.items():
        print(f"{name}, {p[0]-12:0.5}, {p[1]}")

def main(in_file):
    plot_file(in_file)



with open("./output_state.txt") as in_file:
    main(in_file)

