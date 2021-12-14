
##################################################
from os import read
import statistics

unqiue_sig_len = [2, 4, 3, 7]

##################################
# Index of set
#  0000   
# 1    2
# 1    2  
#  3333   
# 4    5
# 4    5  
#  6666   
#
# 0 - [0,1,2,4,5,6]         length: 6
# 1 - [2,5] *               length: 2
# 2 - [0,2,3,4,6]           length: 5
# 3 - [0,2,3,5,6]           length: 5
# 4 - [1,2,3,5] *           length: 4
# 5 - [0,1,3,5,6]           length: 5
# 6 - [0,1,3,4,5,6]         length: 6
# 7 - [0,2,5] *             length: 3
# 8 - [0,1,2,3,4,5,6] *     length: 7
# 9 - [0,1,2,3,5,6]         length: 6


# Rules of discovery // can be more optimized
# -----------------
# 1. (1)
# 2. (4)
# 3. (7)
# 4. (8)
# 5. (7) - (1) => 0
# 6. if len(() len 6 - ((7) + (4))) is 1 => (9), 6
# 7. (8) - (9) => 4
# 8. () of len 5 containing 4 => (2)
# 9. () of len 5 superset of (7) => (3)
# 10. (3) - (2) => 5
# 11. (8) - ((2) + 5) => 1
# 12. (7) - (0 + 5) => 2
# 13. (4) - (1 + 2 + 5) => 3
################################## 

def extract_tdn_fdn(fp):
    input_signals = fp.readlines()
    tdn_numbers = []
    fdn_numbers = []
    for signal in input_signals:        
        tdn_str = signal.split('|')[0].split()
        tdn_numbers.append(list(value for value in tdn_str if value)) #remove empty string

        fdn_str = signal.split('|')[1].split()
        fdn_numbers.append(list(value for value in fdn_str if value))
        
    return tdn_numbers, fdn_numbers

def is_unique_count(digit_str):
    return 1 if len(digit_str) in unqiue_sig_len else 0

#################################################################### 
####################################################################  

class DigitMapper():
    def __init__(self, zero_m, one_m, two_m, three_m, four_m, five_m, six_m) -> None:
        self.zero_m = zero_m 
        self.one_m = one_m
        self.two_m = two_m
        self.three_m = three_m
        self.four_m = four_m
        self.five_m = five_m
        self.six_m = six_m    
        
        self.mapped_set = {}
        
        self.mapped_set[frozenset([self.zero_m, self.one_m, self.two_m, self.four_m, self.five_m, self.six_m])] = 0
        self.mapped_set[frozenset([self.two_m, self.five_m])] = 1
        self.mapped_set[frozenset([self.zero_m, self.two_m, self.three_m, self.four_m, self.six_m ])] = 2
        self.mapped_set[frozenset([self.zero_m, self.two_m, self.three_m, self.five_m, self.six_m ])] = 3
        self.mapped_set[frozenset([self.one_m, self.two_m, self.three_m, self.five_m ])] = 4
        self.mapped_set[frozenset([self.zero_m, self.one_m, self.three_m, self.five_m, self.six_m ])] = 5
        self.mapped_set[frozenset([self.zero_m, self.one_m, self.three_m, self.four_m, self.five_m, self.six_m ])] = 6
        self.mapped_set[frozenset([self.zero_m, self.two_m, self.five_m])] = 7
        self.mapped_set[frozenset([self.zero_m, self.one_m, self.two_m, self.three_m, self.four_m, self.five_m, self.six_m ])] = 8
        self.mapped_set[frozenset([self.zero_m, self.one_m, self.two_m, self.three_m, self.five_m, self.six_m ])] = 9


def discover_digits(signal_strings):
    if len(signal_strings) != 14:
        print('Invalid input to discovery. Exiting')
        exit -1

    zero_m = 0; one_m = 0; two_m = 0; three_m = 0; four_m = 0; five_m = 0; six_m = 0
    one_s = []; two_s = []; three_s = []; four_s = []; five_s = []; six_s = []; seven_s = []; eight_s = []; nine_s = []

    signal_strings = list(list(value) for value in signal_strings)
    # create frozenset of each char sequence e.g. fronzenset({'a','b'})
    # cast it to set to remove duplicates. Cast it back to list
    signal_sets = list(set(frozenset(value) for value in signal_strings)) 
    
    # follow discovery rules
    # Step 1-4
    one_s = list(filter(lambda set_x: len(set_x) == 2, signal_sets))[0]
    four_s = list(filter(lambda set_x: len(set_x) == 4, signal_sets))[0]
    seven_s = list(filter(lambda set_x: len(set_x) == 3, signal_sets))[0]
    eight_s = list(filter(lambda set_x: len(set_x) == 7, signal_sets))[0]
    
    # 5. 
    zero_m = list(seven_s.difference(one_s))[0]

    # 6.
    seven_four = seven_s.union(four_s)
    nine_s = list(filter(lambda set_x: len(set_x) == 6 and len(set_x.difference(seven_four)) == 1, signal_sets))[0]
    six_m = list(nine_s.difference(seven_four))[0]

    #7 
    four_m = list(eight_s.difference(nine_s))[0]

    #8
    two_s = list(filter(lambda set_x: len(set_x) == 5 and (four_m in set_x), signal_sets))[0]

    #9.
    three_s = list(filter(lambda set_x: len(set_x) == 5 and seven_s.issubset(set_x), signal_sets))[0]

    #10.
    five_m = list(three_s.difference(two_s))[0]

    #11.
    two_five_s = two_s.union(set([five_m]))
    one_m = list(eight_s.difference(two_five_s))[0]

    #12.
    two_m = list(seven_s.difference(set([zero_m, five_m])))[0]

    #13. 
    three_m = list(four_s.difference(set([one_m, two_m, five_m])))[0]
    
    return zero_m, one_m, two_m, three_m, four_m, five_m, six_m



    


#################################################################### 
####################################################################  

def get_unique_sig_count(tdn, fdn):
    if len(tdn) != len(fdn):
        print('Mismatched signal. Exiting')
        exit -1

    unique_count = 0
    for index in range(len(tdn)):
        # unique_count += sum(is_unique_count(value) for value in tdn[index])
        unique_count += sum(is_unique_count(value) for value in fdn[index])
    print(f'Unique digits :', unique_count)
    

def get_fdn_number(fdn_line, digit_mapper):
    if len(fdn_line) != 4:
        print(f'Invalid input {fdn_line}. Exiting')
        exit -1
    fdn_strings = list(list(value) for value in fdn_line)
    signal_sets = list(frozenset(value) for value in fdn_strings) 
    number = 0
    for index in range(4):
        signal_set = signal_sets[index]
        number += pow(10, 3 - index) * digit_mapper.mapped_set[signal_set]
    return number

def process():
    filePath = '/Users/pgore/dev/AOC21/P8/input/input1.txt'
    with open(filePath) as fp:
        tdn, fdn = extract_tdn_fdn(fp)
        get_unique_sig_count(tdn, fdn)
        
        sum_fdn = 0
        
        for index in range(len(tdn)):
            zero_m, one_m, two_m, three_m, four_m, five_m, six_m = discover_digits(tdn[index] + fdn[index])
            digit_map = DigitMapper(zero_m, one_m, two_m, three_m, four_m, five_m, six_m)
            # for k,v in digit_map.mapped_set.items():
            #     sk = list(k).sort()
            #     sk.sort()
            #     print(f'{v} : {"".join(sk)}')
            
            fdn_number = get_fdn_number(fdn[index], digit_map)
            # print(fdn_number)
            sum_fdn += fdn_number
    print(sum_fdn)

process()