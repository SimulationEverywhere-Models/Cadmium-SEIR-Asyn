import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

import math
import csv
import os
import datetime

def set_the_floor(x, y=0.01):
    return x#max(x, y)


def one_file(name):


    #with open(f"./csv/{c},{b},{q},{e},{l},{n},{di},{dq},{yi},{ya},{yh},{a},{susceptible},{exposed},{symptomatic},{asymptomatic},seird.csv", 'w') as o_file:
    #o_file.write("time, susceptible, exposed, symptomatic_infective, asymptomatic_infective, quarantined_susceptible, quarantined_exposed, quarantined_infective, recovered, deceased\n");

    #for root, _, files in os.walk("./csv"):
    #    for name in files:

    plt.figure(figsize=(11, 8.5))

    name_parts = name.split(',')
    contact_rate                  = float(name_parts[0])
    exposure_chance               = float(name_parts[1])
    contact_trace_chance          = float(name_parts[2])
    incubation_rate               = float(name_parts[3])
    false_positive_q_release_rate = float(name_parts[4])
    symptom_chance                = float(name_parts[5])
    self_q_rate                   = float(name_parts[6])
    q_incubation_rate             = float(name_parts[7])
    recovery_rate_I               = float(name_parts[8])
    recovery_rate_A               = float(name_parts[9])
    recovery_rate_Q               = float(name_parts[10])
    death_rate                    = float(name_parts[11])
    initial_susceptible           = float(name_parts[12])
    initial_exposed               = float(name_parts[13])
    initial_symptomatic           = float(name_parts[14])
    initial_asymptomatic          = float(name_parts[15])

    plt.title(f"Contact Rate: {contact_rate}, Exposure Chance: {exposure_chance}, Contact Trace Chance: {contact_trace_chance}, Incubation Rate: {incubation_rate}, Q Release Rate: {false_positive_q_release_rate},\n"+
              f"Symptom Chance: {symptom_chance}, Self Q Rate: {self_q_rate}, Q Incubation Rate: {q_incubation_rate}, Recovery Rate I: {recovery_rate_I},\n"+
              f"Recovery Rate A: {recovery_rate_A}, Recovery Rate Q: {recovery_rate_Q}, Death Rate: {death_rate}")

    with open(os.path.join("./csv", name)) as csv:
    #with open("./csv/seird.csv") as csv:
        l_time                    = []
        l_susceptible             = []
        l_exposed                 = []
        l_symptomatic_infective   = []
        l_asymptomatic_infective  = []
        l_quarantined_susceptible = []
        l_quarantined_exposed     = []
        l_quarantined_infective   = []
        l_recovered               = []
        l_deceased                = []

        peek_exposed                 = (0, 0)
        peek_symptomatic_infective   = (0, 0)
        peek_asymptomatic_infective  = (0, 0)
        peek_quarantined_susceptible = (0, 0)
        peek_quarantined_exposed     = (0, 0)
        peek_quarantined_infective   = (0, 0)


        csv.readline()
        for line in csv:
            parts = [float(part) for part in line.split(', ')]
            time                    = parts[0]
            susceptible             = parts[1]
            exposed                 = parts[2]
            symptomatic_infective   = parts[3]
            asymptomatic_infective  = parts[4]
            quarantined_susceptible = parts[5]
            quarantined_exposed     = parts[6]
            quarantined_infective   = parts[7]
            recovered               = parts[8]
            deceased                = parts[9]

            l_time.append(time)
            l_susceptible.append(susceptible)
            l_exposed.append(exposed)
            l_symptomatic_infective.append(symptomatic_infective)
            l_asymptomatic_infective.append(asymptomatic_infective)
            l_quarantined_susceptible.append(quarantined_susceptible)
            l_quarantined_exposed.append(quarantined_exposed)
            l_quarantined_infective.append(quarantined_infective)
            l_recovered.append(recovered)
            l_deceased.append(deceased)

            if peek_exposed[1] < exposed:
                peek_exposed = (time, exposed)
            if peek_symptomatic_infective[1] < symptomatic_infective:
                peek_symptomatic_infective = (time, symptomatic_infective)
            if peek_asymptomatic_infective[1] < asymptomatic_infective:
                peek_asymptomatic_infective = (time, asymptomatic_infective)
            if peek_quarantined_susceptible[1] < quarantined_susceptible:
                peek_quarantined_susceptible = (time, quarantined_susceptible)
            if peek_quarantined_exposed[1] < quarantined_exposed:
                peek_quarantined_exposed = (time, quarantined_exposed)
            if peek_quarantined_infective[1] < quarantined_infective:
                peek_quarantined_infective = (time, quarantined_infective)


            if time > 55:#sum(parts[1:8]) < sum(parts[1:])/1000:
                break

        #plt.scatter(x, y, s=None, c=None, marker=None, cmap=None, norm=None, vmin=None, vmax=None, alpha=None, linewidths=None, verts=None, edgecolors=None, *, plotnonfinite=False, data=None, **kwargs)
        #plt.yscale("log")
        #plt.plot(l_time, l_time,                    label='time')
        #plt.plot(l_time, l_susceptible,             label='Susceptible')
        #plt.plot(l_time, l_exposed,                 label='Exposed')
        #plt.plot(l_time, l_symptomatic_infective,   label='Symptomatic Infective')
        #plt.plot(l_time, l_asymptomatic_infective,  label='Asymptomatic Infective')
        #plt.plot(l_time, l_quarantined_susceptible, label='Quarantined Susceptible')
        #plt.plot(l_time, l_quarantined_exposed,     label='Quarantined Exposed')
        #plt.plot(l_time, l_quarantined_infective,   label='Quarantined Infective')
        #plt.plot(l_time, l_recovered,               label='Recovered')
        #plt.plot(l_time, l_deceased,                label='Deceased')


        plt.plot(l_time, [math.log10(i) for i in l_symptomatic_infective],   label='Symptomatic Infective')

        '''
        total_initial_pop =(l_susceptible[0] +
                            l_exposed[0] +
                            l_symptomatic_infective[0] +
                            l_asymptomatic_infective[0] +
                            l_quarantined_susceptible[0] +
                            l_quarantined_exposed[0] +
                            l_quarantined_infective[0] +
                            l_recovered[0] +
                            l_deceased[0])

        plt.text(l_time[0], total_initial_pop, f"Initial Population: {total_initial_pop}", horizontalalignment='left')
        plt.text(l_time[-1], l_recovered[-1],f"Recovered: {l_recovered[-1]}", horizontalalignment='right')
        plt.text(l_time[-1], l_deceased[-1], f"Deceased: {l_deceased[-1]}",   horizontalalignment='right')

        if peek_exposed[1]                 > 0:
            plt.text(peek_exposed[0],                 peek_exposed[1],                 f"Peak Exposed: {peek_exposed[1]}",                            horizontalalignment='left')
        if peek_symptomatic_infective[1]   > 0:
            plt.text(peek_symptomatic_infective[0],   peek_symptomatic_infective[1],   f"Peak Symptomatic Infective: {peek_symptomatic_infective[1]}",     horizontalalignment='left')
        if peek_asymptomatic_infective[1]  > 0:
            plt.text(peek_asymptomatic_infective[0],  peek_asymptomatic_infective[1],  f"Peak Asymptomatic Infective: {peek_asymptomatic_infective[1]}",   horizontalalignment='left')
        if peek_quarantined_susceptible[1] > 0:
            plt.text(peek_quarantined_susceptible[0], peek_quarantined_susceptible[1], f"Peak Quarantined Susceptible: {peek_quarantined_susceptible[1]}", horizontalalignment='left')
        if peek_quarantined_exposed[1]     > 0:
            plt.text(peek_quarantined_exposed[0],     peek_quarantined_exposed[1],     f"Peak Quarantined Exposed: {peek_quarantined_exposed[1]}",         horizontalalignment='left')
        if peek_quarantined_infective[1]   > 0:
            plt.text(peek_quarantined_infective[0],   peek_quarantined_infective[1],   f"Peak Quarantined Infective: {peek_quarantined_infective[1]}",     horizontalalignment='left')
        '''


        plt.xlim(0, 55)
        plt.ylim(1, 5.5)
        plt.legend()
        plt.show()
        #figname = name[:-len(".csv")]+".png"
        #print(figname)
        #plt.savefig(os.path.join("./figures", figname))
        plt.close()



