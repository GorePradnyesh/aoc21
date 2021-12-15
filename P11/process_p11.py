import sys
import os
import pathlib

# Add Common to sys path
sys.path.append(os.path.join(pathlib.Path(__file__).parent.resolve(),'..', 'Common'))

import DataUtils

###############

def increment_map_for_flash(data_map):
    flash_positions = []
    for y in range(data_map.height):
        for x in range(data_map.width):
            inc = data_map.get_element(x, y) + 1
            if inc > 9:
                flash_positions.append((x, y))
            data_map.set_element(x, y, inc)
    return flash_positions

def process_flash(data_map, flash_positions):
    while len(flash_positions) > 0:
        pos = flash_positions.pop(0)
        # print(f'Process flash {pos}')
        neighbor_pos = DataUtils.get_neighbor_pos(data_map, pos[0], pos[1])  
        for neighbor in neighbor_pos:
            element = data_map.get_element(neighbor[0], neighbor[1])
            if element == 9 and neighbor not in flash_positions:
                flash_positions.append(neighbor)
            data_map.set_element(neighbor[0], neighbor[1], element + 1)
        # print(DataUtils.get_map_string(data_map, element_separator=' ', format_width=2))
            
def reset_flashed(data_map):
    flashed_pos = []
    for y in range(data_map.height):
        for x in range(data_map.width):
            if data_map.get_element(x, y) > 9:
                data_map.set_element(x, y, 0)
                flashed_pos.append((x,y))
    return flashed_pos

def process_step(data_map):    
    # print(f'---------------------- Step ----------------------')
    # print(DataUtils.get_map_string(data_map, element_separator=' ', format_width=2))
    
    # print(f'Increment')
    flash_positions = increment_map_for_flash(data_map)
    # print(DataUtils.get_map_string(data_map, element_separator=' ', format_width=2))
    
    
    process_flash(data_map, flash_positions)
    
    # print(f'Reset')
    flashed_pos = reset_flashed(data_map)    
    # print(DataUtils.get_map_string(data_map, element_separator=' ', format_width=2))
    return len(flashed_pos)

###############

def process_steps(data_map, step_count):
    first_flash = None
    flash_count = 0
    for index in range(step_count):
            step_flash_count = process_step(data_map)
            if step_flash_count == len(data_map.oneD_elements) and not first_flash:
                first_flash = index + 1
            flash_count += step_flash_count
            # print(f'============= Step {index + 1} ==============')
            # print(DataUtils.get_map_string(data_map, element_separator='', format_width=1))
    print(f'After {step_count} steps there have been {flash_count} flashes, first flash: {first_flash}')

def process():
    filePath = '/Users/pgore/dev/AOC21/P11/input/input1.txt'
    step_count = 100    
    with open(filePath) as fp:
        data_map = DataUtils.build_map(fp, cast_type=int)
        process_steps(data_map, 100)
        # pass in 300 to get first flash for input1       
        # alternatively one could run the process_step until a full flash happens. too lazy now that we know the solution :P 
        
process()