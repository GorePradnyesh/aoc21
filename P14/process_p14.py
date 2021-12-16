import sys
import os
import pathlib
import re

# Add Common to sys path
sys.path.append(os.path.join(pathlib.Path(__file__).parent.resolve(),'..', 'Common'))

import DataUtils

###############

insertion_regex = re.compile('(\w\w) -> (\w)')

def construct_rules(rule_lines):
    rules = {}
    for line in rule_lines:
        g = insertion_regex.search(line)
        if g:
            rules[tuple(g.group(1))] = g.group(2)
    return rules

def process_template(template, rules, step_count = 1):
    for index in range(step_count):
        new_template = []
        for pos_i in range(len(template) - 1): #one less 
            pos_0 = template[pos_i]            
            pos_1 = template[pos_i + 1]
            
            new_template.append(pos_0)
            
            if (pos_0, pos_1) in rules:
                new_template.append(rules[(pos_0, pos_1)])
        new_template.append(template[-1])   # append last since we walk one short in the loop
        template = new_template
        # print(f'After step {index + 1}: {"".join(template)}')
    return template
        
def process():
    input_path = os.path.join(pathlib.Path(__file__).parent.resolve(), 'input')    
    filePath = os.path.join(input_path, 'input1.txt')
    lexicon = set()
    reobj = re.compile('\d+,\d+')

    with open(filePath) as fp:
        template = list(fp.readline().strip())
        
        # add template chars to lexicon
        lexicon = lexicon.union(set(template))

        rule_lines = fp.readlines()
        rule_lines = list(rule.strip() for rule in rule_lines if rule.strip())
        
        rules = construct_rules(rule_lines)

        # add rule values to lexicon
        lexicon = lexicon.union(set(rules.values()))

        step_count = 10
        new_template = process_template(template, rules, step_count)
        
        print(f'After {step_count} length = {len(new_template)}')
        token_frequency = {}
        for token in lexicon:
            token_frequency[token] = new_template.count(token)
            print(f'Frequency of {token}: {new_template.count(token)}')
                
process()