def main():

    #with open(f"./csv/{c},{b},{q},{e},{l},{n},{di},{dq},{yi},{ya},{yh},{a},{susceptible},{exposed},{symptomatic},{asymptomatic},seird.csv", 'w') as o_file:
    #o_file.write("time, susceptible, exposed, symptomatic_infective, asymptomatic_infective, quarantined_susceptible, quarantined_exposed, quarantined_infective, recovered, deceased\n");

    os.system("mkdir -p ./figures/ ./figures.old/")
    os.system("mv ./figures/* ./figures.old/")
    files = []

    for root, _, names in os.walk("./csv"):
        for name in names:
            name_parts = name.split(',')
            files.append((
                float(name_parts[0]),
                float(name_parts[1]),
                float(name_parts[2]),
                float(name_parts[3]),
                float(name_parts[4]),
                float(name_parts[5]),
                float(name_parts[6]),
                float(name_parts[7]),
                float(name_parts[8]),
                float(name_parts[9]),
                float(name_parts[10]),
                float(name_parts[11]),

                name
            ))

    files.sort()

    for file_number, name_parts in enumerate(files):

        plt.figure(figsize=(11, 8.5))

        contact_rate                  = name_parts[0]
        exposure_chance               = name_parts[1]
        contact_trace_chance          = name_parts[2]
        incubation_rate               = name_parts[3]
        false_positive_q_release_rate = name_parts[4]
        symptom_chance                = name_parts[5]
        self_q_rate                   = name_parts[6]
        q_incubation_rate             = name_parts[7]
        recovery_rate_I               = name_parts[8]
        recovery_rate_A               = name_parts[9]
        recovery_rate_Q               = name_parts[10]
        death_rate                    = name_parts[11]
        name                          = name_parts[12]

        plt.title(f"Contact Rate: {contact_rate}, Exposure Chance: {exposure_chance}, Contact Trace Chance: {contact_trace_chance}, Incubation Rate: {incubation_rate},\n"+
                  f"Q Release Rate: {false_positive_q_release_rate}, Symptom Chance: {symptom_chance}, Self Q Rate: {self_q_rate}, Q Incubation Rate: {q_incubation_rate},\n"
                  f"Recovery Rate I: {recovery_rate_I}, Recovery Rate A: {recovery_rate_A}, Recovery Rate Q: {recovery_rate_Q}, Death Rate: {death_rate}")

        with open(os.path.join(root, name)) as csv:
        #with open("./csv/seird.csv") as csv:
            l_time                    = []
            l_susceptible             = []
            l_exposed                 = []
            l_symptomatic_infective   = []
            l_asymptomatic_infective  = []
            l_quarantined_susceptible = []
            l_quarantined_exposed     = []
            l_quarantined_infective   = []
            l_recovered               = []
            l_deceased                = []

            peek_exposed                 = (0, 0)
            peek_symptomatic_infective   = (0, 0)
            peek_asymptomatic_infective  = (0, 0)
            peek_quarantined_susceptible = (0, 0)
            peek_quarantined_exposed     = (0, 0)
            peek_quarantined_infective   = (0, 0)


            csv.readline()
            for line in csv:
                parts = [float(part) for part in line.split(', ')]
                time                    = parts[0]
                susceptible             = parts[1]
                exposed                 = parts[2]
                symptomatic_infective   = parts[3]
                asymptomatic_infective  = parts[4]
                quarantined_susceptible = parts[5]
                quarantined_exposed     = parts[6]
                quarantined_infective   = parts[7]
                recovered               = parts[8]
                deceased                = parts[9]

                l_time.append(time)
                l_susceptible.append(susceptible)
                l_exposed.append(exposed)
                l_symptomatic_infective.append(symptomatic_infective)
                l_asymptomatic_infective.append(asymptomatic_infective)
                l_quarantined_susceptible.append(quarantined_susceptible)
                l_quarantined_exposed.append(quarantined_exposed)
                l_quarantined_infective.append(quarantined_infective)
                l_recovered.append(recovered)
                l_deceased.append(deceased)

                if peek_exposed[1] < exposed:
                    peek_exposed = (time, exposed)
                if peek_symptomatic_infective[1] < symptomatic_infective:
                    peek_symptomatic_infective = (time, symptomatic_infective)
                if peek_asymptomatic_infective[1] < asymptomatic_infective:
                    peek_asymptomatic_infective = (time, asymptomatic_infective)
                if peek_quarantined_susceptible[1] < quarantined_susceptible:
                    peek_quarantined_susceptible = (time, quarantined_susceptible)
                if peek_quarantined_exposed[1] < quarantined_exposed:
                    peek_quarantined_exposed = (time, quarantined_exposed)
                if peek_quarantined_infective[1] < quarantined_infective:
                    peek_quarantined_infective = (time, quarantined_infective)

                if time > 55:#sum(parts[1:8]) < sum(parts[1:])/1000:
                    break

            #plt.scatter(x, y, s=None, c=None, marker=None, cmap=None, norm=None, vmin=None, vmax=None, alpha=None, linewidths=None, verts=None, edgecolors=None, *, plotnonfinite=False, data=None, **kwargs)
            #plt.yscale("log")
            #plt.plot(l_time, l_time,                    label='time')
            #plt.plot(l_time, l_susceptible,             label='Susceptible')
            #plt.plot(l_time, l_exposed,                 label='Exposed')
            #plt.plot(l_time, l_symptomatic_infective,   label='Symptomatic Infective')
            #plt.plot(l_time, l_asymptomatic_infective,  label='Asymptomatic Infective')
            #plt.plot(l_time, l_quarantined_susceptible, label='Quarantined Susceptible')
            #plt.plot(l_time, l_quarantined_exposed,     label='Quarantined Exposed')
            #plt.plot(l_time, l_quarantined_infective,   label='Quarantined Infective')
            #plt.plot(l_time, l_recovered,               label='Recovered')
            #plt.plot(l_time, l_deceased,                label='Deceased')

            plt.plot(l_time, [math.log10(i) for i in l_symptomatic_infective],   label='Symptomatic Infective')


            '''
            total_initial_pop =(l_susceptible[0] +
                                l_exposed[0] +
                                l_symptomatic_infective[0] +
                                l_asymptomatic_infective[0] +
                                l_quarantined_susceptible[0] +
                                l_quarantined_exposed[0] +
                                l_quarantined_infective[0] +
                                l_recovered[0] +
                                l_deceased[0])

            plt.text(l_time[0], total_initial_pop, f"Initial Population: {total_initial_pop}", horizontalalignment='left')
            plt.text(l_time[-1], l_recovered[-1],f"Recovered: {l_recovered[-1]}", horizontalalignment='right')
            plt.text(l_time[-1], l_deceased[-1], f"Deceased: {l_deceased[-1]}",   horizontalalignment='right')
            if peek_exposed[1]                 > 0:
                plt.text(peek_exposed[0],                 peek_exposed[1],                 f"Peak Exposed: {peek_exposed[1]}",                            horizontalalignment='left')
            if peek_symptomatic_infective[1]   > 0:
                plt.text(peek_symptomatic_infective[0],   peek_symptomatic_infective[1],   f"Peak Symptomatic Infective: {peek_symptomatic_infective[1]}",     horizontalalignment='left')
            if peek_asymptomatic_infective[1]  > 0:
                plt.text(peek_asymptomatic_infective[0],  peek_asymptomatic_infective[1],  f"Peak Asymptomatic Infective: {peek_asymptomatic_infective[1]}",   horizontalalignment='left')
            if peek_quarantined_susceptible[1] > 0:
                plt.text(peek_quarantined_susceptible[0], peek_quarantined_susceptible[1], f"Peak Quarantined Susceptible: {peek_quarantined_susceptible[1]}", horizontalalignment='left')
            if peek_quarantined_exposed[1]     > 0:
                plt.text(peek_quarantined_exposed[0],     peek_quarantined_exposed[1],     f"Peak Quarantined Exposed: {peek_quarantined_exposed[1]}",         horizontalalignment='left')
            if peek_quarantined_infective[1]   > 0:
                plt.text(peek_quarantined_infective[0],   peek_quarantined_infective[1],   f"Peak Quarantined Infective: {peek_quarantined_infective[1]}",     horizontalalignment='left')
            '''

            plt.xlim(0, 55)
            plt.ylim(1, 5.5)
            plt.legend()
            #plt.show()
            figname = f"{file_number:0{1+len(str(len(files)))}},{name[:-len('.csv')]+'.png'}"
            print(figname)
            plt.savefig(os.path.join("./figures", figname))
            plt.close()

    os.system(f"convert -delay 50 -loop 0 -deconstruct ./figures/*.png ./gifs/{datetime.datetime.now().isoformat()}.gif")


