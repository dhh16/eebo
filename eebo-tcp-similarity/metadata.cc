#include "parser.h"

struct Text : TextBase
{
    Text(std::string id_) : TextBase(id_) {}

    void parse() {
        pugi::xml_document doc;
        parse_to(doc);
        metadata.parse(doc);
    }

    void dump() {
        std::vector<std::string> p;
        p.push_back(id);
        p.push_back(metadata.get_title());
        p.push_back(metadata.get_long_author());
        p.push_back(metadata.date);
        p.push_back(metadata.extent);
        p.push_back(metadata.publ);
        for (int i = 0; i < N_PARTS; ++i) {
            p.push_back(metadata.publ_parts[i]);
        }
        std::cout << join("\t", p) << "\n";
    }

    Metadata metadata;
};


int main() {
    std::ios_base::sync_with_stdio(0);
    std::vector<Text> texts;
    std::string id;
    while (std::cin >> id) {
        texts.emplace_back(Text(id));
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
