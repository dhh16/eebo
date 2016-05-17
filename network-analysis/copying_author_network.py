import pandas as pd
import networkx as nx
from itertools import combinations

g = nx.Graph()

root = "/cs/home/hxiao/code/eebo/network-analysis"

cp_df = pd.read_pickle(root + "/output/copying_dataframe.pkl")
tx_df = pd.read_pickle(root + "/output/text_dataframe.pkl")


for i, r in cp_df.iterrows():
    for d1, d2 in combinations(r['doc_ids'], 2):
        author1 = tx_df.loc[d1, 'author_raw']
        author2 = tx_df.loc[d2, 'author_raw']

        if not author1 or not author2:
            continue

        g.add_node(author1, {'Label': author1})
        g.add_node(author2, {'Label': author2})
        if g.has_edge(author1, author2):
            g[author1][author2]['weight'] += r['length']
        else:
            g.add_edge(author1, author2, {'weight': r['length']})

nx.write_graphml(g,
                 'output/author_network_of_copying_relation.graphml')
