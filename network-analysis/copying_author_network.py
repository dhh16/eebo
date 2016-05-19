import pandas as pd
import networkx as nx
from itertools import combinations

def main():
    import argparse
    from util import makedir
    parser = argparse.ArgumentParser()
    parser.add_argument('--cp_df_path')
    parser.add_argument('--txt_df_path')
    parser.add_argument('--use_author_long',
                        action='store_true',
                        default=False)

    parser.add_argument('--output_dir')

    args = parser.parse_args()
    g = nx.Graph()


    cp_df = pd.read_pickle(args.cp_df_path)
    tx_df = pd.read_pickle(args.txt_df_path)


    for i, r in cp_df.iterrows():
        for d1, d2 in combinations(r['doc_ids'], 2):
            if args.use_author_long:
                authors1 = tx_df.loc[d1, 'author_long'].split(';')
                authors2 = tx_df.loc[d2, 'author_long'].split(';')
            else:
                authors1 = tx_df.loc[d1, 'author_short'].split(';')
                authors2 = tx_df.loc[d2, 'author_short'].split(';')

            for author1 in authors1:
                for author2 in authors2:
                    author1 = author1.strip()
                    author2 = author2.strip()

                    if not author1 or not author2:
                        continue

                    if author1 == author2:  # self-loop
                        continue

                    g.add_node(author1, {'Label': author1})
                    g.add_node(author2, {'Label': author2})
                    if g.has_edge(author1, author2):
                        g[author1][author2]['weight'] += r['length']
                    else:
                        g.add_edge(author1, author2, {'weight': r['length']})

    suffix = ('author_long' if args.use_author_long else 'author_short')
    nx.write_graphml(
        g,
        '{}/author_network_of_copying_relation_{}.graphml'.format(
            args.output_dir,
            suffix)
        )


if __name__ == "__main__":
    main()
