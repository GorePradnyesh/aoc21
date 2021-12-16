import sys
import os
import pathlib

# Add Common to sys path
sys.path.append(os.path.join(pathlib.Path(__file__).parent.resolve(),'..', 'Common'))

import DataUtils

###############
class Data_graph:
    def __init__(self) -> None:
        self.nodes = {}
    
    def add_connection(self, node1, node2):
        if node1 not in self.nodes.keys():
            self.nodes[node1] = [node2]
        elif node2 not in self.nodes[node1]:
            self.nodes[node1].append(node2)
        # else:
        #   duplicate entry, ignore

        if node2 not in self.nodes.keys():
            self.nodes[node2] = [node1]
        elif node1 not in self.nodes[node2]:
            self.nodes[node2].append(node1)
        # else:
        #   duplicate entry, ignore
    
    def get_connections(self, node):
        if node not in self.nodes.keys():
            return None
        return self.nodes[node]

    def getLength(self):
        return len(self.nodes)

def is_end(node):
    return node.lower() == 'end'

def is_start(node):
    return node.lower() == 'start'

def is_big(node):
    return node.isupper()

def is_small(node):
    return node.islower()


def traverse_nodes(data_graph, current_node, end_node, prior_path, final_paths, visited_nodes):    
    current_path = prior_path + [current_node]
    
    if is_end(current_node):
        final_paths.append(prior_path + [current_node])
        return
    
    # recruse through the connections, skipping visited nodes 
    connections = data_graph.get_connections(current_node)
    for conn in connections:

        if conn in prior_path and (is_small(conn) or is_start(conn)):
            continue
        else:
            # add small node to visited list to prevent walking them again            
            traverse_nodes(data_graph, conn, end_node, current_path.copy(), final_paths, visited_nodes)




def build_graph_connections(data_graph, connection_strs):
    for conn in connection_strs:
        nodes = conn.split('-')
        if len(nodes) != 2:
            print(f'Exiting. Invalud input {conn}')
        data_graph.add_connection(nodes[0], nodes[1])

def process():
    filePath = '/Users/pgore/dev/AOC21/P12/input/input1.txt' 
    with open(filePath) as fp:
        connections = fp.readlines()
        connections = list(conn.strip() for conn in connections)
        data_graph = Data_graph()

        build_graph_connections(data_graph, connections)
        
        start_node = 'start'
        end_node = 'end'
        final_paths = []
        visited_nodes = []
        path = []
        traverse_nodes(data_graph, start_node, end_node, path, final_paths, visited_nodes)

        # for g_path in final_paths:
        #     print(g_path)
        print('# of paths ',len(final_paths))
        

process()