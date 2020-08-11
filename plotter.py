import matplotlib.pyplot as plt
import runner
import re
import math
import itertools
import os


def cad_time(line_time):
    if line_time in [None, '', '\n']:
        return None
    time_parts = line_time.split(':')
    return int(time_parts[0])*(60*60*1000) + int(time_parts[1])*(60*1000) + int(time_parts[2])*(1000) + int(time_parts[3])

def args_from_input_string(intput_string):
    ar = {}

    for line in intput_string.split('\n'):
        if match := re.match(r"^[\s]*([^=;\n\r]+?)\s*=\s*([^;\n\r]+?)\s*(([;\n\r].*)|$)", line):
            ar[match.group(1)] = float(match.group(2))

    return ar#tuple([ar[key] for key in ['c', 'b', 'q', 'e', 'l', 'n', 'di', 'dq', 'yi', 'ya', 'yh', 'a', 't', 'S', 'E', 'I', 'A', 'Sq', 'Eq', 'H', 'R', 'D']])

def input_string_from_args(c, b, q, e, l, n, di, dq, yi, ya, yh, a, t, S, E, I, A, Sq, Eq, H, R, D):
    return f"c={c};\nb={b};\nq={q};\ne={e};\nl={l};\nn={n};\ndi={di};\ndq={dq};\nyi={yi};\nya={ya};\nyh={yh};\na={a};\nt={t};\nS={S};\nE={E};\nI={I};\nA={A};\nSq={Sq};\nEq={Eq};\nH={H};\nR={R};\nD={D};"

def input_string_from_state_and_args(state, c, b, q, e, l, n, di, dq, yi, ya, yh, a, t):
    return input_string_from_args(c, b, q, e, l, n, di, dq, yi, ya, yh, a, t, state['S'], state['E'], state['I'], state['A'], state['Sq'], state['Eq'], state['H'], state['R'], state['D'])

def state_at_time(state_file, time_offset):
    state = None
    for new_state in parse_state(state_file):
        state = new_state
        if state['time'] >= time_offset:
            return state
    return state

def parse_state(state_file, *, time_offset=0.0, stop_at=None):
    while True:
        line_time = state_file.readline()
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

        yield { "time":time+time_offset,
                "S" :susceptible,
                "E" :exposed,
                "I" :symptomatic_infective,
                "A" :asymptomatic_infective,
                "Sq":quarantined_susceptible,
                "Eq":quarantined_exposed,
                "H" :quarantined_infective,
                "R" :recovered,
                "D" :deceased
                }
        if stop_at is not None and time+time_offset >= stop_at:
            return


def all_in_a(base_paths, path_dicts, c_proportions):
    base_times = []
    base_I     = []
    theta = None

    with open(base_paths['input']) as base_i_input_file:
        theta = args_from_input_string(base_i_input_file.read())['t']


    with open(base_paths['state']) as base_i_file:
        for state in parse_state(base_i_file, stop_at=12):
            base_times.append(state['time'])
            base_I.append(math.log10(state['I']))

    for index, paths in enumerate(path_dicts):
        print(index, paths['root'])
        other_times = []
        other_I     = []
        with open(paths['state']) as i_file:
            for state in parse_state(i_file, time_offset = base_times[-1]):
                other_times.append(state['time'])
                other_I.append(math.log10(max(state['I'], 1)))
        plt.plot(other_times, other_I, color=[(0.0,0.0,0.0), (1.0,0.0,0.0), (0.0,0.9,0.0), (0.0,0.7,1.0), (0.8,0.0,0.8)][index%5], label=f"{list(c_proportions)[index]}c")
    plt.legend()
    plt.plot(base_times, base_I, color=(0.0,0.0,0.0), label=None)
    plt.xlim(0, 55)
    plt.ylim(1, 5.5)
    plt.title(f"Theta {theta:01.02f}")

    figname = f"figure_{theta:01.02f}.png"#f"{file_number:0{1+len(str(len(files)))}},{name[:-len('.csv')]+'.png'}"
    fig_path = os.path.join(base_paths['root'], figname)
    print(fig_path)
    plt.savefig(fig_path)
    plt.close()
    return fig_path


print("The start")
offset_time_t = 12.0
thetas = [0]#, 0.2, 0.4, 0.6, 0.8, 0.9, 0.95, 1.0, 1.05, 1.1, 1.2, 1.4, 1.6, 1.8, 2.0]
base_paths_list = (runner.run_sim_batched([input_string_from_args(14.781, 2.1011e-8, 1.8887e-7, 1/7, 1/14, 0.86834, 0.13266, 0.1259, 0.33029, 0.13978, 0.11624, 1.7826e-5, theta, 11081000, 105.1, 27.679, 53.839, 739, 1.1642, 1, 2, 0) for theta in thetas], 1, "theta_base"))
print("base_paths_list made")
for base_paths, theta in zip(base_paths_list, thetas):
    print(f"starting {theta:01.02f} {base_paths['root']}")
    with open(base_paths['state']) as base_state_file:
        state = state_at_time(base_state_file, offset_time_t)
        scales = [1.0, 0.8, 0.5, 0.3, 0.1]
        alt_paths = (runner.run_sim_batched([(input_string_from_state_and_args(state, scale*14.781, 2.1011e-8, 1.8887e-7, 1/7, 1/14, 0.86834, 0.13266, 0.1259, 0.33029, 0.13978, 0.11624, 1.7826e-5, theta)) for scale in scales], -1, f"theta_{theta:01.02f}"))
        print("done", all_in_a(base_paths, alt_paths, scales))







#print(args_from_input_string(input_string_from_args(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22)))
