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
const int context = 500;

typedef std::pair<int, int> pi;


struct Text : pugi::xml_tree_walker
{
    Text(std::string id_, int t) : id{id_} {}
    std::string get_full_name() const;
    void fail(std::string msg);
    void parse();
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



struct Chunk
{
    int size;
    int authors;
    std::vector<pi> positions;
};


struct Analyse
{
public:
    void process(const char* output_file, const char* filename);

private:
    void find_chunk_size(int i);
    void count_chunk_authors(int i);
    char peek_chunk(const Chunk& chunk, int j);
    bool ok_chunk(const Chunk& chunk);

    std::vector<Text> texts;
    std::vector<Chunk> chunks;
    Timer timer;
};

void Analyse::process(const char* output_file, const char* filename) {
    timer.log("Read file list");
    std::string id;
    while (std::cin >> id) {
        int t = texts.size();
        texts.emplace_back(Text(id, t));
    }
    const int n = texts.size();

    timer.log("Read input file");
    std::ifstream f(filename);
    if (!f) {
        std::cerr << filename << ": error reading file" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    while (true) {
        int size;
        if (!(f >> size)) {
            break;
        }
        assert(size >= 2);
        Chunk chunk;
        for (int i = 0; i < size; ++i) {
            int t;
            int pos;
            if (!(f >> t >> pos)) {
                std::cerr << filename << ": unexpected end of file" << std::endl;
                std::exit(EXIT_FAILURE);
            }
            assert(0 <= t);
            assert(t < n);
            texts[t].positions.push_back(pos);
            chunk.positions.push_back(pi{t, pos});
        }
        chunks.push_back(chunk);
    }

    int relevant = 0;
    for (int t = 0; t < n; ++t) {
        if (texts[t].positions.size()) {
            ++relevant;
        }
    }
    std::cout << "\trelevant = " << relevant << std::endl;

    timer.log("Parse relevant files");
    #pragma omp parallel for
    for (int t = 0; t < n; ++t) {
        if (texts[t].positions.size()) {
            texts[t].parse();
        }
    }

    timer.log("Analyse chunks");
    #pragma omp parallel for
    for (int i = 0; i < chunks.size(); ++i) {
        find_chunk_size(i);
        count_chunk_authors(i);
    }

    timer.log("Sort chunks");
    std::sort(chunks.begin(), chunks.end(), [](Chunk& a, Chunk& b) {
        return b.size < a.size ||
            (a.size == b.size && a.positions[0] < b.positions[0]);
    });

    timer.log("Save results");
    int c = 0;
    pugi::xml_document xresults;
    pugi::xml_node xroot = xresults.append_child("results");
    for (const Chunk& chunk : chunks) {
        if (chunk.authors == 1) {
            continue;
        }
        pugi::xml_node xchunk = xroot.append_child("chunk");
        xchunk.append_attribute("length") = chunk.size;
        for (pi p : chunk.positions) {
            pugi::xml_node xtext = xchunk.append_child("text");
            const Text& text = texts[p.first];
            const int pos = p.second;
            xtext.append_attribute("code") = text.id.c_str();
            xtext.append_attribute("author") = text.author.c_str();
            xtext.append_attribute("title") = text.title.c_str();
            xtext.append_attribute("date") = text.date.c_str();
            xtext.append_attribute("publ") = text.publ.c_str();
            xtext.append_attribute("position") = pos;
            pugi::xml_node xb = xtext.append_child("before");
            pugi::xml_node xc = xtext.append_child("content");
            pugi::xml_node xa = xtext.append_child("after");
            int i1 = pos;
            int i2 = pos + chunk.size;
            int i0 = i1 - context;
            int i3 = i2 + context;
            int n = text.normalised.size();
            i0 = std::max(i0, 0);
            i3 = std::min(i3, n);
            i0 = i0 ? text.index[i0] : 0;
            i1 = text.index[i1];
            i2 = text.index[i2];
            i3 = text.index[i3];
            xb.text() = text.content.substr(i0, i1 - i0).c_str();
            xc.text() = text.content.substr(i1, i2 - i1).c_str();
            xa.text() = text.content.substr(i2, i3 - i2).c_str();
        }
        ++c;
    }
    xresults.save_file(output_file);
    std::cout << "\tc = " << c << std::endl;
    timer.log("Done");
}

char Analyse::peek_chunk(const Chunk& chunk, int j) {
    const Text& text = texts[chunk.positions[j].first];
    const int start = chunk.positions[j].second;
    if (start + chunk.size >= text.normalised.size()) {
        return -1;
    } else {
        return text.normalised[start + chunk.size];
    }
}

bool Analyse::ok_chunk(const Chunk& chunk) {
    char a = peek_chunk(chunk, 0);
    if (a == -1) {
        return false;
    }
    for (int j = 1; j < chunk.positions.size(); ++j) {
        char b = peek_chunk(chunk, j);
        if (a != b) {
            return false;
        }
    }
    return true;
}

void Analyse::find_chunk_size(int i) {
    Chunk& chunk = chunks[i];
    chunk.size = 0;
    while (ok_chunk(chunk)) {
        ++chunk.size;
    }
}


void Analyse::count_chunk_authors(int i) {
    Chunk& chunk = chunks[i];
    std::unordered_set<std::string> authors;
    for (pi p : chunk.positions) {
        authors.insert(texts[p.first].author);
    }
    chunk.authors = authors.size();
}


int main(int argc, const char** argv) {
    std::ios_base::sync_with_stdio(0);
    if (argc != 3) {
        std::cerr << "usage: similarity OUTPUT_XML INPUT_FILE < EEBO_LIST" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    Analyse analyse;
    analyse.process(argv[1], argv[2]);
}
