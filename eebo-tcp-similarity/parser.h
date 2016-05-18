#ifndef PARSER_H
#define PARSER_H

#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include "pugixml.hpp"
#include "char-map.h"
#include "utf8.h"

const char* xml_dir = "tcp-xml";

enum {
    PUBLISHER, PLACE, DATE, N_PARTS
};

static std::string trimmed(const std::string& x) {
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

static void trim(std::string& x) {
    x = trimmed(x);
}

static std::string join(std::string sep, const std::vector<std::string>& x) {
    std::string y;
    for (std::string p : x) {
        if (x.size()) {
            if (y.size()) {
                y += sep;
            }
            y += p;
        }
    }
    return y;
}


struct TextBase
{
    TextBase(std::string id_) : id{id_} {}
    std::string get_full_name() const {
        std::string fullname{xml_dir};
        fullname += "/";
        fullname += id;
        fullname += ".xml";
        return fullname;
    }
    void fail(std::string msg) {
        std::cerr << get_full_name() << ": " << msg << std::endl;
        std::exit(EXIT_FAILURE);
    }
    void parse_to(pugi::xml_document& doc) {
        unsigned opt = pugi::parse_default | pugi::parse_ws_pcdata;
        pugi::xml_parse_result result = doc.load_file(get_full_name().c_str(), opt, pugi::encoding_utf8);
        if (!result) {
            fail(result.description());
        }
    }

    std::string id;
};


struct Content : pugi::xml_tree_walker
{
    Content() : prev{0}, depth_seen{-1}
    {}

    virtual void feed_normalised(char c) = 0;
    virtual void feed(const char* q, const char* p) = 0;

    virtual bool for_each(pugi::xml_node& node) {
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
                if (c && c != prev) {
                    feed_normalised(c);
                    prev = c;
                }
                feed(q, p);
            }
        }
        return true;
    }

    void parse(pugi::xml_document& doc) {
        pugi::xml_node tei = doc.child("TEI");
        pugi::xml_node text = tei.child("text");
        assert(text);
        text.traverse(*this);
    }

    char prev;
    int depth_seen;
    std::string skipped{"desc"};
};


struct Metadata : pugi::xml_tree_walker
{
    virtual bool for_each(pugi::xml_node& node) {
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

    void parse(pugi::xml_document& doc) {
        pugi::xml_node tei = doc.child("TEI");
        pugi::xml_node head = tei.child("teiHeader");
        pugi::xml_node fileDesc = head.child("fileDesc");
        pugi::xml_node biblFull = fileDesc.child("sourceDesc").child("biblFull");
        for (auto node : fileDesc.child("titleStmt").children()) {
            if (node.type() == pugi::node_element) {
                if (!strcmp(node.name(), "author")) {
                    authors.push_back(trimmed(node.child_value()));
                }
            }
        }
        for (auto node : biblFull.child("titleStmt").children()) {
            if (node.type() == pugi::node_element) {
                if (!strcmp(node.name(), "title")) {
                    titles.push_back(node.child_value());
                } else if (!strcmp(node.name(), "author")) {
                    long_authors.push_back(trimmed(node.child_value()));
                }
            }
        }
        date = trimmed(fileDesc.child("editionStmt").child("edition").child_value("date"));
        extent = trimmed(biblFull.child_value("extent"));
        pugi::xml_node publStmt = biblFull.child("publicationStmt");
        depth_prev = 0;
        current = -1;
        publStmt.traverse(*this);
        trim(publ);
        for (int i = 0; i < N_PARTS; ++i) {
            trim(publ_parts[i]);
        }
    }

    std::string get_title() const { return join("; ", titles); }

    std::string get_author() const { return join("; ", authors); }

    std::string get_long_author() const { return join("; ", long_authors); }

    bool has_author(const std::string& x) {
        for (const std::string& a : authors) {
            auto hit = a.find(x);
            if (hit != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    std::vector<std::string> titles;
    std::vector<std::string> authors;
    std::vector<std::string> long_authors;
    std::string date;
    std::string extent;
    std::string publ;
    std::string publ_parts[N_PARTS];
    int depth_prev;
    int current;
};

#endif
