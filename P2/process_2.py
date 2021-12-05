
depth_key = 'depth'
horizontal_key = 'horizontal'
aim_key = 'aim'

def processInput(filePath):
    with open(filePath) as fp:
        process_motion(fp)

def readNextDisplacement(displacement_str):
    displacement = {depth_key:None, horizontal_key:None}
    if displacement_str:        
        displacement_value = int(displacement_str.split()[-1])
        if displacement_str.startswith('forward'):
            displacement[horizontal_key] = displacement_value
        elif displacement_str.startswith('down'):
            displacement[depth_key] = displacement_value
        elif displacement_str.startswith('up'):
            displacement[depth_key] = displacement_value * -1
        return displacement

def displaceWithAim(current_position, displacement):
    if displacement[horizontal_key]:
        current_position[horizontal_key] += displacement[horizontal_key]
        current_position[depth_key] += displacement[horizontal_key] * current_position[aim_key]
    elif displacement[depth_key]:
        current_position[aim_key] += displacement[depth_key]
    return current_position

def simpleDisplace(current_position, displacement):
    if displacement[horizontal_key]:
        current_position[horizontal_key] += displacement[horizontal_key]
    elif displacement[depth_key]:
        current_position[depth_key] += displacement[depth_key]
    return current_position

##################################################################

def process_motion(fpHandle):
    position = {depth_key: 0, horizontal_key:0, aim_key:0}
    displacement_str = ' '
    while displacement_str:
        displacement_str = fpHandle.readline()
        if displacement_str:
            displacement = readNextDisplacement(displacement_str)
            # position = simpleDisplace(position, displacement)
            position = displaceWithAim(position, displacement)            
    print(position)
    print('total displacement: ', position[depth_key] * position[horizontal_key])

def process():
    filePath = '/Users/pgore/dev/AOC21/P2/input/input1.txt'
    processInput(filePath)

process()