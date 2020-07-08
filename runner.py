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
def seird_message_output(message_path="./simulation_results/SEIRD_output_messages.txt"):
    with open(message_path, "r") as message_file:
        time = cad_time(message_file.readline())
        output = {'time':time}
        while True:
            line = message_file.readline()
            if line in [None, '', '\n'] or re.compile("^\d+:\d+:\d+:\d+\n$").match(line):
                yield dict(output)
                time = cad_time(line)
                if(time is None):
                    return
                output = {'time':time}
            elif line.startswith("[Laundromat_cleaning_defs::clean: {"):
                parts = re.split(r"[\{\}]", line)
                output["cleaning_clean"] = int('0'+parts[1])
                if non_full := int('0'+parts[3]):
                    output["cleaning_non_full_load"] = non_full
                if delay := cad_time(parts[5]):
                    output["cleaning_load_delayed"] = delay
            elif line.startswith("[Laundromat_shipping_defs::shipped: {"):
                parts = re.split(r"[\{\}]", line)
                output["shipping_clean"] = int('0'+parts[1])
                if short_delivery := int('0'+parts[3]):
                    output["shipping_short_delivery"] = short_delivery
            elif line.startswith("[Hospital_defs::dirty: {"):
                parts = re.split(r"[\{\}]", line)
                output["hospital_dirty"] = int('0'+parts[1])
                if outage := int('0'+parts[3]):
                    output["hospital_outage"] = outage
'''
def laundry_output():
    return zip(laundry_state_output(), laundry_message_output())

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

def run(c, b, q, e, l, n, di, dq, yi, ya, yh, a, susceptible, exposed, symptomatic, asymptomatic):
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
        
        input_file.write(f"S  = {susceptible};\n")
        input_file.write(f"E  = {exposed};\n")
        input_file.write(f"I  = {symptomatic};\n")
        input_file.write(f"A  = {asymptomatic};\n")
        input_file.write("Sq = 0;\n")
        input_file.write("Eq = 0;\n")
        input_file.write("H  = 0;\n")
        input_file.write("R  = 0;\n")
        input_file.write("D  = 0;\n")
    
    
    os.system("./bin/SEIRD auto_input.txt")
    with open(f"./csv/{c},{b},{q},{e},{l},{n},{di},{dq},{yi},{ya},{yh},{a},{susceptible},{exposed},{symptomatic},{asymptomatic},seird.csv", 'w') as o_file:
        o_file.write("time, cleaning_amount_dirty, cleaning_load_size, cleaning_load_time_remaining, cleaning_load_delay_time, shipping_amount_clean, shipping_time_until_next_shipment, hospital_amount_clean, hospital_amount_dirty, hospital_outage, hospital_time_until_next_shipment, m_cleaning_clean, m_cleaning_non_full_load, m_cleaning_load_delayed, m_shipping_clean, m_shipping_short_delivery, m_hospital_dirty, m_hospital_outage\n");
        for s in seird_state_output():
            o_file.write(f"{s['time']}, {s['susceptible']}, {s['exposed']}, {s['symptomatic_infective']}, {s['asymptomatic_infective']}, {s['quarantined_susceptible']}, {s['quarantined_exposed']}, {s['quarantined_infective']}, {s['recovered']}, {s['deceased']}\n")
           

def main():
    stats_c = (14.781, 0.904)
    stats_b = (2.1011e-8, 1.1886e-9)
    stats_q = (1.8887e-7, 6.3654e-8)
    stats_n = (0.86834, 0.049227)
    stats_di= (0.13266, 0.021315)
    stats_dq= (0.1259, 0.052030)
    stats_yi= (0.33029, 0.052135)
    stats_ya= (0.13978, 0.034821)
    stats_yh= (0.11624, 0.038725)
    stats_a = (1.7826e-5, 6.8221e-6)
    
    set_c            = numpy.arange(stats_c[0]-stats_c[1], stats_c[0]+stats_c[1]*1.1, stats_c[1])
    set_b            = numpy.arange(stats_b[0]-stats_b[1], stats_b[0]+stats_b[1]*1.1, stats_b[1])
    set_q            = numpy.arange(stats_q[0]-stats_q[1], stats_q[0]+stats_q[1]*1.1, stats_q[1])
    set_e            = (1/7,)
    set_l            = (1/14,)
    set_n            = numpy.arange(stats_n[0]-stats_n[1], stats_n[0]+stats_n[1]*1.1, stats_n[1])
    set_di           = numpy.arange(stats_di[0]-stats_di[1], stats_di[0]+stats_di[1]*1.1, stats_di[1])
    set_dq           = numpy.arange(stats_dq[0]-stats_dq[1], stats_dq[0]+stats_dq[1]*1.1, stats_dq[1])
    set_yi           = (stats_yi[0],)
    set_ya           = (stats_ya[0],)
    set_yh           = (stats_yh[0],)
    set_a            = (stats_a[0],)
    set_susceptible  = (1000000,)
    set_exposed      = (0,)
    set_symptomatic  = (100,)
    set_asymptomatic = (0,)
    
    sets = (set_c, set_b, set_q, set_e, set_l, set_n, set_di, set_dq, set_yi, set_ya, set_yh, set_a, set_susceptible, set_exposed, set_symptomatic, set_asymptomatic)
    
    
    #count and total_count are used by the progress indicator
    count = 0
    total_count = sum(1 for _ in itertools.product(*sets))
    #return
    for args in itertools.product(*sets):
        run(*args)
        count += 1
        print(f"\nProgress: {count}/{total_count} : {count/total_count}\n")

main()

'''
with open(f"./csv/paper.csv", 'w') as o_file:
    o_file.write("time, susceptible, exposed, symptomatic_infective, asymptomatic_infective, quarantined_susceptible, quarantined_exposed, quarantined_infective, recovered, deceased\n");
    for s in seird_state_output():
        o_file.write(f"{s['time']}, {s['susceptible']}, {s['exposed']}, {s['symptomatic_infective']}, {s['asymptomatic_infective']}, {s['quarantined_susceptible']}, {s['quarantined_exposed']}, {s['quarantined_infective']}, {s['recovered']}, {s['deceased']}\n")
'''


