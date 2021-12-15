

open_tokens = ['(', '[', '{', '<']
close_tokens = [')', ']', '}', '>']
co_token = {')': '(', ']': '[', '}': '{', '>' : '<'}
co_token = {'(': ')', '[': ']', '{': '}', '<' : '>'}

token_cost = {')': 3, ']': 57, '}' : 1197, '>': 25137}

SYNTAX_ERROR_TYPE_CORRUPT = 'CORRUPT'
SYNTAX_ERROR_TYPE_INCOMPLETE = 'INCOMPLETE'


class SyntaxError:
    def __init__(self, type, input_line, expected, actual) -> None:
        self.input_line = input_line
        self.expected = expected
        self.actual = actual
        self.type = type

def process_line(input_line, syntax_error):
    tokens = list(input_line)
    token_stack = []
    for token in tokens:        
        if token in close_tokens:
            if token_stack[-1] == co_token[token]:
                token_stack.pop()
            else:
                syntax_error.append(SyntaxError(SYNTAX_ERROR_TYPE_CORRUPT, input_line, co_token[token], token))
                return
        elif token in open_tokens:
            token_stack.append(token)
    if len(token_stack) != 0:
        syntax_error.append(SyntaxError(SYNTAX_ERROR_TYPE_INCOMPLETE, input_line, None, None))
    # else: success ! 

def get_corrupt_lines(input_lines):
    pass

def process():
    syntax_errors = []
    filePath = '/Users/pgore/dev/AOC21/P10/input/input1.txt'
    with open(filePath) as fp:
        input_lines = fp.readlines()
        input_lines = list(line.strip() for line in input_lines)
        
        for line in input_lines:
            process_line(line, syntax_errors)
        corrupt_errors = list(filter(lambda error: error.type == SYNTAX_ERROR_TYPE_CORRUPT, syntax_errors))
        token_sum = sum(token_cost[error.actual] for error in corrupt_errors)
        print(f'Token Sum : {token_sum}')


process()