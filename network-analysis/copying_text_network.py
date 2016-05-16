import pandas as pd
import networkx as nx
from itertools import combinations

g = nx.Graph()

root = "/cs/home/hxiao/code/eebo/network-analysis"

cp_df = pd.read_pickle(root + "/output/copying_dataframe.pkl")
tx_df = pd.read_pickle(root + "/output/text_dataframe.pkl")


for i, r in cp_df.iterrows():
    for d1, d2 in combinations(r['doc_ids'], 2):
        d1d = tx_df.loc[d1].to_dict()
        d2d = tx_df.loc[d1].to_dict()
        d1d['Label'] = d1
        d2d['Label'] = d2
        g.add_node(d1, d1d)
        g.add_node(d2, d2d)
        g.add_edge(d1, d2)

nx.write_graphml(g,
                 'output/text_network_of_copying_relation.graphml')
