
depth_key = 'depth'
horizontal_key = 'horizontal'

def processInput(filePath):
    with open(filePath) as fp:
        process_motion(fp)

def readNextDisplacement(fpHandle):
    displacement_str = fpHandle.readline()
    if displacement_str:        
        displacement = {depth_key: 0, horizontal_key:0}
        displacement_value = int(displacement_str.split()[-1])
        if displacement_str.startswith('forward'):
            displacement[horizontal_key] += displacement_value
        elif displacement_str.startswith('down'):
            displacement[depth_key] += displacement_value
        elif displacement_str.startswith('up'):
            displacement[depth_key] -= displacement_value
        return displacement
    return None

def displace(current_dislacement, new_displacement):
    return {depth_key: (current_dislacement[depth_key] + new_displacement[depth_key]), 
        horizontal_key: (current_dislacement[horizontal_key] + new_displacement[horizontal_key])}

##################################################################

def process_motion(fpHandle):
    initial_displacement = {depth_key: 0, horizontal_key:0}
    new_displacement = {} 
    while new_displacement is not None:
        new_displacement = readNextDisplacement(fpHandle)
        if new_displacement:
            initial_displacement = displace(initial_displacement, new_displacement)
    print(initial_displacement)
    print('total displacement: ', initial_displacement[depth_key] * initial_displacement[horizontal_key])

def process():
    filePath = '/Users/pgore/dev/AOC21/P2/input/input1.txt'
    processInput(filePath)

process()