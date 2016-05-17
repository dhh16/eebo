#include <cassert>
#include <cctype>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include "pugixml.hpp"
#include "char-map.h"

const char* xml_dir = "tcp-xml";

enum {
    PUBLISHER, PLACE, DATE, N_PARTS
};


std::string clean(std::string x) {
    std::string y;
    bool seen_ws = false;
    for (char c : x) {
        if (std::isspace(c)) {
            seen_ws = true;
        } else {
            if (seen_ws && y.size()) {
                y += ' ';
            }
            seen_ws = false;
            y += c;
        }
    }
    return y;
}


struct Text : pugi::xml_tree_walker
{
    Text(std::string id_, int t) : id{id_} {}
    std::string get_full_name() const;
    void fail(std::string msg);
    void parse();
    void dump();
    virtual bool for_each(pugi::xml_node& node);

    std::string id;
    std::string title;
    std::string author;
    std::string date;
    std::string extent;
    std::string publ;
    std::string publ_parts[N_PARTS];
    int depth_prev;
    int current;
};

std::string Text::get_full_name() const {
    std::string fullname{xml_dir};
    fullname += "/";
    fullname += id;
    fullname += ".xml";
    return fullname;
}  

void Text::fail(std::string msg) {
    std::cerr << get_full_name() << ": " << msg << std::endl;
    std::exit(EXIT_FAILURE);
}

void Text::parse() {
    unsigned opt = pugi::parse_default | pugi::parse_ws_pcdata;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(get_full_name().c_str(), opt, pugi::encoding_utf8);
    if (!result) {
        fail(result.description());
        return;
    }
    pugi::xml_node tei = doc.child("TEI");
    pugi::xml_node head = tei.child("teiHeader");
    pugi::xml_node fileDesc = head.child("fileDesc");
    pugi::xml_node biblFull = fileDesc.child("sourceDesc").child("biblFull");
    for (auto node : biblFull.child("titleStmt").children()) {
        if (node.type() == pugi::node_element) {
            if (!strcmp(node.name(), "title")) {
                if (title.size()) {
                    title += "; ";
                }
                title += node.child_value();
            } else if (!strcmp(node.name(), "author")) {
                if (author.size()) {
                    author += "; ";
                }
                author += node.child_value();
            }
        }
    }
    date = fileDesc.child("editionStmt").child("edition").child_value("date");
    extent = biblFull.child_value("extent");
    pugi::xml_node publStmt = biblFull.child("publicationStmt");
    depth_prev = 0;
    current = -1;
    publStmt.traverse(*this);
}

void Text::dump() {
    std::vector<std::string> p;
    p.push_back(id);
    p.push_back(title);
    p.push_back(author);
    p.push_back(date);
    p.push_back(extent);
    p.push_back(publ);
    for (int i = 0; i < N_PARTS; ++i) {
        p.push_back(publ_parts[i]);
    }
    for (int i = 0; i < p.size(); ++i) {
        if (i) {
            std::cout << "\t";
        }
        std::cout << clean(p[i]);
    }
    std::cout << "\n";
}

bool Text::for_each(pugi::xml_node& node) {
    using namespace pugi;
    if (node.type() == node_element && depth() <= depth_prev) {
        if (!strcmp(node.name(), "publisher")) {
            current = PUBLISHER;
        } else if (!strcmp(node.name(), "pubPlace")) {
            current = PLACE;
        } else if (!strcmp(node.name(), "date")) {
            current = DATE;
        } else {
            current = -1;
        }
        depth_prev = depth();
    } else if (node.type() == node_pcdata) {
        const char* p = node.value();
        publ += p;
        if (current != -1) {
            publ_parts[current] += p;
        }
    }
    return true;
}


int main() {
    std::ios_base::sync_with_stdio(0);
    std::vector<Text> texts;
    std::string id;
    while (std::cin >> id) {
        int t = texts.size();
        texts.emplace_back(Text(id, t));
    }
    const int n = texts.size();

    #pragma omp parallel for
    for (int t = 0; t < n; ++t) {
        texts[t].parse();
    }

    for (int t = 0; t < n; ++t) {
        texts[t].dump();
    }
}
