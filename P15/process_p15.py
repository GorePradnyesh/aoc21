import sys
import os
import pathlib
import re


# Add Common to sys path
sys.path.append(os.path.join(pathlib.Path(__file__).parent.resolve(),'..', 'Common'))

import DataUtils

#####################

class Pos:
    def __init__(self, x, y, z=0) -> None:
        self.x = x 
        self.y = y
        self.z = z

    def __eq__(self, __o: object) -> bool:
        if isinstance(__o, Pos):
            return (self.x == __o.x and self.y == __o.y and self.y == __o.y)
        return False

    def __ne__(self, __o: object) -> bool:
        if isinstance(__o, Pos):
            return not (self.x == __o.x and self.y == __o.y and self.y == __o.y)
        return True
    
    def __repr__(self) -> str:
        if self.z:
            return f'({self.x, self.y, self.z})'
        return f'({self.x, self.y})'

def move_down(pos):
    return Pos(pos.x, pos.y + 1)

def move_right(pos):
    return Pos(pos.x + 1, pos.y)

def is_pos_valid(data_map, pos):
    return data_map.is_valid_pos(pos.x, pos.y)

def get_neighbors(data_map, cur_pos):
    neighbors = []
    down_pos = move_down(cur_pos)
    if is_pos_valid(data_map, down_pos):
        neighbors.append(down_pos)
    
    right_pos = move_right(cur_pos)    
    if is_pos_valid(data_map, right_pos):
        neighbors.append(right_pos)
    
    return neighbors

## optimization is that dont save the whole path. Save next path object, and follow based on cost
class CachedCost:
    def __init__(self, cost:int, path) -> None:
        self.cost = cost
        self.path = path

    def __int__(self):
        return self.cost

######################

def get_path_cost(data_map, cost_map, cur_pos, end_pos, use_caching=True):
    if cur_pos == end_pos:
        return data_map.get_element(cur_pos.x, cur_pos.y), [cur_pos]
    
    # check if this is cached. Avoid running through this
    if use_caching and cost_map.get_element(cur_pos.x, cur_pos.y) != None:
        # print(f'Found cached cost and path for node {cur_pos}')
        cached_pos = cost_map.get_element(cur_pos.x, cur_pos.y)
        return cached_pos.cost, cached_pos.path

    neighbors = get_neighbors(data_map, cur_pos)
    min_cost = sys.maxsize
    min_path = None
    for pos in neighbors:        
        cost,path = get_path_cost(data_map, cost_map, pos, end_pos, use_caching)
        if cost <= min_cost:
            min_cost = cost
            min_path = path
    
    if len(min_path):
        # self cost plus path of downstream min path
        current_min_cost = min_cost + data_map.get_element(cur_pos.x, cur_pos.y)
        current_min_path = [cur_pos] + min_path
        if use_caching:
            cost_map.set_element(cur_pos.x, cur_pos.y, CachedCost(current_min_cost, current_min_path))
        # print(f'Caching cost for pos: {cur_pos}, cost:{current_min_cost}')

        # print(f'Min path for f{cur_pos} : f{current_min_path}')
    else:
        print(f'Unexpected. No min path found !')
        exit -1

    return current_min_cost,current_min_path

def presence_replace(element, presence_list, character):
    if element in presence_list:
        return character

#####################
def process():
    input_path = os.path.join(pathlib.Path(__file__).parent.resolve(), 'input')    
    filePath = os.path.join(input_path, 'input1.txt')

    with open(filePath) as fp:
        data_map = DataUtils.build_map(fp, cast_type=int)
        print(DataUtils.get_map_string(data_map))

        start_pos = Pos(0,0)
        end_pos = Pos(data_map.width - 1, data_map.height - 1)

        cost_map = DataUtils.build_init_map(data_map.width, data_map.height, None)
        total_cost, min_path = get_path_cost(data_map, cost_map, start_pos, end_pos, use_caching=True)
                
        def replace(x, y, element):
            if Pos(x, y) in min_path:
                return ' '
            return element

        print(f'NOTE: CURRENT ANSWER IS INCORRECT. THIS IS WIP')
        print(f'Total cost w/o start: {total_cost - data_map.get_element(start_pos.x, start_pos.y)}')
        
        DataUtils.mutate_map_with_operation(data_map, replace)
        print('---MAP----')
        print(DataUtils.get_map_string(data_map, ''))

       
process()