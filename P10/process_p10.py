import statistics

open_tokens = ['(', '[', '{', '<']
close_tokens = [')', ']', '}', '>']
co_token = {')': '(', ']': '[', '}': '{', '>' : '<'}
oc_token = {'(': ')', '[': ']', '{': '}', '<' : '>'}

token_cost = {')': 3, ']': 57, '}' : 1197, '>': 25137}
incomplete_token_cost = {')': 1, ']': 2, '}' : 3, '>': 4}

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

def process_incomplete_error(error):
    input_tokens = list(error.input_line)
    token_stack = []
    missing_tokens = []
    for token in input_tokens[::-1]: #reversed
        if token in close_tokens:
            token_stack.append(token)
        
        elif token in open_tokens:
            if len(token_stack) and token == co_token[token_stack[-1]]:
                # open token found matching close token at top of stack
                token_stack.pop()
            elif len(token_stack) == 0:
                missing_tokens.append(oc_token[token])
            else:
                print('Unexpected condition. Exiting')
                exit -1
    return missing_tokens


def process():
    syntax_errors = []
    filePath = '/Users/pgore/dev/AOC21/P10/input/input1.txt'

    # ## Temp
    # se = []
    # process_line('((<>{[]}', se)
    # mt = process_incomplete_error(se[0])
    # print(''.join(mt))


    with open(filePath) as fp:
        input_lines = fp.readlines()
        input_lines = list(line.strip() for line in input_lines)
        
        # process Corrupt strings
        for line in input_lines:
            process_line(line, syntax_errors)
        corrupt_errors = list(filter(lambda error: error.type == SYNTAX_ERROR_TYPE_CORRUPT, syntax_errors))
        token_sum = sum(token_cost[error.actual] for error in corrupt_errors)
        print(f'Token Sum : {token_sum}')


        #process incomplete strings
        incomplete_errors = list(filter(lambda error: error.type == SYNTAX_ERROR_TYPE_INCOMPLETE, syntax_errors))
        
        incomplete_scores = []
        for error in incomplete_errors:
            missing_tokens = process_incomplete_error(error)
            incomplete_score = 0
            for token in missing_tokens:
                incomplete_score = incomplete_score * 5 + incomplete_token_cost[token]
            incomplete_scores.append(incomplete_score)
            print(f'incomplete score: {"".join(missing_tokens)} : {incomplete_score}')
        
        print('Middle Score: ',statistics.median(incomplete_scores))
        
process()