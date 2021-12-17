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

# inefficient
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

# inefficient
def process_template_simple(template, rules, lexicon):
    step_count = 10
    new_template = process_template(template, rules, step_count)
    
    print(f'After {step_count} length = {len(new_template)}')
    token_frequency = {}
    for token in lexicon:
        token_frequency[token] = new_template.count(token)
        print(f'Frequency of {token}: {new_template.count(token)}')

##########################################
def get_template_distro(template):
    template_distro = {}
    for pos_i in range(len(template) - 1): #one less 
        pos_0 = template[pos_i]            
        pos_1 = template[pos_i + 1]
        if (pos_0, pos_1) not in template_distro.keys():
            template_distro[(pos_0, pos_1)] = 0
        template_distro[(pos_0, pos_1)] += 1
    return template_distro

def process_template_efficient(template, rules, lexicon, step_count):
    distro = get_template_distro(template)

    for index in range(step_count):
        new_distro = {}
        for pair in distro.keys():
            if distro[pair] == 0:
                continue
            
            if pair not in rules:
                continue
            
            # get resultant
            res_char = rules[pair]
            pair_0 = (pair[0], res_char)
            pair_1 = (res_char, pair[1])

            # init new distro if it does not contain new pairs
            if pair_0 not in new_distro.keys():
                new_distro[pair_0] = 0             
            if pair_1 not in new_distro.keys():
                new_distro[pair_1] = 0    
            
            # increse the count of resultant pairs 
            # by the count of the originating pair
            new_distro[pair_0] += distro[pair] 
            new_distro[pair_1] += distro[pair]
        distro = new_distro
    return distro


#####################
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
        
        # process_template_simple(template, rules, lexicon)
        distro = process_template_efficient(template, rules, lexicon, 40)
        print(f'len: {sum(distro.values()) + 1}')

        # count the element in the pairs
        token_frequency = {}
        for pair in distro.keys():
            ct = distro[pair]
            
            p0 = pair[0]                        
            if p0 not in token_frequency.keys():
                token_frequency[p0] = 0
            token_frequency[p0] += ct
            
            p1 = pair[1]
            if p1 not in token_frequency.keys():
                token_frequency[p1] = 0
            token_frequency[p1] += ct

        # Each element will be counted twice, once for each pair with its neightbor.
        # Divide the frequency by 2 
        # but if element is at start, it will be counted once for that pair ! 
        # Special case that by adding 1 if it occurs at the start or end
        
        for lex in token_frequency.keys():
            token_frequency[lex] = int(token_frequency[lex] / 2)
            if lex == template[0] or lex == template[-1]:
                token_frequency[lex] += 1

        print(token_frequency)
        print(f'difference : {max(token_frequency.values()) - min(token_frequency.values())}')


                
process()