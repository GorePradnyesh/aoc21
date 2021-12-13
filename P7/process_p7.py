
##################################################
from os import read
import statistics

def read_initial_data(fp_handle):
    initial_data = fp_handle.readline()
    return initial_data.split(',')

def calculate_fuel_cost(fp_handle):
    # Set up initial data
    initial_data = read_initial_data(fp_handle)
    initial_data = [int(value) for value in initial_data]
    mean_d = int(statistics.mean(initial_data))
    min_d = min(initial_data)
    max_d = max(initial_data)
    stdev_d = int(statistics.stdev(initial_data))
    
    low_chec = max(min_d, mean_d - stdev_d)
    high_check = min(mean_d + stdev_d, max_d)
    print(f'Mean : {mean_d}, stdeve: {stdev_d}, Check range: {low_chec}=>{high_check}')
    
    min_cost = 0
    min_cost_pos = 0

    for check_d in range(low_chec, high_check + 1):
        cost = sum(abs(value - check_d) for value in initial_data)
        if min_cost == 0 or cost < min_cost:
            min_cost = cost
            min_cost_pos = check_d

    print(f'Total cost for moving to {min_cost_pos} is {min_cost}')
        

def process():
    filePath = '/Users/pgore/dev/AOC21/P7/input/input1.txt'
    with open(filePath) as fp:
        calculate_fuel_cost(fp)

process()