#include <climits>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include "parser.h"
#include "timer.h"

constexpr int report_threshold = 200000000;
constexpr int size_threshold = 50;

typedef std::pair<unsigned, unsigned> pui;
typedef std::pair<pui, pui> ppui;
constexpr unsigned M {UINT_MAX};
constexpr int sort_threshold = 1000000;

struct puihash {
    size_t operator()(pui a) const {
        unsigned long long x = a.first;
        x <<= 32;
        x |= a.second;
        return std::hash<unsigned long long>()(x);
    }
};

inline pui prev(pui a) {
    --a.second;
    return a;
}

template<class Cmp>
void mysort_rec(ppui* data, long long n, Cmp cmp) {
    if (n < sort_threshold) {
        std::sort(data, data + n, cmp);
        return;
    }
    ppui pivot = data[n/2];
    auto lt = [pivot,cmp](ppui x) { return cmp(x, pivot); };
    auto lteq = [pivot,cmp](ppui x) { return !cmp(pivot, x); };
    ppui* end = data + n;
    ppui* mid1 = std::partition(data, end, lt);
    ppui* mid2 = std::partition(mid1, end, lteq);
    ppui* data1 = data;
    ppui* data2 = mid2;
    long long n1 = mid1 - data;
    long long n2 = end - mid2;
    #pragma omp task
    mysort_rec(data1, n1, cmp);
    #pragma omp task
    mysort_rec(data2, n2, cmp);
}

template<class Cmp>
void mysort(ppui* data, long long n, Cmp cmp) {
    #pragma omp parallel
    #pragma omp single
    mysort_rec(data, n, cmp);
}


struct MyContent : Content
{
    MyContent(ppui* target_, int t_) : target{target_}, t{t_}, i{0} {}

    virtual void feed_normalised(char c) final {
        if (target) {
            target[i].first.first = 0;
            target[i].first.second = c;
            target[i].second.first = t;
            target[i].second.second = i;
        }
        ++i;
    }

    virtual void feed(const char* q, const char* p) final {}

    ppui* const target;
    const int t;
    int i;
};


class Text : TextBase
{
public:
    Text(std::string id_, int t) : TextBase(id_), t{t}, length{-1} {}

    void parse(ppui* target) {
        pugi::xml_document doc;
        parse_to(doc);
        MyContent content(target, t);
        content.parse(doc);
        if (target) {
            assert(length == content.i);
        } else {
            length = content.i;
        }
    }

    std::string id;
    int t;
    int length;
};


class Similarity
{
public:
    void do_all(const char* result_dir);

private:
    void report(const char* result_dir);
    std::vector<Text> texts;
    ppui* data;
    long long c;
    long long s;
    long long step;
    Timer timer;
};


