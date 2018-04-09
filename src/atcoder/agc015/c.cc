/*
URL https://beta.atcoder.jp/contests/agc015/tasks/agc015_c
SCORE 700
AC true
WA false
TLE false
MLE false
TASK_TYPE グラフ 木 森 累積和
FAILURE_TYPE 問題読解
NOTES
・森であることの条件が読み取りづらい
・連結成分を頂点と辺の差へ帰着
・二次元累積和（長方形へのクエリを扱う場合によくあるらしい）
*/
#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include <cstdint>
#include <queue>

using namespace std;

// Type aliases
using i8  =  int8_t; using  u8 =  uint8_t;
using i16 = int16_t; using u16 = uint16_t;
using i32 = int32_t; using u32 = uint32_t;
using i64 = int64_t; using u64 = uint64_t;

template <class T> using V = vector<T>;

// Loops
#define REP(i, n) for (int i = 0; i < (n); ++i)
#define REPR(i, n) for (int i = (n) - 1; i >= 0; --i)
#define FOR(i, n, m) for (int i = (n); i < (m); ++i)
#define FORR(i, n, m) for (int i = (m) - 1; i >= (n); --i)

#define FORE(x, xs) for (auto &x: (xs))

// Utils for Tuple
namespace tuple_utils {
    template<size_t...> struct seq{};

    template<size_t N, size_t... Is>
    struct gen_seq : gen_seq<N - 1, N - 1, Is...>{};

    template<size_t... Is>
    struct gen_seq<0, Is...> : seq<Is...>{};

    template <class Tuple, size_t... Is>
    void read(istream &stream, Tuple &t, seq<Is...>) {
        static_cast<void>((int[]){0, (void(stream  >> get<Is>(t)), 0)...});
    }

    template<class Tuple, size_t... Is>
    void print(ostream& stream, Tuple const& t, seq<Is...>) {
        static_cast<void>((int[]){0, (void(stream << (Is == 0 ?  "" : ", ") << get<Is>(t)), 0)...});
    }

    template <size_t I, class F, class A, class... Elems>
    struct ForEach {
        void operator()(A &arg, tuple<Elems...> const& t) const {
            F()(arg, get<I>(t));
            ForEach<I - 1, F, A, Elems...>()(arg, t);
        };
        void operator()(A &arg, tuple<Elems...>& t) const {
            F()(arg, get<I>(t));
            ForEach<I - 1, F, A, Elems...>()(arg, t);
        };
    };
    template <class F, class A, class... Elems>
    struct ForEach<0, F, A, Elems...> {
        void operator()(A &arg, tuple<Elems...> const& t) const {
            F()(arg, get<0>(t));
        };
        void operator()(A &arg, tuple<Elems...>& t) const {
            F()(arg, get<0>(t));
        };
    };
    template <class F, class A, class... Elems>
    void for_each(A &arg, tuple<Elems...> const& t) {
        ForEach<tuple_size<tuple<Elems...>>::value - 1, F, A, Elems...>()(arg, t);
    };
    template <class F, class A, class... Elems>
    void for_each(A &arg, tuple<Elems...>& t) {
        ForEach<tuple_size<tuple<Elems...>>::value - 1, F, A, Elems...>()(arg, t);
    };

    struct hash_for_element {
        template <class V>
        void operator()(size_t &size, const V& v) const {
            size ^= hash<V>()(v);
        }
    };
}

// Input
template <class F, class S>
istream &operator>>(istream &stream, pair<F, S> &pair) {
    stream >> pair.first;
    stream >> pair.second;
    return stream;
}
template <class ...Args>
istream &operator>>(istream &stream, tuple<Args...> &tuple) {
    tuple_utils::read(stream, tuple, tuple_utils::gen_seq<sizeof...(Args)>());
    return stream;
}

template <class T>
T read() {
    T t;
    cin >> t;
    return t;
}
template <class F, class S>
pair<F, S> read() {
    pair<F, S> p;
    cin >> p;
    return p;
}
template <class T1, class T2, class T3, class ...Args>
tuple<T1, T2, T3, Args...> read() {
    tuple<T1, T2, T3, Args...> t;
    cin >> t;
    return t;
}

