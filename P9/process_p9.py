

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
            s += ' '
        s += '\n'
    return s


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
        

process()