import sys
import os
import pathlib
import re
# Add Common to sys path
sys.path.append(os.path.join(pathlib.Path(__file__).parent.resolve(),'..', 'Common'))

import DataUtils

###############

fold_regex = re.compile('fold along ([xy])=(\d+)')

class Fold:
    def __init__(self, axis, line_index) -> None:
        self.axis = axis
        self.line_index = int(line_index)
    
    def __repr__(self) -> str:
        return f'Fold {self.axis} at {self.line_index}'

def get_folds(fold_lines):
    folds = []
    for line in fold_lines:
        g = fold_regex.search(line)
        folds.append(Fold(g.group(1), g.group(2)))
    return folds

def get_dims(positions):
    max_x = 0
    max_y = 0
    for pos in positions:
        if pos[0] > max_x:
            max_x = pos[0]
        if pos[1] > max_y:
            max_y = pos[1]
    return max_x, max_y

def process_positions(positions, fold_lines, fold_count):
    folds = get_folds(fold_lines)    
    max_x, max_y = get_dims(positions)
    # do this for all folds
    for index in range(fold_count):
        fold = folds[index]
        axis_index = 1 if fold.axis == 'y' else 0
        # walk positions
        updated_positions = set()
        for position in positions:
            if position[axis_index] > fold.line_index:
                distance = position[axis_index] - fold.line_index
                new_pos_axis = position[axis_index] - (distance * 2)
                new_pos = [0,0]
                new_pos[axis_index] = new_pos_axis
                new_pos[1-axis_index] = position[1-axis_index]                
                updated_positions.add(tuple(new_pos))
            else:
                updated_positions.add(position)
        positions = updated_positions
    return positions
     

def process():
    input_path = os.path.join(pathlib.Path(__file__).parent.resolve(), 'input')    
    filePath = os.path.join(input_path, 'input1.txt')
    
    reobj = re.compile('\d+,\d+')

    with open(filePath) as fp:
        data_lines = fp.readlines()
        fold_lines = list(line.strip() for line in data_lines if 'fold' in line)
        positions = set(
            (int(line.strip().split(',')[0]), int(line.strip().split(',')[1])) # tuple of ints
            for line in data_lines 
            if re.match(reobj, line))
        


        folded_positions = process_positions(positions, fold_lines.copy(), 1)
        print(len(folded_positions))

process()