template <class T>
V<T> read(const int length) {
    V<T> ts(length);
    for (auto& t: ts) {
        cin >> t;
    }
    return ts;
}
template <class F, class S>
V<pair<F, S>> read(const int length) {
    V<pair<F, S>> ps(length);
    for (auto& p: ps) {
        cin >> p;
    }
    return ps;
}
template <class T1, class T2, class T3, class ...Args>
V<tuple<T1, T2, T3, Args...>> read(const int length) {
    V<tuple<T1, T2, T3, Args...>> ts(length);
    for (auto& t: ts) {
        cin >> t;
    }
    return ts;
}

// Output
namespace debug {
    template <class F, class S>
    ostream &operator<<(ostream& stream, const pair<F, S> &pair) {
        stream << "{" << pair.first << ", " << pair.second << "}";
        return stream;
    }
    template <class ...Args>
    ostream &operator<<(ostream& stream, const tuple<Args...> &tuple) {
        stream << "{";
        tuple_utils::print(stream, tuple, tuple_utils::gen_seq<sizeof...(Args)>());
        stream << "}";
        return stream;
    }

    template <class T, class Alloc>
    ostream &operator<<(ostream& stream, const vector<T, Alloc> &vector) {
        stream << "[";

        for (size_t i = 0; i < vector.size(); i++) {
            stream << vector[i];

            if (i != vector.size() - 1) {
                stream << "," << ((i % 10 == 9) ? "\n " : "\t");
            }
        }

        stream << "]";
        return stream;
    }
}

// Hash
namespace std {
    template <class F, class S>
    struct hash<pair<F, S>> {
        size_t operator ()(const pair<F, S> &p) const {
            return hash<F>()(p.first) ^ hash<S>()(p.second);
        }
    };

    template <class ...Args>
    struct hash<tuple<Args...>> {
        size_t operator ()(const tuple<Args...> &t) const {
            size_t retval = 0;

            tuple_utils::for_each<tuple_utils::hash_for_element, size_t, Args...>(retval, t);

            return retval;
        }
    };
}

// Data Structure
template <class T, class Compare = std::less<T>>
struct heap {
    heap () : m_heap(), m_cmp() {
    }
    heap (const V<T> &v) : m_heap(v), m_cmp() {
        make_heap(this->m_heap.begin(), this->m_heap.end(), this->m_cmp);
    }
    heap (const Compare &cmp) : m_heap(), m_cmp(cmp) {
    }
    heap (const V<T> &v, const Compare &cmp) : m_heap(v), m_cmp(cmp) {
        make_heap(this->m_heap.begin(), this->m_heap.end(), this->m_cmp);
    }

    void reserve(const size_t size) {
        this->m_heap.reserve(size);
    }

    const T &top() const {
        return this->m_heap.front();
    }
    T pop() {
        pop_heap(this->m_heap.begin(), this->m_heap.end(), this->m_cmp);
        auto retval = this->m_heap.back();
        this->m_heap.pop_back();
        return retval;
    }
    void push(const T &v) {
        this->m_heap.push_back(v);
        push_heap(this->m_heap.begin(), this->m_heap.end(), this->m_cmp);
    }

    size_t size() const {
        return this->m_heap.size();
    }

    auto begin() const {
        return this->m_heap.cbegin();
    }
    auto cbegin() const {
        return this->m_heap.cbegin();
    }
    auto end() const {
        return this->m_heap.cend();
    }
    auto cend() const {
        return this->m_heap.cend();
    }
private:
    V<T> m_heap;
    Compare m_cmp;
};

int body(int &argc, char **argv);

// main function (DO NOT EDIT)
int main (int argc, char **argv) {
    ios_base::sync_with_stdio(false);
    return body(argc, argv);
}

