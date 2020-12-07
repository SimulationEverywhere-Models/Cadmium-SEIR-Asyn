#!/bin/python3

import matplotlib.pyplot as plt
import runner
import re
import math
import itertools
import os
import json







def main(in_file):
    time = 0
    for line in in_file:
        if re.match(r'^-?\d+(?:\.\d+)?$', line):
            time = float(line)
        else:
            try:
                name, params, pop, _, travel = json.loads(line[line.find('['):])
                print(time, name, pop)
            except:
                print("####")
                print("")
                print(line.strip())
                print("")
                print("####")
                exit()




with open("./output_state.txt") as in_file:
    main(in_file)

