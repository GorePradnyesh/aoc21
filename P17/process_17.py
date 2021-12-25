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
y = (m + 1/2)x - 1/2x^2
where m is the initial velocity, and each step y increases by one in the neagative. 

equation 
index:              0   1       2       3       4       5       ...
position:           0   m       2m-1    3m-3    4m-6    5m-10   ...
initial velocity:   m   m-1     m-2     m-3     m-4     m-5     ...

// get eq coefficients
y = a + bx + c(x^2)
for x =0
    0 = a ~~~
for x = 1
    m = b + c
for x = 2
    2m - 1 = 2b + 4c
    2m - 1 = m + 2c
    c = -1/2 ~~~~
    b = m + 1/2 ~~~~
using values of a,b,c
∴ y = (m + 1/2)x - 1/2(x ^ 2)
'''
def get_y_pos(m, x):
    return (m*x) + int(0.5 * x) - int(0.5 * x * x)

'''
similar equation for velocity at index
y = m -x 
'''
def get_velocity_at_index(m, index):
    return (m-index)


class TA:
    def __init__(self, x_l, x_r, y_t, y_b) -> None:
        self.x_l = x_l
        self.x_r = x_r
        self.y_t = y_t
        self.y_b = y_b
    
    def __repr__(self) -> str:
        return f'({self.x_l}, {self.x_r}),({self.y_t}, {self.y_b})'

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
        y_pos = get_y_pos(initial_vel, index)
        if positon_overshoots_TA(ta, 0, y_pos):
            if print_pos:
                print(f'initial velocity: {initial_vel}, position overshot {y_pos} index: {index}')
            break
        elif TA_contains_position(ta, 0, y_pos):
            if print_pos:
                print(f'initial velocity: {initial_vel}, position found {y_pos} index: {index}')
            ta_contains_pos = True
            break
        if print_pos:
            print(f'initial velocity: {initial_vel}, position skipped {y_pos} index: {index}')
        index += 1
    return ta_contains_pos
    
#############################
def process(ta):

    # check_viability(ta, 133, True)

    '''
    upper bound velocity -> m
    m <= (y distance between start_y and y_b) - 1    

    note the trajectory when thrown with +ve velocity.
    The probe always reaches y = 0 at index 2m ( aka step # 2m )

    For the TA with a y below SA ( i.e. y in the negative )
    the probe will reach TA after step 2m.
    the velocity cannot be more than the distance of the bottom of TA from y =0.
    If it is more then in step 2m+1 the probe will overstep the TA entirely. 
    therefore 

    for initial velocity m 
    the velocity at index 2m = m + 1
    therefore 
        m + 1 should be less than distance between y = 0 and y_b of TA 
        m + 1 <= abs(y_b)
        m <= yb - 1

    this is the derivaton of the upper-bound of viable velocities
    '''    
    vel_upperbound = abs(ta.y_b)
    
    viable_velocities = []
    for velocity in range(vel_upperbound + 5): # 
        ta_contains_position = check_viability(ta, velocity)
        if ta_contains_position:
            viable_velocities.append(velocity)
        # print(f'For initial velocity:{velocity}, TA contains step :{ta_contains_position}')
        
    max_viable_velocity = max(viable_velocities)
    # the highest point for initial velocity 'v_i' is at index 'v_i' 
    print(f'Highest position for max viable velocity:{max_viable_velocity} pos:{get_y_pos(max_viable_velocity, max_viable_velocity)}')



ta0 = TA(20,30,-5,-10) # sample
ta1 = TA(175,227,-79,-134)  # iput
process(ta0)

