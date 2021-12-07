########################################################
from typing import final

operation_o2_gen_rating = 1
operation_co2_scrubber = 0

def process_histogram(input_values):
    histogram = None
    field_size = 0
    for bin_input_str in input_values:           
        # note field size and adjust histogram
        if not field_size:
            field_size = len(bin_input_str.strip())
            histogram = [0] * field_size

        if not bin_input_str:
            break            

        # MSB is the first bit in this example
        input_value = int(bin_input_str, base=2)        
        
        # count the number of 1s in the input and increment the histogram
        # walk the range 0 to (field-size-1) and bit test it. 
        # because we walk MSB to LSB do field-size - (index+1) left shift
        for index in range(field_size):
            histogram[index] += 1 if input_value & 1<<(field_size - (index + 1)) else 0
    return histogram

def get_gamma_value(histogram, field_size, input_count):
    # Gamma is the most occurring bits in the postion
    gamma_value = 0
    for index in range(field_size):
        if histogram[index] >= int(input_count/2):
            gamma_value |= 1 << (field_size - (index + 1))

    return gamma_value

def get_power_consumption(histogram, field_size, input_count):
    gamma_value = get_gamma_value(histogram, field_size, input_count)
    print('Gamma value', bin(gamma_value))

    # epsilon is the least occurring
    # but because binary just an complement of the number
    epsilon_value = gamma_value ^ ((1 << field_size) -1)    
    return gamma_value * epsilon_value

def process_pc(fpHandle):
    bin_input_str = ' '
    input_values = fpHandle.readlines()
    histogram = process_histogram(input_values) 
    
    input_count = len(input_values)
    field_size = len(input_values[0].strip())

    print(histogram)
    power_consumption = get_power_consumption(histogram, field_size, input_count)
    print('Power Consumption', power_consumption)
    
########################################################

def process_values(bit_position, input_values, field_size):
    one_count = 0
    for bin_input_str in input_values:
        input_value = int(bin_input_str, base=2)        
        one_count += 1 if input_value & 1 << (field_size - (bit_position + 1)) else 0
    return one_count

def filter_values(bit_position, field_size, input_values, bias):
    filtered_values = []
    for bin_input_str in input_values:
        input_value = int(bin_input_str, base=2)
        if bias:    # 
            if input_value & 1 << field_size - (bit_position + 1):
                filtered_values.append(bin_input_str)
        else:
            if not input_value & 1 << field_size - (bit_position + 1):
                filtered_values.append(bin_input_str)
    return filtered_values
        
def get_bias(one_count, input_size, operation):
    bias = None
    if operation == operation_o2_gen_rating:
        bias = 1 if (one_count >= input_size / 2) else 0
    elif operation == operation_co2_scrubber:
        bias = 0 if (one_count >= input_size / 2) else 1
    return bias

def process_operation(input_values, field_size, operation):
    filtered_values = input_values
    # Walk each bit position
    for bit_position in range(field_size):
       one_count = process_values(bit_position, filtered_values, field_size)
       bias = get_bias(one_count, len(filtered_values), operation)
       # print(bias)
       filtered_values = filter_values(bit_position, field_size, filtered_values, bias)
       # print(filtered_values)
       if len(filtered_values) == 1:
           break
    return filtered_values[0]

# processes while holding all values memory. Can do better !
def process_ls(fpHandle):
    input_values = fpHandle.readlines()
    field_size = len(input_values[0].strip())
    
    o2_gen_rating_str = process_operation(input_values, field_size, operation_o2_gen_rating)
    o2_gen_rating = int(o2_gen_rating_str.strip(), base=2)
    # print(o2_gen_rating)
    co2_scrubber_rating_str = process_operation(input_values, field_size, operation_co2_scrubber)
    co2_scrubber_rating = int(co2_scrubber_rating_str.strip(), base=2)
    # print(co2_scrubber_rating)
    print('Life support:', o2_gen_rating * co2_scrubber_rating)

########################################################

def process():
    filePath = '/Users/pgore/dev/AOC21/P3/input/input1.txt'
    with open(filePath) as fp:
        # process_pc(fp)
        process_ls(fp)

process()