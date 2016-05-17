#!/usr/bin/env python3

import csv
import json

records = {}

with open('../eebo-tcp/tcp-texts/TCP.json') as f:
    data = json.load(f)
    for e in data["records"]:
        if e["EEBO"] == "":
            continue
        if e["Status"] != "Free":
            continue
        tcp = e["TCP"]
        records[tcp] = e

raw = []
with open('output-metadata/metadata.tsv') as f:
    with open('output-metadata/metadata.csv', 'w', newline='') as tf:
        writer = csv.writer(tf)
        writer.writerow([
            'TCP', 'Title', 'Author', 'Date', 'Extent', 'Pages', 'Publisher', 'Pub name', 'Pub place', 'Pub date'
        ])
        for l in f:
            row = l[:-1].split('\t')
            tcp, title, author, date, extent, publ, ppubl, pplace, pdate = row
            e = records[tcp]
            pages = e["Pages"]
            outrow = [tcp, title, author, date, extent, pages, publ, ppubl, pplace, pdate]
            writer.writerow(outrow)
