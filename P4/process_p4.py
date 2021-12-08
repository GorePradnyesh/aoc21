########################################################

dim = 5

class Position:
    def __init__(self, x, y) -> None:
        self.x = x
        self.y = y

    def __repr__(self) -> str:
        return f'({self.x}, {self.y})'

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
            # print(f'Found {input_draw} in board {self.board_number} at {position}')
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

def find_winner_board(boards, draw_numbers):
    winning_number = None
    winner_found = False
    winning_board = None
    for draw_number in draw_numbers:
        # print(f'Searching number {draw_number}')
        for board in boards:
            board.check_and_mark_draw(draw_number)
            if board.is_winner:
                winner_found = True
                winning_number = draw_number
                winning_board = board
                print(f'Found winner board = {board.board_number}')
                break
        if winner_found:
            break
    
    return winning_board, winning_number

def get_winner_sum(winner_board):
    pass

def process_bingo(fp_handle):
    draw_string = fp_handle.readline()
    draw_numbers = get_draw_numbers(draw_string)       
    boards = get_boards(fp_handle)
    print(f'Found {len(boards)} boards')
    ########    
    # boards[0].check_and_mark_draw('16')
    ########

    winner_board, winning_number = find_winner_board(boards, draw_numbers)
    win_sum = winner_board.get_win_sum(winning_number)
    print('Win Sum : ', win_sum)
    # winner_sum = get_winner_sum(winner_board)

def process():
    filePath = '/Users/pgore/dev/AOC21/P4/input/input1.txt'
    with open(filePath) as fp:
        process_bingo(fp)

process()