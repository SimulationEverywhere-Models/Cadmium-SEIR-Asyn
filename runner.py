#runner.py by Griffin Barrett

import os
import subprocess
import itertools
import json
import secrets

def _write_input(data, name, token, index):
    #print(name, token, index)
    path = os.path.join(".", "runs", (f"set_{name}_{token}" if name else f"set_{token}"), f"data_{index:04}")
    os.makedirs(path)
    with open(os.path.join(path, "input.txt"), "w") as input_file:
        input_file.write(data)
    return os.path.abspath(path), 3

def _run_sim(executable_name, data, name, token, index):
    path, depth = _write_input(data, name, token, index)
    #if os.name == 'nt':
    #    return subprocess.Popen(f"cd {path} && {os.path.join(*itertools.repeat('..', depth), 'bin', 'hoya.exe')} 'input.json' > cout.log 2> cerr.log", shell=True), path, index
    #return subprocess.Popen(f"cd {path}; sleep {secrets.choice(list(range(1)))}; {os.path.join(*itertools.repeat('..', depth), 'bin', 'hoya')} './input.json' > ./cout.log 2> ./cerr.log", shell=True), path, index
    #else:
    with open(os.path.join(path, 'cout.log'), 'w') as cout, open(os.path.join(path, 'cerr.log'), 'w') as cerr:
        return subprocess.Popen([os.path.join(*itertools.repeat("..", depth), 'bin', executable_name), 'input.txt'], cwd=path, stdout=cout, stderr=cerr), {
            'root'    :path,
            'cout'    :os.path.join(path, 'cout.log'),
            'cerr'    :os.path.join(path, 'cerr.log'),
            'input'   :os.path.join(path, 'input.txt'),
            'state'   :os.path.join(path, 'output_state.txt'),
            'messages':os.path.join(path, 'output_messages.txt')}, index

def _run_next(executable_name, name, data_set):
    token = secrets.token_hex(16)
    for index, data in enumerate(data_set):
        yield _run_sim(executable_name, data, name, token, index)

def run_sim_batched(executable_name, data_set, batch_size = 1, name = None):
    #batch_size = int(0 if batch_size < 0 else batch_size)
    sentinel = object()
    sims = _run_next(executable_name, name, data_set)

    if batch_size < 0:
        for p in list(sims):
            p[0].wait()
            yield p[1]
    elif batch_size == 0:
        for p in sims:
            p[0].wait()
            yield p[1]
    elif batch_size == 1:
        last_sim = None
        for p in sims:
            if last_sim:
                yield last_sim[1]
            p[0].wait()
            last_sim = p
        yield last_sim[1]
    else:
        active_p = []
        sentinel = object()
        next_p_to_yield = None
        more_to_come = True

        while more_to_come:
            while more_to_come and len(active_p) < batch_size:
                if (new_p := next(sims, sentinel)) is not sentinel:
                    active_p.append(new_p)
                else:
                    more_to_come = False
            if next_p_to_yield is not None:
                yield next_p_to_yield
            active_p[0][0].wait()
            next_p_to_yield = active_p.pop(0)[1]
        if next_p_to_yield is not None:
            yield next_p_to_yield
        for old_p in active_p:
            old_p[0].wait()
            yield old_p[1]


def run_sim_once(data, name = None):
    return list(run_sim_batched([data], 0, name))[0][1];









