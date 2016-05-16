#!/usr/bin/env python3

import math
import re
import lxml
from lxml.builder import E
from collections import OrderedDict

maxlen = 500
maxoutside = 250
perfile = 100

def cls(x):
    return {"class": x}

def fixtext(x):
    if x is None:
        x = ''
    return re.sub(r"\s+", " ", x)

def before(x):
    if len(x) > maxoutside:
        return E.span(cls("before"), E.span(cls("abbr"), "… "), x[-maxoutside:])
    else:
        return E.span(cls("before"), x)

def after(x):
    if len(x) > maxoutside:
        return E.span(cls("after"), x[:maxoutside],  E.span(cls("abbr"), " …"))
    else:
        return E.span(cls("after"), x)

def content(x):
    if len(x) > 2.5 * maxlen:
        return E.span(cls("content"), x[:maxlen], E.span(cls("abbr"), " … "), x[-maxlen:])
    else:
        return E.span(cls("content"), x)

def menu(what, nfiles):
    buttons = []
    for i in ['index'] + list(range(nfiles)):
        if i == 'index':
            label = 'Index'
            link = './'
        else:
            label = '{}–{}'.format(perfile * i + 1, perfile * (i + 1))
            link = '{}.html'.format(i + 1)
        c = cls('btn selected') if i == what else cls('btn')
        buttons.append(E.span(c, E.a(label, href=link)))
    return E.div(cls('menu'), *buttons)

def doc(title, menubar, chunks, filename):
    doc = E.html({"lang": "en"},
        E.head(
            E.title(title),
            E.link(OrderedDict([("rel", "stylesheet"), ("href", "style.css"), ("type", "text/css")])),
            E.meta(charset="UTF-8"),
            E.meta(OrderedDict([("name", "viewport"), ("content", "width=device-width, initial-scale=1")])),
        ),
        E.body(menubar, *chunks)
    )

    with open("html/" + filename, "w") as f:
        f.write("<!DOCTYPE html>\n")
        f.write(lxml.etree.tostring(doc, method="html", encoding=str))
        f.write("\n")

def main():
    results = lxml.etree.parse("output/results.xml").getroot()

    seen = set()

    chunks = []
    chunks2 = []

    i = 1
    for chunk in results:
        ids = tuple(sorted(text.attrib["code"] for text in chunk))
        if ids in seen:
            continue
        seen.add(ids)

        texts = []
        texts2 = []
        for text in chunk:
            code = text.attrib["code"]
            author = text.attrib["author"]
            texts.append(E.div(cls("text"),
                E.div(cls("authorline"),
                    E.span(cls("code"), E.a(code, href="https://github.com/textcreationpartnership/" + code)),
                    E.span(cls("author unknown"), 'unknown?') if author == '' else E.div(cls("author"), author),
                ),
                E.div(cls("title"), text.attrib["title"]),
                E.div(cls("publ"), text.attrib["publ"]),
                E.div(cls("samplewrap"),
                    E.div(cls("sample"),
                        before(fixtext(text.find("before").text)),
                        content(fixtext(text.find("content").text)),
                        after(fixtext(text.find("after").text)),
                    )
                )
            ))
            texts2.append(E.div(cls("text"),
                E.div(cls("authorline"),
                    E.span(cls("code"), E.a(code, href="https://github.com/textcreationpartnership/" + code)),
                    E.span(cls("author unknown"), 'unknown?') if author == '' else E.div(cls("author"), author),
                ),
                E.div(cls("title"), text.attrib["title"]),
                E.div(cls("publ"), text.attrib["publ"]),
            ))
        id = "r{}".format(i)
        filename = math.ceil(i / perfile)
        chunks.append(E.div(cls("chunk detail"), {"id": id},
            E.div(cls("head"),
                E.a('{}: '.format(i),
                E.span(cls("sub"), str(format(chunk.attrib["length"]))),
                href="./#{}".format(id))
            ),
            *texts
        ))
        chunks2.append(E.div(cls("chunk main"), {"id": id},
            E.div(cls("head"),
                E.a('{}: '.format(i),
                E.span(cls("sub"), str(format(chunk.attrib["length"]))),
                href="{}.html#{}".format(filename, id))
            ),
            *texts2
        ))
        i += 1

    nfiles = math.ceil(len(chunks) / perfile)
    for k in range(nfiles):
        a = k * perfile
        b = min(a + perfile, len(chunks))
        doc(
            "EEBO TCP textual overlap {}/{}".format(k+1, nfiles),
            menu(k, nfiles),
            chunks[a:b],
            "{}.html".format(k+1)
        )

    doc(
        "EEBO TCP textual overlap",
        menu('index', nfiles),
        chunks2,
        'index.html'
    )

    print(len(chunks))

main()
