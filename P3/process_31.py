########################################################
from typing import final


def process_pc(fpHandle):
    bin_input_str = ' '
    histogram = None
    input_count = 0
    field_size = 0

    while bin_input_str:
        bin_input_str = fpHandle.readline()
        
        # note field size and adjust histogram
        if not field_size:
            field_size = len(bin_input_str.strip())
            histogram = [0] * field_size
        if not bin_input_str:
            break
        
        #count the number of inputs 
        input_count +=1 

        # MSB is the first bit in this example
        input_value = int(bin_input_str, base=2)        
        
        # count the number of 1s in the input and increment the histogram
        # walk the range 0 to (field-size-1) and bit test it. 
        # because we walk MSB to LSB do field-size - (index+1) left shift
        for index in range(field_size):
            histogram[index] += 1 if input_value & 1<<(field_size - (index + 1)) else 0
    
    print(histogram)

    # Gamma is the most occurring bits in the postion
    gamma_value = 0
    for index in range(field_size):
        if histogram[index] > int(input_count/2):
            gamma_value |= 1 << (field_size - (index + 1))
    
    print('gamma value:',gamma_value)

    # epsilon is the least occurring
    # but because binary just an complement of the number
    epsilon_value = gamma_value ^ ((1 << field_size) -1)
    print('epsilom value:', epsilon_value)
    print('power consumption:', gamma_value * epsilon_value)


        

########################################################

def process():
    filePath = '/Users/pgore/dev/AOC21/P3/input/input1.txt'
    with open(filePath) as fp:
        process_pc(fp)

process()