def all_in_a():

    #with open(f"./csv/{c},{b},{q},{e},{l},{n},{di},{dq},{yi},{ya},{yh},{a},{susceptible},{exposed},{symptomatic},{asymptomatic},seird.csv", 'w') as o_file:
    #o_file.write("time, susceptible, exposed, symptomatic_infective, asymptomatic_infective, quarantined_susceptible, quarantined_exposed, quarantined_infective, recovered, deceased\n");

    os.system("mkdir -p ./figures/ ./figures.old/")
    os.system("mv ./figures/* ./figures.old/")
    files = []

    for root, _, names in os.walk("./csv_a"):
        for name in names:
            name_parts = name.split(',')
            files.append((
                float(name_parts[0]),
                float(name_parts[1]),
                float(name_parts[2]),
                float(name_parts[3]),
                float(name_parts[4]),
                float(name_parts[5]),
                float(name_parts[6]),
                float(name_parts[7]),
                float(name_parts[8]),
                float(name_parts[9]),
                float(name_parts[10]),
                float(name_parts[11]),

                name
            ))

    files.sort()


    #plt.figure(figsize=(11, 8.5))

    data_sets = []

    for file_number, name_parts in enumerate(files):


        contact_rate                  = name_parts[0]
        exposure_chance               = name_parts[1]
        contact_trace_chance          = name_parts[2]
        incubation_rate               = name_parts[3]
        false_positive_q_release_rate = name_parts[4]
        symptom_chance                = name_parts[5]
        self_q_rate                   = name_parts[6]
        q_incubation_rate             = name_parts[7]
        recovery_rate_I               = name_parts[8]
        recovery_rate_A               = name_parts[9]
        recovery_rate_Q               = name_parts[10]
        death_rate                    = name_parts[11]
        name                          = name_parts[12]

        #plt.title(f"Contact Rate: {contact_rate}, Exposure Chance: {exposure_chance}, Contact Trace Chance: {contact_trace_chance}, Incubation Rate: {incubation_rate},\n"+
        #          f"Q Release Rate: {false_positive_q_release_rate}, Symptom Chance: {symptom_chance}, Self Q Rate: {self_q_rate}, Q Incubation Rate: {q_incubation_rate},\n"
        #          f"Recovery Rate I: {recovery_rate_I}, Recovery Rate A: {recovery_rate_A}, Recovery Rate Q: {recovery_rate_Q}, Death Rate: {death_rate}")

        with open(os.path.join(root, name)) as csv:
        #with open("./csv/seird.csv") as csv:
            l_time                    = []
            l_susceptible             = []
            l_exposed                 = []
            l_symptomatic_infective   = []
            l_asymptomatic_infective  = []
            l_quarantined_susceptible = []
            l_quarantined_exposed     = []
            l_quarantined_infective   = []
            l_recovered               = []
            l_deceased                = []

            peek_exposed                 = (0, 0)
            peek_symptomatic_infective   = (0, 0)
            peek_asymptomatic_infective  = (0, 0)
            peek_quarantined_susceptible = (0, 0)
            peek_quarantined_exposed     = (0, 0)
            peek_quarantined_infective   = (0, 0)


            csv.readline()
            for line in csv:
                parts = [float(part) for part in line.split(', ')]
                time                    = parts[0]
                susceptible             = parts[1]
                exposed                 = parts[2]
                symptomatic_infective   = parts[3]
                asymptomatic_infective  = parts[4]
                quarantined_susceptible = parts[5]
                quarantined_exposed     = parts[6]
                quarantined_infective   = parts[7]
                recovered               = parts[8]
                deceased                = parts[9]

                l_time.append(time)
                l_susceptible.append(susceptible)
                l_exposed.append(exposed)
                l_symptomatic_infective.append(symptomatic_infective)
                l_asymptomatic_infective.append(asymptomatic_infective)
                l_quarantined_susceptible.append(quarantined_susceptible)
                l_quarantined_exposed.append(quarantined_exposed)
                l_quarantined_infective.append(quarantined_infective)
                l_recovered.append(recovered)
                l_deceased.append(deceased)

                if peek_exposed[1] < exposed:
                    peek_exposed = (time, exposed)
                if peek_symptomatic_infective[1] < symptomatic_infective:
                    peek_symptomatic_infective = (time, symptomatic_infective)
                if peek_asymptomatic_infective[1] < asymptomatic_infective:
                    peek_asymptomatic_infective = (time, asymptomatic_infective)
                if peek_quarantined_susceptible[1] < quarantined_susceptible:
                    peek_quarantined_susceptible = (time, quarantined_susceptible)
                if peek_quarantined_exposed[1] < quarantined_exposed:
                    peek_quarantined_exposed = (time, quarantined_exposed)
                if peek_quarantined_infective[1] < quarantined_infective:
                    peek_quarantined_infective = (time, quarantined_infective)

                if time > 55:#sum(parts[1:8]) < sum(parts[1:])/1000:
                    break


            data_sets.append((contact_rate, l_time, [math.log10(i) for i in l_symptomatic_infective]))


    #plt.show()
    max_contact_rate = max([ds[0] for ds in data_sets])
    for data_set, color in zip(reversed(data_sets), ( (0.0,0.0,0.0), (1.0,0.0,0.0), (0.0,0.9,0.0), (0.0,0.7,1.0), (0.8,0.0,0.8) )):
        cr, l_time, l_s_i = data_set
        plt.plot(l_time, l_s_i, color=color, label=f"{cr/max_contact_rate}c")
    plt.title("Figure A, Recreated")
    plt.xlim(0, 55)
    plt.ylim(1, 5.5)
    plt.legend()

    figname = "contact_rate.png"#f"{file_number:0{1+len(str(len(files)))}},{name[:-len('.csv')]+'.png'}"
    print(figname)
    plt.savefig(os.path.join("./figures", figname))
    plt.close()

    #os.system(f"convert -delay 50 -loop 0 -deconstruct ./figures/*.png ./gifs/{datetime.datetime.now().isoformat()}.gif")