int body(int &argc, char **argv) {
    using namespace debug;

    auto N = read<i32>();
    auto M = read<i32>();
    auto Q = read<i64>();
    auto S = V<V<i16>>(N, V<i16>(M));
    REP (i, N) {
        auto str = read<string>();
        REP (j, M) {
            S[i][j] = str.at(j) - '0';
        }
    }
    auto qs = read<i32, i32, i32, i32>(Q);

    auto vertices_table = V<V<i32>>(N, V<i32>(M));
    auto hedges_table = V<V<i32>>(N, V<i32>(M));
    auto vedges_table = V<V<i32>>(N, V<i32>(M));

    // 前計算 (二次元累積和）
    REP (i, N) {
        REP (j, M) {

            // 頂点
            vertices_table[i][j] += S[i][j];
            if (i != 0 && j != 0) {
                vertices_table[i][j] += vertices_table[i-1][j];
                vertices_table[i][j] += vertices_table[i][j-1];
                vertices_table[i][j] -= vertices_table[i-1][j-1];
            } else if (i == 0 && j != 0) {
                vertices_table[i][j] += vertices_table[i][j-1];
            } else if (i != 0 && j == 0) {
                vertices_table[i][j] += vertices_table[i-1][j];
            }

            // 辺
            if (i != 0 && S[i][j] == 1 && S[i-1][j] == 1) {
                vedges_table[i][j] += 1;
            }
            if (j != 0 && S[i][j] == 1 && S[i][j-1] == 1) {
                hedges_table[i][j] += 1;
            }
            if (i != 0 && j != 0) {
                hedges_table[i][j] += hedges_table[i-1][j];
                hedges_table[i][j] += hedges_table[i][j-1];
                hedges_table[i][j] -= hedges_table[i-1][j-1];

                vedges_table[i][j] += vedges_table[i-1][j];
                vedges_table[i][j] += vedges_table[i][j-1];
                vedges_table[i][j] -= vedges_table[i-1][j-1];
            } else if (i == 0 && j != 0) {
                vedges_table[i][j] += vedges_table[i][j-1];
                hedges_table[i][j] += hedges_table[i][j-1];
            } else if (i != 0 && j == 0) {
                vedges_table[i][j] += vedges_table[i-1][j];
                hedges_table[i][j] += hedges_table[i-1][j];
            }
        }
    }

    FORE (q, qs) {
        auto x1 = get<0>(q) - 1;
        auto y1 = get<1>(q) - 1;
        auto x2 = get<2>(q) - 1;
        auto y2 = get<3>(q) - 1;

        i32 num_vertices = 0;
        i32 num_vedges = 0;
        i32 num_hedges = 0;
        i32 num_edges = 0;

        if (x1 == 0 && y1 == 0) {
            num_vertices = vertices_table[x2][y2];
            num_vedges = vedges_table[x2][y2];
            num_hedges = hedges_table[x2][y2];
        } else if (x1 == 0) {
            num_vertices = vertices_table[x2][y2] - vertices_table[x2][y1 - 1];
            num_vedges = vedges_table[x2][y2] - vedges_table[x2][y1 - 1];
            num_hedges = hedges_table[x2][y2] - hedges_table[x2][y1];
        } else if (y1 == 0) {
            num_vertices = vertices_table[x2][y2] - vertices_table[x1 - 1][y2];
            num_vedges = vedges_table[x2][y2] - vedges_table[x1][y2];
            num_hedges = hedges_table[x2][y2] - hedges_table[x1 - 1][y2];
        } else {
            num_vertices = vertices_table[x2][y2] - vertices_table[x1 - 1][y2] - vertices_table[x2][y1 - 1] + vertices_table[x1 - 1][y1 - 1];
            num_vedges = vedges_table[x2][y2] - vedges_table[x1][y2] - vedges_table[x2][y1 - 1] + vedges_table[x1][y1 - 1];
            num_hedges = hedges_table[x2][y2] - hedges_table[x1 - 1][y2] - hedges_table[x2][y1] + hedges_table[x1 - 1][y1];
        }

        cout << num_vertices - num_hedges - num_vedges << endl;
    }

    return 0;
}
