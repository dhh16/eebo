#include <cassert>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include "pugixml.hpp"
#include "utf8.h"
#include "char-map.h"
#include "timer.h"

const char* xml_dir = "tcp-xml";


struct Text : pugi::xml_tree_walker
{
    Text(std::string id_, int t) : id{id_} {}
    std::string get_full_name() const;
    void fail(std::string msg);
    void parse();
    void dump();
    virtual bool for_each(pugi::xml_node& node);

    std::string id;
    std::string content;
    std::string normalised;
    std::vector<int> index;
    std::vector<int> positions;
    char walk_prev;
    int depth_seen;
    std::string title;
    std::string author;
    std::string date;
    std::string publ;
    std::string skipped{"desc"};
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
    title = head.child("fileDesc").child("titleStmt").child_value("title");
    author = head.child("fileDesc").child("titleStmt").child_value("author");
    date = head.child("fileDesc").child("editionStmt").child("edition").child_value("date");
    pugi::xml_node publStmt = head.child("fileDesc").child("sourceDesc").child("biblFull").child("publicationStmt");
    bool add_ws = false;
    for (auto n : publStmt) {
        if (n.type() == pugi::node_element) {
            if (add_ws && publ.size()) {
                publ += ' ';
            }
            publ += n.child_value();
            add_ws = false;
        } else {
            add_ws = true;
        }
    }

    pugi::xml_node text = tei.child("text");
    if (!text) {     
        fail("could not find TEI/text");
    }

    depth_seen = -1;
    walk_prev = 0;
    text.traverse(*this);
    index.push_back(content.size());
}

void Text::dump() {
    std::cout << title << "\n";
    std::cout << author << "\n";
    std::cout << date << "\n";
    std::cout << publ << "\n";
    std::cout << "\n";
}

bool Text::for_each(pugi::xml_node& node) {
    using namespace pugi;
    if (depth_seen != -1 && depth() <= depth_seen) {
        depth_seen = -1;
    }
    if (depth_seen == -1 && node.type() == node_element && skipped == node.name()) {
        depth_seen = depth();
    }
    if (depth_seen == -1 && node.type() == node_pcdata) {
        const char* p = node.value();
        const char* e = p + std::strlen(p);
        while (p != e) {
            const char* q = p;
            uint32_t v = utf8::next(p, e);
            char c = normalise(v);
            if (c && c != walk_prev) {
                index.push_back(content.size());
                normalised += c;
                walk_prev = c;
            }
            content.append(q, p);
        }
    }
    return true;
}


int main() {
    std::ios_base::sync_with_stdio(0);
    Timer timer;
    std::vector<Text> texts;
    timer.log("Read file list");
    std::string id;
    while (std::cin >> id) {
        int t = texts.size();
        texts.emplace_back(Text(id, t));
    }
    const int n = texts.size();

    timer.log("Parse files");
    #pragma omp parallel for
    for (int t = 0; t < n; ++t) {
        texts[t].parse();
    }

    for (int t = 0; t < n; ++t) {
        texts[t].dump();
    }

    timer.log("Done");
}
