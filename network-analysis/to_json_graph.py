import networkx as nx
from networkx.readwrite import json_graph
import pandas as pd
import json


import argparse
parser = argparse.ArgumentParser()
parser.add_argument('--dataset', '-d',
                    default='civil-war')
args = parser.parse_args()

G = nx.Graph()

# Read csv for nodes and edges using pandas:
nodes = pd.read_csv("data/{}/nodes.csv".format(args.dataset),
                    sep='\t')
edges = pd.read_csv("data/{}/edges.csv".format(args.dataset),
                    sep='\t')

nodes.fillna('', inplace=True)
edges.fillna('', inplace=True)

# Import id, name, and group into node of Networkx:
for i, n in nodes.iterrows():
    G.add_node(n['id'], n.to_dict())

# Import source, target, and value into edges of Networkx:
for i, e in edges.iterrows():
    G.add_edge(e['Source'], e['Target'], e.to_dict())

j = json_graph.node_link_data(G)
js = json.dumps(j, indent=2)
with open("data/{}/graph.json".format(args.dataset), "w") as f:
    f.write(js)
    
# if __name__ == "__main__":
#     main()
