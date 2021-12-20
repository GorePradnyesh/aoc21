###############

###############
class DataMap:
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

####
def build_map(fp, cast_type = None):
    map_lines = fp.readlines()
    map_lines = list(line.strip() for line in map_lines)
    map_elements = []
    width = 0
    height = len(map_lines)
    for map_line in map_lines:
        if cast_type:
            map_line = list(cast_type(element) for element in map_line)
        if width == 0:
            width = len(map_line)
        map_elements += list(map_line)
    return DataMap(map_elements, width, height)


def build_init_map(width, height, init_value):
    return DataMap([init_value] * width * height, width, height)

def build_map_with_initializer(width, height, initializer):
    data_map = DataMap([None] * width * height, width, height)
    for y in range(data_map.height):
        for x in range(data_map.width):
            data_map.set_element(x, y, initializer(x, y))
    return data_map
    

####
def get_map_string(map, element_separator = '', format_width = None):
    s = ''
    for y in range(map.height):
        for x in range(map.width):
            element = map.get_element(x,y)
            if format_width:
                s +=  f'{element:{format_width}}'    
            else:
                s +=  str(map.get_element(x,y))
            s += element_separator
        s += '\n'
    return s

def mutate_map_with_operation(map, operation):
    for y in range(map.height):
        for x in range(map.width):
            element = map.get_element(x,y)
            operated_val = operation(x, y, element)
            map.set_element(x, y, operated_val)

####
# no diagonals considered
def get_neighbor_pos(data_map, x, y, diagonal=True):
    neighbor_pos = [(x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)]    
    if diagonal:
        neighbor_pos += [(x - 1, y - 1), (x - 1, y + 1), (x + 1, y - 1), (x + 1, y + 1)]
    neighbor_pos = list(pos for pos in neighbor_pos if data_map.is_valid_pos(pos[0], pos[1])) # remove None elements
    return neighbor_pos


####################################################################
####################################################################

