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

####
def get_map_string(map, element_separator = ''):
    s = ''
    for y in range(map.height):
        for x in range(map.width):
            s += str(map.get_element(x,y))
            s += element_separator
        s += '\n'
    return s