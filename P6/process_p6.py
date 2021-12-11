
##################################################
from os import read


def read_initial_data(fp_handle):
    initial_data = fp_handle.readline()
    return initial_data.split(',')

def calculate_population(fp_handle, day_count):
    day_dict = {0:0, 1:0, 2:0, 3:0, 4:0, 5:0, 6:0, 7:0, 8:0}    
    # Set up initial data
    initial_data = read_initial_data(fp_handle)
    for counter in initial_data:
        day_dict[int(counter)] += 1

    for day in range(day_count):
        swap_dict = {0:0, 1:0, 2:0, 3:0, 4:0, 5:0, 6:0, 7:0, 8:0}
        for key in day_dict.keys():
            ct = day_dict[key]
            if key == 0:
                swap_dict[8] = ct
                swap_dict[6] += ct
            else:
                swap_dict[key - 1] += ct
        day_dict = swap_dict
        # print(f'Fish after {day_count} days ', sum(day_dict.values()))
    print(f'Fish after {day_count} days ', sum(day_dict.values()))

def process():
    filePath = '/Users/pgore/dev/AOC21/P6/input/input1.txt'
    with open(filePath) as fp:
        calculate_population(fp, 256)

process()