void Similarity::do_all(const char* result_dir) {
    timer.log("Read file list");
    std::string id;
    while (std::cin >> id) {
        int t = texts.size();
        texts.emplace_back(Text(id, t));
    }
    int n = texts.size();

    timer.log("Measure files");
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        texts[i].parse(NULL);
    }

    std::vector<long long> start(n);
    s = 0;
    for (int i = 0; i < n; ++i) {
        start[i] = s;
        s += texts[i].length;
    }
    std::cout << "\ts = " << s << std::endl;
    assert(s < M);
    data = new ppui[s];

    timer.log("Read files");
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        texts[i].parse(data + start[i]);
    }

    step = 4;
    if (s < step) {
        return;
    }

    timer.log("Pack");
    #pragma omp parallel for
    for (long long i = 0; i < s - step + 1; ++i) {
        ppui& a = data[i];
        ppui& b = data[i + step - 1];
        if (a.second.first == b.second.first) {
            unsigned v = 0;
            for (int j = 0; j < step; ++j) {
                v <<= 8;
                v |= data[i + j].first.second;
            }
            data[i].first.first = v;
        } else {
            data[i].first.first = M;
        }
    }
    timer.log("Remove useless");
    s = std::remove_if(data, data + s - step + 1, [](ppui a){return a.first.first == M;}) - data;
    std::cout << "\ts = " << s << std::endl;

    while (s && step <= s) {
        std::cout << "\tstep = " << step << std::endl;
        timer.log("Form pairs");
        #pragma omp parallel for
        for (long long i = 0; i < s - step; ++i) {
            ppui& a = data[i];
            ppui& b = data[i+step];
            if (a.second.first == b.second.first && a.second.second + step == b.second.second) {
                a.first.second = b.first.first;
            } else {
                a.first.second = M;
            }
        }
        timer.log("Remove useless");
        s = std::remove_if(data, data + s - step, [](ppui a){return a.first.second == M;}) - data;
        std::cout << "\ts = " << s << std::endl;
        timer.log("Sort by content");
        mysort(data, s, [](ppui a, ppui b) {return a.first < b.first;});
        timer.log("Relabel");
        c = 0;
        {
            long long i = 0;
            while (i < s) {
                long long j = i + 1;
                while (j < s && data[i].first == data[j].first) {
                    ++j;
                }
                if (j == i + 1) {
                    data[i].first.first = M;
                } else {
                    for (long long l = i; l < j; ++l) {
                        data[l].first.first = c;
                    }
                    ++c;
                }
                i = j;
            }
        }
        std::cout << "\tc = " << c << std::endl;
        timer.log("Remove useless");
        s = std::remove_if(data, data + s, [](ppui a){return a.first.first == M;}) - data;
        std::cout << "\ts = " << s << std::endl;
        step *= 2;

        if (0 < s && s <= report_threshold && step >= size_threshold) {
            report(result_dir);
        }

        timer.log("Sort by position");
        mysort(data, s, [](ppui a, ppui b) {return a.second < b.second;});
    }

    delete[] data;

    timer.log("Done");
}

void Similarity::report(const char* result_dir) {
    timer.log("Clear garbage");
    #pragma omp parallel for
    for (long long i = 0; i < s; ++i) {
        data[i].first.second = 0;
    }
    timer.log("Sort fully");
    mysort(data, s, [](ppui a, ppui b) {return a < b;});

    timer.log("Create map");
    std::unordered_map<pui, unsigned, puihash> chunk;
    chunk.reserve(s);
    for (long long i = 0; i < s; ++i) {
        ppui& p = data[i];
        assert(chunk.count(p.second) == 0);
        chunk[p.second] = p.first.first;
    }

    timer.log("Prune dominated");
    std::string filename(result_dir);
    filename += "/raw-";
    filename += std::to_string(step);
    filename += ".txt";
    std::cout << "\tfilename = " << filename << std::endl;
    std::ofstream f(filename);
    if (!f) {
        std::cerr << filename << ": error creating file" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    long long distinct = 0;
    long long elements = 0;
    for (long long i = 0; i < s;) {
        long long j = i + 1;
        while (j < s && data[i].first == data[j].first) {
            ++j;
        }
        assert(j >= i + 2);
        unsigned common = 0;
        bool good = false;
        for (long long l = i; l < j; ++l) {
            auto hit = chunk.find(prev(data[l].second));
            if (hit == chunk.end()) {
                good = true;
                break;
            } else if (l == i) {
                common = hit->second;
            } else if (common != hit->second) {
                good = true;
                break;
            }
        }
        if (good) {
            f << (j - i) << "\n";
            for (long long l = i; l < j; ++l) {
                ppui& a = data[l];
                f << a.second.first << "\t" << a.second.second << "\n";
                ++elements;
            }
            ++distinct;
        }
        i = j;
    }
    f.close();
    if (!f) {
        std::cerr << filename << ": write error" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::cout << "\tdistinct = " << distinct << std::endl;
    std::cout << "\telements = " << elements << std::endl;
}


int main(int argc, const char** argv) {
    std::ios_base::sync_with_stdio(0);
    if (argc != 2) {
        std::cerr << "usage: similarity RESULT_DIR < EEBO_LIST" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    const char* result_dir = argv[1];
    Similarity sim;
    sim.do_all(result_dir);
}
