
def processInput(filePath):
    with open(filePath) as fp:
        # processSingleDepth(fp)
        process_3Slider(fp)
    
def readNextDepth(fpHandle):
    depth_str = fpHandle.readline()
    if depth_str:
        return int(depth_str)
    return None

##################################################################
def processSingleDepth(fpHandle):
    print('Processing Single Pass')
    prev_depth = None
    
    #read first value
    depth_value = fpHandle.readline()
    if depth_value:
        prev_depth = int(depth_value)
    else:
        exit
    
    depth_increase_counter = 0
    total_value_counter = 0
    
    # read subsequent values 
    while depth_value:
        depth_value = fpHandle.readline()            
        if depth_value:                
            total_value_counter += 1
            current_depth = int(depth_value)
            # print(str(current_depth))
            if current_depth > prev_depth:
                depth_increase_counter += 1
            prev_depth = current_depth

    print('Total: ', total_value_counter)
    print('Increased: ', depth_increase_counter)


##################################################################
def process_3Slider(fpHandle):
    print('Processing 3 Slider Window')
    depth_increase_counter = 0
    prev_sliding_sum = None
    
    depth_window = [None, None, None]    
    depth_window_size = 3
    
    sliding_index = 0
    depth_value = 0
    while depth_value is not None:
        depth_value = readNextDepth(fpHandle)
        
        if not depth_value:
            # if depth value is not return, continue ( and while stops )
            continue
        depth_window[sliding_index] = depth_value
        
        # slide the index over
        sliding_index = (sliding_index + 1) % depth_window_size
        # print(depth_window)

        if None in depth_window:
            # if window measurement is not compete, continue to measure more values
            continue
        else:
            current_sliding_sum = sum(depth_window)
            print('Depth sum ', current_sliding_sum)            
            # a previous window has been obtained
            if prev_sliding_sum is not None:           
                if current_sliding_sum > prev_sliding_sum:
                    depth_increase_counter += 1 
            prev_sliding_sum = current_sliding_sum
    print('Increased depth = ', depth_increase_counter)

##################################################################

def process():
    filePath = '/Users/pgore/dev/AOC21/P1/input/input1.txt'
    processInput(filePath)

process()