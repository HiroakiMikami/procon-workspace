/*
URL http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2170
SCORE 0
AC true
WA false
TLE false
MLE false
TASK_TYPE UnionFind グラフ 後ろから状態復元
FAILURE_TYPE 実装ミス
NOTES
*/
#include <iostream>
#include <cstdint>
#include <utility>
#include <tuple>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <limits>
#include <numeric>
#include <cmath>
#include <iomanip>

using namespace std;

// Type aliases
using i8  =  int8_t; using  u8 =  uint8_t;
using i16 = int16_t; using u16 = uint16_t;
using i32 = int32_t; using u32 = uint32_t;
using i64 = int64_t; using u64 = uint64_t;

template <class T> using V = vector<T>;

// Loops
#define REP(i, n) for (i64 i = 0; i < (n); ++i)
#define REPR(i, n) for (i64 i = (n) - 1; i >= 0; --i)
#define FOR(i, n, m) for (i64 i = (n); i < (m); ++i)
#define FORR(i, n, m) for (i64 i = (m) - 1; i >= (n); --i)

#define FORE(x, xs) for (auto &x: (xs))

// Macros
#define CTR(x) (x).begin(), (x).end()

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

    template <class Iterator>
    struct Container {
        Container(const Iterator &begin, const Iterator &end) : begin(begin), end(end) {}
        const Iterator &begin;
        const Iterator &end;
    };

    template <class Iterator>
    Container<Iterator> container(const Iterator &begin, const Iterator &end) {
        return Container<Iterator>(begin, end);
    }

    template <class Iterator>
    ostream &operator<<(ostream &stream, const Container<Iterator> &container) {
        stream << "[";

        size_t cnt = 0;
        for (auto it = container.begin; it != container.end; ++it) {
            stream << *it;
            stream << "," << ((cnt % 10 == 9) ? "\n " : "\t");

            cnt += 1;
        }

        stream << "\b\b]";
        return stream;
    }

    template <class T, class Alloc>
    ostream &operator<<(ostream& stream, const vector<T, Alloc> &vector) {
        return stream << container(vector.begin(), vector.end());
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

#define MAIN
int body(int &argc, char **argv);

// main function (DO NOT EDIT)
int main (int argc, char **argv) {
    cin.tie(0);
    ios_base::sync_with_stdio(false);
    return body(argc, argv);
}

#ifndef MAIN
#include "common.cc"
#endif

struct union_find {
    union_find() : m_parents(0), m_rank(0) {}
    union_find(size_t N) : m_parents(N), m_rank(N) {
        for (auto i = 0; i < N; i++) {
            m_parents[i] = i;
            m_rank[i] = 0;
        }
    }

    void merge(size_t t1, size_t t2) {
        auto p1 = this->parent(t1);
        auto p2 = this->parent(t2);

        if (p1 == p2) {
            return ;
        }

        if (this->m_rank[p1] < this->m_rank[p2]) {
            this->m_parents[p1] = p2;
        } else {
            this->m_parents[p2] = p1;
            if (this->m_rank[p1] == this->m_rank[p2]) {
                this->m_rank[p1] += 1;
            }
        }
    }
    bool is_same(size_t t1, size_t t2) const {
        return this->parent(t1) == this->parent(t2);
    }

    size_t parent(size_t t) const {
        auto p = this->m_parents[t];
        if (p == t) {
            return t;
        } else {
            auto p2 = this->parent(p);
            this->m_parents[t] = p2;
            return p2;
        }
    }
private:
    mutable vector<size_t> m_parents;
    vector<u64> m_rank;

};

int body(int &argc, char **argv) {
    while (true) {
        auto N = read<i32>();
        auto Q = read<i32>();

        if (N == 0 && Q == 0) {
            break ;
        }

        auto edges = V<V<i32>>(N, V<i32>());
        auto parents = V<i32>(N, -1);
        FOR (i, 1, N) {
            parents[i] = read<i32>() - 1;
            edges[parents[i]].push_back(i);
        }

        auto qs = read<char, i32>(Q);
        REP (i, Q) {
            qs[i].second -= 1;
        }

        union_find set(N);

        V<bool> marked(N, false);
        V<i32> marked_node(N, -1);
        marked[0] = true;
        FORE (q, qs) {
            if (q.first == 'M') {
                if (marked[q.second]) {
                    q.second = -1;
                } else {
                    marked[q.second] = true;
                }
            }
        }

        REP (i, N) {
            if (marked[i]) {
                continue ;
            }

            set.merge(parents[i], i);
        }

        REP (i, N) {
            if (marked[i]) {
                marked_node[set.parent(i)] = i;
            }
        }

        u64 ans = 0;
        REPR(i, Q) {
            auto q = qs[i];
            if (q.first == 'M') {
                if (q.second > 0 && !set.is_same(parents[q.second], q.second)) {
                    auto next = marked_node[set.parent(parents[q.second])];
                    set.merge(parents[q.second], q.second);
                    marked_node[set.parent(q.second)] = next;
                }
            } else {
                auto marked = marked_node[set.parent(q.second)];
                ans += marked + 1;
            }
        }

        cout << ans << endl;
    }

    return 0;
}