def all_in_b():

    #with open(f"./csv/{c},{b},{q},{e},{l},{n},{di},{dq},{yi},{ya},{yh},{a},{susceptible},{exposed},{symptomatic},{asymptomatic},seird.csv", 'w') as o_file:
    #o_file.write("time, susceptible, exposed, symptomatic_infective, asymptomatic_infective, quarantined_susceptible, quarantined_exposed, quarantined_infective, recovered, deceased\n");

    os.system("mkdir -p ./figures/ ./figures.old/")
    os.system("mv ./figures/* ./figures.old/")
    files = []

    for root, _, names in os.walk("./csv_b"):
        for name in names:
            name_parts = name.split(',')
            files.append((
                float(name_parts[0]),
                float(name_parts[1]),
                float(name_parts[2]),
                float(name_parts[3]),
                float(name_parts[4]),
                float(name_parts[5]),
                float(name_parts[6]),
                float(name_parts[7]),
                float(name_parts[8]),
                float(name_parts[9]),
                float(name_parts[10]),
                float(name_parts[11]),

                name
            ))

    files.sort()


    #plt.figure(figsize=(11, 8.5))

    data_sets = []

    for file_number, name_parts in enumerate(files):


        contact_rate                  = name_parts[0]
        exposure_chance               = name_parts[1]
        contact_trace_chance          = name_parts[2]
        incubation_rate               = name_parts[3]
        false_positive_q_release_rate = name_parts[4]
        symptom_chance                = name_parts[5]
        self_q_rate                   = name_parts[6]
        q_incubation_rate             = name_parts[7]
        recovery_rate_I               = name_parts[8]
        recovery_rate_A               = name_parts[9]
        recovery_rate_Q               = name_parts[10]
        death_rate                    = name_parts[11]
        A_to_I_scaler_factor          = name_parts[12]
        name                          = name_parts[13]

        #plt.title(f"Contact Rate: {contact_rate}, Exposure Chance: {exposure_chance}, Contact Trace Chance: {contact_trace_chance}, Incubation Rate: {incubation_rate},\n"+
        #          f"Q Release Rate: {false_positive_q_release_rate}, Symptom Chance: {symptom_chance}, Self Q Rate: {self_q_rate}, Q Incubation Rate: {q_incubation_rate},\n"
        #          f"Recovery Rate I: {recovery_rate_I}, Recovery Rate A: {recovery_rate_A}, Recovery Rate Q: {recovery_rate_Q}, Death Rate: {death_rate}, A to I scaler: {A_to_I_scaler_factor}")

        with open(os.path.join(root, name)) as csv:
        #with open("./csv/seird.csv") as csv:
            l_time                    = []
            l_susceptible             = []
            l_exposed                 = []
            l_symptomatic_infective   = []
            l_asymptomatic_infective  = []
            l_quarantined_susceptible = []
            l_quarantined_exposed     = []
            l_quarantined_infective   = []
            l_recovered               = []
            l_deceased                = []

            peek_exposed                 = (0, 0)
            peek_symptomatic_infective   = (0, 0)
            peek_asymptomatic_infective  = (0, 0)
            peek_quarantined_susceptible = (0, 0)
            peek_quarantined_exposed     = (0, 0)
            peek_quarantined_infective   = (0, 0)


            csv.readline()
            for line in csv:
                parts = [float(part) for part in line.split(', ')]
                time                    = parts[0]
                susceptible             = parts[1]
                exposed                 = parts[2]
                symptomatic_infective   = parts[3]
                asymptomatic_infective  = parts[4]
                quarantined_susceptible = parts[5]
                quarantined_exposed     = parts[6]
                quarantined_infective   = parts[7]
                recovered               = parts[8]
                deceased                = parts[9]

                l_time.append(time)
                l_susceptible.append(susceptible)
                l_exposed.append(exposed)
                l_symptomatic_infective.append(symptomatic_infective)
                l_asymptomatic_infective.append(asymptomatic_infective)
                l_quarantined_susceptible.append(quarantined_susceptible)
                l_quarantined_exposed.append(quarantined_exposed)
                l_quarantined_infective.append(quarantined_infective)
                l_recovered.append(recovered)
                l_deceased.append(deceased)

                if peek_exposed[1] < exposed:
                    peek_exposed = (time, exposed)
                if peek_symptomatic_infective[1] < symptomatic_infective:
                    peek_symptomatic_infective = (time, symptomatic_infective)
                if peek_asymptomatic_infective[1] < asymptomatic_infective:
                    peek_asymptomatic_infective = (time, asymptomatic_infective)
                if peek_quarantined_susceptible[1] < quarantined_susceptible:
                    peek_quarantined_susceptible = (time, quarantined_susceptible)
                if peek_quarantined_exposed[1] < quarantined_exposed:
                    peek_quarantined_exposed = (time, quarantined_exposed)
                if peek_quarantined_infective[1] < quarantined_infective:
                    peek_quarantined_infective = (time, quarantined_infective)

                if time > 55:#sum(parts[1:8]) < sum(parts[1:])/1000:
                    break


            data_sets.append((contact_trace_chance, l_time, [math.log10(i) for i in l_symptomatic_infective]))


    #plt.show()
    max_contact_rate = min([ds[0] for ds in data_sets])
    for data_set, color in zip(data_sets, ( (0.0,0.0,0.0), (1.0,0.0,0.0), (0.0,0.9,0.0), (0.0,0.7,1.0), (0.8,0.0,0.8) )):
        cr, l_time, l_s_i = data_set
        plt.plot(l_time, l_s_i, color=color, label=f"{cr/max_contact_rate}q")
    plt.title("Figure C, Recreated")
    plt.xlim(0, 55)
    plt.ylim(1, 5.5)
    plt.legend()

    figname = "contact_trace_chance.png"#f"{file_number:0{1+len(str(len(files)))}},{name[:-len('.csv')]+'.png'}"
    print(figname)
    plt.savefig(os.path.join("./figures", figname))
    plt.close()

    #os.system(f"convert -delay 50 -loop 0 -deconstruct ./figures/*.png ./gifs/{datetime.datetime.now().isoformat()}.gif")


#all_in_a()
all_in_b()
#main()
#one_file("14.781,2.1011e-08,1.8887e-07,0.14285714285714285,0.07142857142857142,0.86834,0.13266,0.1259,0.33029,0.13978,0.11624,1.7826e-05,11081000,105.1,27.679,53.839,seird.csv")
