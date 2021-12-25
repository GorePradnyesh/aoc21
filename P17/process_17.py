import sys
import os
import pathlib
import math 

# Add Common to sys path
sys.path.append(os.path.join(pathlib.Path(__file__).parent.resolve(),'..', 'Common'))

import DataUtils

#############################
'''
Based on the formula 
" Due to gravity, the probe's y velocity decreases by 1. " 
the ploynomial is 
y = (m - 1/2)x + 1/2x^2
where m is the initial velocity
'''

def get_y_pos(m, x):
    return (m*x) - int(0.5 * x) + int(0.5 * x * x)

class TA:
    def __init__(self, x_l, x_r, y_t, y_b) -> None:
        self.x_l = x_l
        self.x_r = x_r
        self.y_t = y_t
        self.y_b = y_b

def TA_contains_position(ta, x, y):
    # TEMP: check only y
    
    # for a point P to be contained beteen two points A,B on the same axis    
    # the abs distance between the point P and either edge should be less or equal
    # to the distance between A,B

    # TODO: Hack. below is shortcut with the presumption that TA is below the 0 line
    # use distance formula to correctly determine containment
    return (y <= ta.y_t and y >= ta.y_b)


def positon_overshoots_TA(ta, x, y):
    # TEMP check only y
    return (y < ta.y_b)

def check_viability(ta, initial_vel, print_pos = False):
    index = 0
    ta_contains_pos = False
    while True:
        y_pos = get_y_pos(initial_vel, index) * (-1) # multiplying by -1 to adjust for problem. TODO: switch eq so that formula is not needed ?
        if positon_overshoots_TA(ta, 0, y_pos):
            if print_pos:
                print(f'initial velocity: {initial_vel}, position overshot {y_pos}')
            break
        elif TA_contains_position(ta, 0, y_pos):
            if print_pos:
                print(f'initial velocity: {initial_vel}, position found {y_pos}')
            ta_contains_pos = True
            break
        if print_pos:
            print(f'initial velocity: {initial_vel}, position skipped {y_pos}')
        index += 1
    return ta_contains_pos
    
#############################
def process(ta):
    # a = 3
    # step_ct = 11
    # for i in range(step_ct):
    #     print(f'At step {i} y pos: {y_pos(a, i)}')

    # upper bound velocity -> m
    # m <= (y distance between start and y_b) - 1
    # reasoning - TBD

    check_viability(ta, -133, True)

    vel_upperbound = abs(ta.y_b)
    for velocity in range(vel_upperbound + 40): # +5 for additional buffer
        ta_contains_position = check_viability(ta, velocity * -1) # TODO: -1 to flip velocity. Fix equation
        print(f'For initial velocity:{velocity}, TA contains step :{ta_contains_position}')



ta0 = TA(20,30,-5,-10)
ta1 = TA(175,227,-79,-134)
process(ta1)

