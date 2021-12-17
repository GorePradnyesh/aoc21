import sys
import os
import pathlib
import re

# Add Common to sys path
sys.path.append(os.path.join(pathlib.Path(__file__).parent.resolve(),'..', 'Common'))

import DataUtils

#####################


#####################
def process():
    input_path = os.path.join(pathlib.Path(__file__).parent.resolve(), 'input')    
    filePath = os.path.join(input_path, 'input0.txt')
    lexicon = set()
    reobj = re.compile('\d+,\d+')

    with open(filePath) as fp:
        data_map = DataUtils.build_map(fp, cast_type=int)
        print(DataUtils.get_map_string(data_map))
process()