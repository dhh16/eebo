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
    std::string title;
    std::string author;
    std::string date;
    std::string extent;
    std::string publ;
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
    pugi::xml_node titleStmt = fileDesc.child("titleStmt");
    pugi::xml_node biblFull = fileDesc.child("sourceDesc").child("biblFull");
    title = titleStmt.child_value("title");
    author = titleStmt.child_value("author");
    date = fileDesc.child("editionStmt").child("edition").child_value("date");
    extent = biblFull.child_value("extent");
    pugi::xml_node publStmt = biblFull.child("publicationStmt");
    publStmt.traverse(*this);
}

void Text::dump() {
    std::cout << title << "\n";
    std::cout << author << "\n";
    std::cout << date << "\n";
    std::cout << publ << "\n";
    std::cout << extent << "\n";
    std::cout << "\n";
}

bool Text::for_each(pugi::xml_node& node) {
    using namespace pugi;
    if (node.type() == node_pcdata) {
        const char* p = node.value();
        publ += p;
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
