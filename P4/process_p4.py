########################################################

dim = 5

strategy_win = 1
strategy_lose = 0

class Position:
    def __init__(self, x, y) -> None:
        self.x = x
        self.y = y

    def __repr__(self) -> str:
        return f'({self.x}, {self.y})'
class WinnerBoard:
    def __init__(self, board, winning_value) -> None:
        self.board = board
        self.winning_value = winning_value
class BingoBoard:
    def __init__(self, inputs_1d, board_number) -> None:        
        if len(inputs_1d) != (dim * dim):
            print('incorrect input')
            exit
        self.inputs_1d = inputs_1d
        self.mark_list = [0] * dim * dim # used to get unmarked numbers
        self.row_tracker = [0] * dim
        self.col_tracker = [0] * dim
        self.is_winner = False
        self.board_number = board_number
    
    def check_draw(self, input_draw):
        if input_draw not in self.inputs_1d:
            return None
        index = 0
        try:
            index = self.inputs_1d.index(input_draw)
            # Row major, / does x, % does y
            return Position(int(index / dim), index % dim) 
        except ValueError:
            return None
    
    def check_and_mark_draw(self, input_draw):
        position = self.check_draw(input_draw)
        if position:
            # if found increment row and column
            self.row_tracker[position.x] += 1
            self.col_tracker[position.y] += 1
            self.mark_list[position.x * dim + position.y] = 1

        # Check board for win, and mark if it is.
        if dim in self.row_tracker or dim in self.col_tracker:
            self.is_winner = True

    def get_win_sum(self, winning_value):
        if not self.is_winner:
            print('Board is not a winner. 0 Winning sum')
            return 0
        sum = 0
        for index in range(dim * dim):
            if not self.mark_list[index]:
                sum += int(self.inputs_1d[index])
        return sum * int(winning_value)

##############################

def get_draw_numbers(draw_string):
    return draw_string.split(',')

def get_boards(fp_handle):
    boards = []
    board_lines= []    
    board_number = 0
    while 1:   
        input_str = fp_handle.readline()     
        if not input_str:
            # EOF 
            break
        
        if not input_str.strip():
            # new line
            if len(board_lines) > 0:
                print('unexpected new line while processing board')
                exit
            continue            
        else:                        
            # collect board
            line_cells = input_str.strip().split(' ')
            line_cells =[cell for cell in line_cells if cell]
            board_lines += line_cells
            
            if len(board_lines) ==  dim * dim:
                boards.append(BingoBoard(board_lines, board_number))
                board_number += 1 
                board_lines= []

    return boards


def find_winner_boards(boards, draw_numbers):
    winning_boards = []
    winning_indices = []
    for draw_number in draw_numbers:
        # print(f'Searching number {draw_number}')
        for board in boards:
            if board.board_number in winning_indices:
                continue # dont search winning boards anymore
            board.check_and_mark_draw(draw_number)
            if board.is_winner:
                # print(f'Found winner board = {board.board_number}')
                winning_boards.append(WinnerBoard(board, draw_number))
                winning_indices.append(board.board_number)
    return winning_boards

def process_bingo(fp_handle, strategy):
    draw_string = fp_handle.readline()
    draw_numbers = get_draw_numbers(draw_string)       
    boards = get_boards(fp_handle)
    print(f'Found {len(boards)} boards')
    
    winning_boards = find_winner_boards(boards, draw_numbers)
    board = None
    winning_number = None
    if strategy == strategy_win:
        # choose first board
        board = winning_boards[0].board
        winning_number = winning_boards[0].winning_value
    else:
        board = winning_boards[-1].board
        winning_number = winning_boards[-1].winning_value
    win_sum = board.get_win_sum(winning_number)  
    print('Win Sum : ', win_sum)

def process():
    filePath = '/Users/pgore/dev/AOC21/P4/input/input1.txt'
    with open(filePath) as fp:
        process_bingo(fp, strategy_lose)

process()