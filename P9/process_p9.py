

class Map:
    def __init__(self, oneD_elements, width, height) -> None:
        self.oneD_elements = oneD_elements
        self.width = width
        self.height = height
    
    def get_element(self, x, y):
        if x < 0 or y < 0:
            return None
        if x >= self.width or y >= self.height:
            return None
        return self.oneD_elements[y * self.width + x]   
    
    def set_element(self, x, y, value):
        if x < 0 or y < 0:
            return None
        if x >= self.width or y >= self.height:
            return None
        self.oneD_elements[y * self.width + x] = value

    def is_valid_pos(self, x, y):
        if x < 0 or y < 0:
            return False
        if x >= self.width or y >= self.height:
            return False
        return True


#############################################

def build_map(fp):
    map_lines = fp.readlines()
    map_lines = list(line.strip() for line in map_lines)
    map_elements = []
    width = 0
    height = len(map_lines)
    for map_line in map_lines:
        if width == 0:
            width = len(map_line)
        map_elements += list(map_line)
    return Map(map_elements, width, height)


def print_map(map):
    s = ''
    for y in range(map.height):
        for x in range(map.width):
            s += str(map.get_element(x,y))
            # s += ''
        s += '\n'
    return s

#currently mutates the map
def print_basins(map, basin_list):
    for basin in basin_list:
        for point in basin:
            map.set_element(point[0], point[1], '*')
    map_s = print_map(map)
    print(map_s)

def get_neighbors(map, x, y, diagonal = False):
    neighbors = []
    # get_element will return none if outside the bounds
    neighbors.append(map.get_element(x - 1, y))
    neighbors.append(map.get_element(x + 1, y))
    neighbors.append(map.get_element(x, y - 1))
    neighbors.append(map.get_element(x, y + 1))
    # diagonals unused
    neighbors = list(element for element in neighbors if element) # remove None elements
    return neighbors

# no diagonals considered
def get_neighbor_pos(map, x, y):
    neighbor_pos = [(x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)]    
    neighbor_pos = list(pos for pos in neighbor_pos if map.is_valid_pos(pos[0], pos[1])) # remove None elements
    return neighbor_pos

# Naive appraoch where overlap is not accounted for.!! 
def get_basin(map, low_point, basin_positions):
    if low_point not in basin_positions:        
        basin_positions += [low_point]
    # Get valid neighbors
    neighbors = get_neighbor_pos(map, low_point[0], low_point[1])

    basin_neighbors = []
    for pos in neighbors:
        if pos not in basin_positions and map.get_element(pos[0], pos[1]) != '9':
            basin_neighbors.append(pos)

    basin_positions += basin_neighbors
    # recurse to neighbors
    for next_pos in basin_neighbors:
        get_basin(map, next_pos, basin_positions)

def get_low_points(map, diagonal = False):
    min_points = []
    for y in range(map.height):
        for x in range(map.width):
            neighbors = get_neighbors(map, x, y, diagonal)
            current_element = map.get_element(x, y)
            if current_element < min(neighbors):
                min_points.append((x, y))
    return min_points

def process():
    filePath = '/Users/pgore/dev/AOC21/P9/input/input1.txt'
    with open(filePath) as fp:
        map = build_map(fp)
        low_points = get_low_points(map)
        low_sum = sum(int(map.get_element(pos[0], pos[1])) + 1 for pos in low_points)
        print(low_sum)

        basin_list = []
        for point in low_points:
            basin_positions = []
            get_basin(map, point, basin_positions)
            # print(f'value for low point {point} : {len(basin_positions)}' )
            basin_list.append(basin_positions)
        basin_list.sort(reverse=True, key=lambda x: len(x))
        
        print(f'Final sum : {len(basin_list[0]) * len(basin_list[1]) * len(basin_list[2])}')
        
        print_basins(map, basin_list)

process()