import pandas as pd
import networkx as nx
from itertools import combinations

def main():
    import argparse
    from util import makedir
    parser = argparse.ArgumentParser()
    parser.add_argument('--cp_df_path')
    parser.add_argument('--txt_df_path')

    parser.add_argument('--output_dir')

    args = parser.parse_args()
    
    cp_df = pd.read_pickle(args.cp_df_path)
    tx_df = pd.read_pickle(args.txt_df_path)

    g = nx.Graph()


    for i, r in cp_df.iterrows():
        for d1, d2 in combinations(r['doc_ids'], 2):
            d1d = tx_df.loc[d1].to_dict()
            d2d = tx_df.loc[d1].to_dict()
            d1d['Label'] = d1
            d2d['Label'] = d2
            g.add_node(d1, d1d)
            g.add_node(d2, d2d)
            if g.has_edge(d1, d2):
                g[d1][d2]['weight'] += r['length']
            else:
                g.add_edge(d1, d2, {'weight': r['length']})

    nx.write_graphml(
        g,
        '{}/text_network_of_copying_relation.graphml'.format(args.output_dir)
    )

if __name__ == "__main__":
    main()
