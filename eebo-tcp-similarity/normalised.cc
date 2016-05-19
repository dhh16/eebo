#include <algorithm>
#include "parser.h"

static std::string light_trimmed(const std::string& x) {
    std::string y;
    bool seen_ws = false;
    for (char c : x) {
        if (std::isspace(c)) {
            seen_ws = true;
        } else {
            if (seen_ws) {
                y += ' ';
            }
            seen_ws = false;
            y += c;
        }
    }
    if (seen_ws) {
        y += ' ';
    }
    return y;
}

struct MyContent : Content
{
    virtual void feed_normalised(char c) final {
        index.push_back(content.size());
        normalised += c;
    }

    virtual void feed(const char* q, const char* p) final {
        content.append(q, p);
    }

    std::string content;
    std::string normalised;
    std::vector<int> index;
};

struct Text : TextBase
{
    Text(std::string id_) : TextBase(id_) {}

    void parse() {
        pugi::xml_document doc;
        parse_to(doc);
        content.parse(doc);
        content.index.push_back(content.content.size());
    }

    void dump() {
        std::cout << content.normalised << "\n";
    }

    void dump_parallel() {
        int a = 0;
        for (int i = 0; i < content.normalised.size(); ++i) {
            int b = content.index[i + 1];
            std::cout << content.normalised[i] << "\t|" << light_trimmed(content.content.substr(a, b-a)) << "|\n";
            a = b;
        }
    }

    MyContent content;
};

int main(int argc, const char** argv) {
    std::ios_base::sync_with_stdio(0);
    bool parallel = (argc > 1);
    std::string id;
    while (std::cin >> id) {
        Text text(id);
        text.parse();
        if (parallel) {
            text.dump_parallel();
        } else {
            text.dump();
        }
    }
}
