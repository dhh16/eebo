import pandas as pd
from bs4 import BeautifulSoup
from codecs import open


def load_doc(path):
    return BeautifulSoup(open(path, 'r', 'utf8'), 'html.parser')


def extract_text_info(n):
    return {
        "id": n['code'],
        "author_raw": n['author'],
        "title_raw": n['title'],
        "date_raw": n['date'],
        "publ_raw": n['publ'],
        "position_raw": n['position']
        }


def extract_chunk_info(chunk_node):
    text_nodes = chunk_node.find_all('text')
    return [extract_text_info(n) for n in text_nodes]


PATH = "/cs/home/hxiao/code/eebo/eebo-tcp-similarity/output/results.xml"

doc = load_doc(PATH)

copying_data = []
copying_columns = ('id', 'length', 
                   'doc_ids')
text_data = {}

for i, chunk in enumerate(doc.find_all("chunk")):
    texts = extract_chunk_info(chunk)
    copying_data.append(
        (i, int(chunk['length']), [t['id'] for t in texts])
        )
    for t in texts:
        text_data[t['id']] = t

cp_df = pd.DataFrame(copying_data, columns=copying_columns)
text_df = pd.DataFrame.from_records(text_data.values())

cp_df.to_pkl('output/copying_dataframe.pkl')
text_df.to_pkl('output/text_dataframe.pkl')

