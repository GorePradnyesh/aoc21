import sys
import os
import pathlib
import re
import time
import heapq

# Add Common to sys path
sys.path.append(os.path.join(pathlib.Path(__file__).parent.resolve(),'..', 'Common'))

import DataUtils

#####################

class Data_Node:
    def __init__(self, x, y, cost) -> None:
        self.distance = sys.maxsize  # total distance of node from source
        self.cost = cost
        self.visited = False
        self.prev_node = None        
        self.x = x
        self.y = y
    
    def set_distance(self, distance):
        self.distance = distance
    
    def __int__(self):
        return self.distance

    def __str__(self) -> str:
        #return f'({self.x, self.y}) d:{self.distance})'
        if self.distance == sys.maxsize:
            return 'X'
            # return '∞'
        return str(self.distance)
    
    def __format__(self, format_spec):
        return format(str(self), format_spec)

    
    def __eq__(self, __o: object) -> bool:
        if isinstance(__o, Data_Node):
            return (self.distance == __o.distance and self.x == __o.x and self.y == __o.y)
        return False
    
    def __ne__(self, __o: object) -> bool:
        if isinstance(__o, Data_Node):
            return (self.distance != __o.distance or self.x != __o.x or self.y != __o.y)
        return False

    def __gt__(self, __o: object) -> bool:
        if isinstance(__o, Data_Node):
            return (self.distance > __o.distance)
        return False
    
    def __lt__(self, __o: object) -> bool:
        if isinstance(__o, Data_Node):
            return (self.distance < __o.distance)
        return False

    def __ge__(self, __o: object) -> bool:
        if isinstance(__o, Data_Node):
            return (self.distance >= __o.distance)
        return False

    def __le__(self, __o: object) -> bool:
        if isinstance(__o, Data_Node):
            return (self.distance <= __o.distance)
        return False


def set_node_distace(distance_map, pos, distance):
    node = distance_map.get_element(pos.x, pos.y)
    node.set_distance(distance)

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

def move_up(pos):
    return Pos(pos.x, pos.y - 1)

def move_down(pos):
    return Pos(pos.x, pos.y + 1)

def move_right(pos):
    return Pos(pos.x + 1, pos.y)

def move_left(pos):
    return Pos(pos.x - 1, pos.y)

def is_pos_valid(data_map, pos):
    return data_map.is_valid_pos(pos.x, pos.y)

def is_pos_unvisited(distance_map, pos):
    if is_pos_valid(distance_map, pos):
        return not distance_map.get_element(pos.x, pos.y).visited
    return False

def get_unvisited_neighbors(distance_map, cur_pos):
    pos_list = [move_up(cur_pos), move_down(cur_pos), move_right(cur_pos), move_left(cur_pos)] 
    unvisited_neighbors = \
        list(distance_map.get_element(pos.x, pos.y) for pos in pos_list if is_pos_unvisited(distance_map, pos))
    return unvisited_neighbors

#####################

def walk_disjkstra(distance_map, unvisited_list, end_node):
    while True:        
        if len(unvisited_list) == 0:
            print('Visited all nodes. Stopping')
            return

        if end_node.visited:
            print('Visited all neighbors of end node from start')
            return

        # current_node = min(unvisited_list)
        current_node = heapq.heappop(unvisited_list)
        current_distance = current_node.distance

        if current_distance == sys.maxsize:
            print(f'All remaining nodes are infinitely far away')
            return

        distance_modfied = False
        neighbors = get_unvisited_neighbors(distance_map, current_node)
        for node in neighbors:            
            temp_distance = current_node.distance + node.cost
            
            if temp_distance < node.distance:
                node.distance = temp_distance
                node.prev = current_node
                distance_modfied = True
            
            # if not is not present in the PQ, add it
            if node not in unvisited_list:
                heapq.heappush(unvisited_list, node)
        
        # if node distance has been updated, heapify the PQ 
        if distance_modfied:
            heapq.heapify(unvisited_list)

        current_node.visited = True
        print(f'{len(unvisited_list)}\r', end='')
        # print(DataUtils.get_map_string(distance_map, ' ', 2) + '\r', end='')
    pass

def process():
    input_path = os.path.join(pathlib.Path(__file__).parent.resolve(), 'input')    
    filePath = os.path.join(input_path, 'input1.txt')

    with open(filePath) as fp:
        cost_map = DataUtils.build_map(fp, cast_type=int)
        # print(DataUtils.get_map_string(cost_map, ' '))
        
        should_extend_map = True # P15,Pt 2. False for Pt 1.
        if should_extend_map:
            initial_width = cost_map.width
            initial_height = cost_map.height
            def extend_map(x, y):
                increment = int(x / initial_width)
                increment += int(y / initial_height)
                inc_x = x % initial_width
                inc_y = y % initial_height
                new_value = (cost_map.get_element(inc_x, inc_y) + increment) % 9
                new_value = new_value if new_value else 9   #(7+1)->8,(8+1)->9,(9+1)->1,(9+2)->2 
                return new_value if new_value else 1

            
            # extend cost_map tiles
            new_width = cost_map.width * 5
            new_height = cost_map.height * 5
            extended_map = DataUtils.build_map_with_initializer(
                new_width,
                new_height,
                extend_map)
            cost_map = extended_map
            # print(DataUtils.get_map_string(cost_map))

        unvisited_list = []

        def node_initializer(x, y):
            node = Data_Node(x, y, cost_map.get_element(x, y))
            return node

        distance_map = DataUtils.build_map_with_initializer(
            cost_map.width, 
            cost_map.height, 
            node_initializer)

        end_node = distance_map.get_element(distance_map.width - 1, distance_map.height - 1)
        
        start_node = distance_map.get_element(0 ,0)
        start_node.distance = 0
        start_node.cost = 0
        
        heapq.heappush(unvisited_list, start_node)

        st_time = time.perf_counter()
        walk_disjkstra(distance_map, unvisited_list, end_node)
        st_time = time.perf_counter() - st_time
        
        print(f'Distance of end node : {end_node.distance}, time: {st_time}')

       
process()