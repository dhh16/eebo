#include <algorithm>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include "parser.h"
#include "timer.h"

const int context = 500;

typedef std::pair<int, int> pi;


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
    Text(std::string id_, int t) : TextBase(id_), relevant{false} {}

    void parse() {
        pugi::xml_document doc;
        parse_to(doc);
        metadata.parse(doc);
        content.parse(doc);
        content.index.push_back(content.content.size());
    }

    bool relevant;
    MyContent content;
    Metadata metadata;
};


struct Chunk
{
    int size;
    bool good;
    std::vector<pi> positions;
};


struct Analyse
{
public:
    Analyse(const char* output_file_, const char* filename_, const char* author_limit_, bool sametext_)
        : output_file{output_file_}, filename{filename_}, author_limit{author_limit_}, sametext{sametext_}
    {}
    void process();

private:
    void find_chunk_size(int i);
    void verify_chunk(int i);
    char peek_chunk(const Chunk& chunk, int j);
    bool ok_chunk(const Chunk& chunk);

    const char* output_file;
    const char* filename;
    const char* author_limit;
    bool sametext;
    std::vector<Text> texts;
    std::vector<Chunk> chunks;
    Timer timer;
};

void Analyse::process() {
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
        std::unordered_set<int> text_set;
        for (int i = 0; i < size; ++i) {
            int t;
            int pos;
            if (!(f >> t >> pos)) {
                std::cerr << filename << ": unexpected end of file" << std::endl;
                std::exit(EXIT_FAILURE);
            }
            assert(0 <= t);
            assert(t < n);
            chunk.positions.push_back(pi{t, pos});
            text_set.insert(t);
        }
        bool good;
        if (sametext) {
            good = (text_set.size() < size);
        } else {
            good = (text_set.size() > 1);
        }
        if (!good) {
            continue;
        }
        for (int i = 0; i < chunk.positions.size(); ++i) {
            pi p = chunk.positions[i];
            texts[p.first].relevant = true;
        }
        chunks.push_back(chunk);
    }

    int relevant = 0;
    for (int t = 0; t < n; ++t) {
        if (texts[t].relevant) {
            ++relevant;
        }
    }
    std::cout << "\trelevant = " << relevant << std::endl;

    timer.log("Parse relevant files");
    #pragma omp parallel for
    for (int t = 0; t < n; ++t) {
        if (texts[t].relevant) {
            texts[t].parse();
        }
    }

    timer.log("Analyse chunks");
    #pragma omp parallel for
    for (int i = 0; i < chunks.size(); ++i) {
        find_chunk_size(i);
        verify_chunk(i);
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
        if (!chunk.good) {
            continue;
        }
        pugi::xml_node xchunk = xroot.append_child("chunk");
        xchunk.append_attribute("length") = chunk.size;
        for (pi p : chunk.positions) {
            pugi::xml_node xtext = xchunk.append_child("text");
            const Text& text = texts[p.first];
            const int pos = p.second;
            xtext.append_attribute("code") = text.id.c_str();
            xtext.append_attribute("author") = text.metadata.get_author().c_str();
            xtext.append_attribute("longauthor") = text.metadata.get_long_author().c_str();
            xtext.append_attribute("title") = text.metadata.get_title().c_str();
            xtext.append_attribute("date") = text.metadata.date.c_str();
            xtext.append_attribute("publ") = text.metadata.publ.c_str();
            xtext.append_attribute("position") = pos;
            pugi::xml_node xb = xtext.append_child("before");
            pugi::xml_node xc = xtext.append_child("content");
            pugi::xml_node xa = xtext.append_child("after");
            int i1 = pos;
            int i2 = pos + chunk.size;
            int i0 = i1 - context;
            int i3 = i2 + context;
            int n = text.content.normalised.size();
            i0 = std::max(i0, 0);
            i3 = std::min(i3, n);
            i0 = i0 ? text.content.index[i0] : 0;
            i1 = text.content.index[i1];
            i2 = text.content.index[i2];
            i3 = text.content.index[i3];
            xb.text() = text.content.content.substr(i0, i1 - i0).c_str();
            xc.text() = text.content.content.substr(i1, i2 - i1).c_str();
            xa.text() = text.content.content.substr(i2, i3 - i2).c_str();
        }
        ++c;
    }
    if (!xresults.save_file(output_file)) {
        std::cerr << output_file << ": could not create" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::cout << "\tc = " << c << std::endl;
    timer.log("Done");
}

char Analyse::peek_chunk(const Chunk& chunk, int j) {
    const Text& text = texts[chunk.positions[j].first];
    const int start = chunk.positions[j].second;
    if (start + chunk.size >= text.content.normalised.size()) {
        return -1;
    } else {
        return text.content.normalised[start + chunk.size];
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

void Analyse::verify_chunk(int i) {
    Chunk& chunk = chunks[i];
    chunk.good = true;
    if (sametext) {
        return;
    }
    if (author_limit) {
        chunk.good = false;
        for (pi p : chunk.positions) {
            if (texts[p.first].metadata.has_author(author_limit)) {
                chunk.good = true;
            }
        }
    }
    if (chunk.good) {
        std::unordered_map<std::string, int> author_count;
        for (pi p : chunk.positions) {
            const auto& authors = texts[p.first].metadata.authors;
            for (const auto& a : authors) {
                ++author_count[a];
            }
        }
        for (const auto& p : author_count) {
            if (p.second >= chunk.positions.size()) {
                chunk.good = false;
            }
        }
    }
}


int main(int argc, const char** argv) {
    std::ios_base::sync_with_stdio(0);
    if (argc < 3) {
        std::cerr << "usage: similarity OUTPUT_XML INPUT_FILE [AUTHOR|--same-text] < EEBO_LIST" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    const char* output_file = argv[1];
    const char* filename = argv[2];
    const char* author_limit = NULL;
    bool sametext = false;
    if (argc > 3) {
        if (!strcmp(argv[3], "--same-text")) {
            sametext = true;
        } else {
            author_limit = argv[3];
        }
    }
    Analyse analyse(output_file, filename, author_limit, sametext);
    analyse.process();
}
