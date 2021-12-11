import re

######
class Position:
    def __init__(self, x, y) -> None:
        self.x = x 
        self.y = y

    def __repr__(self) -> str:
        return f'({self.x}, {self.y})'

    def displace_x(self, d):
        self.x += d
    
    def displace_y(self, d):
        self.y += d
    

class SparseImaage():
    def __init__(self) -> None:
        self.image_pos = {}

    def note_xy(self, x, y):
        if (x, y) not in self.image_pos.keys():
            # print(f'Not found {(x,y)} in keys')
            self.image_pos[(x, y)] = 0
        self.image_pos[(x, y)] += 1
    
    def __repr__(self) -> str:
        s = ''
        for y in range(10):
            for x in range(10):
                if (x,y) in self.image_pos.keys():
                    s += str(self.image_pos[(x,y)])
                else:
                    s += '.'
                s += ' '
            s += '\n'
        return s
                    
    
######
class Line:
    def __init__(self, positions) -> None:
        self.pos1 = Position(positions[0], positions[1])
        self.pos2 = Position(positions[2], positions[3])
    
    # 
    def __init__(self, pos1, pos2) -> None:
        self.pos1 = pos1
        self.pos2 = pos2
    
    #
    def is_diagonal(self):
        return self.pos1.x != self.pos2.x and self.pos1.y != self.pos2.y
    
    #
    def __repr__(self) -> str:
        return f'({self.pos1.x}, {self.pos1.y}) -> ({self.pos2.x}, {self.pos2.y})'

def walk_line(line, sparse_image):
    # Walk from pos 1 to pos 2
    # increment both (x, y) for pos 1 until (x, y) for pos2 is reached
    x_displace = 1 if line.pos2.x > line.pos1.x else -1
    y_displace = 1 if line.pos2.y > line.pos1.y else -1
    
    current_pos = line.pos1
    while current_pos.x != line.pos2.x or current_pos.y != line.pos2.y:
        # note position in sparse_image
        sparse_image.note_xy(current_pos.x, current_pos.y)
        if current_pos.x != line.pos2.x:
            current_pos.displace_x(x_displace)
        if current_pos.y != line.pos2.y:
            current_pos.displace_y(y_displace)
    
    # note pos2. while does not cover pos2
    sparse_image.note_xy(current_pos.x, current_pos.y)
    return sparse_image

    

re_str = '(\d+),(\d+) -> (\d+),(\d+)'
re_matcher = re.compile(re_str)

######
def update_min_max_coords(position, min_x, min_y, max_x, max_y):
    if position.x < min_x:
        min_x = position.x
    if position.x > max_x:
        max_x = position.x
    
    if position.y < min_y:
        min_y = position.y
    if position.y > max_y:
        max_y = position.y
    
    return min_x,min_y,max_x,max_y

######
def process_coords(fp_handle):
    lines = []
    
    min_x = 0
    min_y = 0
    max_x = 0
    max_y = 0

    dim_x = 0
    dim_y = 0

    for line in fp_handle.readlines():
        matched = re_matcher.match(line)
        if matched:
            positions = matched.groups()
            pos_1 = Position(int(positions[0]), int(positions[1]))
            pos_2 = Position(int(positions[2]), int(positions[3]))

            min_x,min_y,max_x,max_y = update_min_max_coords(
                                        pos_1, min_x, min_y, max_x, max_y)

            lines.append(Line(pos_1, pos_2))
    
    dim_x = max_x - min_x
    dim_y = max_y - min_y

    return lines, dim_x, dim_y  

######
def process_sparse_image(fp_handle):
    lines, dim_x, dim_y = process_coords(fp_handle)
    print(f'Size : {dim_x}, {dim_y}')

    sparse_image = SparseImaage()
    target_lines = [line for line in lines if not line.is_diagonal()]
    for line in target_lines:
        walk_line(line, sparse_image)

    # print(sparse_image)
    print("Overlap: ",sum(value > 1 for value in sparse_image.image_pos.values()))


##################################################


def process():
    filePath = '/Users/pgore/dev/AOC21/P5/input/input1.txt'
    with open(filePath) as fp:
        process_sparse_image(fp)

process()