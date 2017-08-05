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

int body(int &argc, char **argv);

// main function (DO NOT EDIT)
int main (int argc, char **argv) {
    cin.tie(0);
    ios_base::sync_with_stdio(false);
    return body(argc, argv);
}

void find_cycle(const V<V<i64>> &edges, V<bool> &is_visited, V<i64> &path) {
    is_visited[path.back()] = true;

    FORE (n, edges[path.back()]) {
        path.push_back(n);
        if (path.front() == path.back()) {
            return ;
        }
        if (is_visited[path.back()]) continue ;
        find_cycle(edges, is_visited, path);
        if (path.size() > 1 && path.front() == path.back()) {
            return ;
        }
        path.pop_back();
    }
}


int body(int &argc, char **argv) {
    using namespace debug;
    auto N = read<i64>();
    auto ps = read<i64>(N);
    FORE (p, ps) {
        p -= 1;
    }

    auto edges = V<V<i64>>(N, V<i64>());
    auto edges2 = V<i64>(N);
    REP (i, N) {
        // ps[i] -> i
        auto n1 = ps[i];
        i64 n2 = i;
        edges[n1].push_back(n2);
        edges2[n2] = n1;
    }

    // Find cycle
    V<i64> cycle;
    auto s = stack<V<i64>>();
    V<bool> is_visited(N);
    REP (i, N) {
        if (is_visited[i]) continue ;
        cycle = {i};
        find_cycle(edges, is_visited, cycle);

        if (cycle.size() > 1 && cycle.front() == cycle.back()) {
            break ;
        }
    }

    V<i64> values(N, -1);
    auto fill_value = [&](i64 n, auto f) -> i64 {
        if (values[n] != -1) {
            return values[n];
        }

        unordered_set<i64> xs;
        FORE (n2, edges[n]) {
            xs.insert(f(n2, f));
        }

        REP(i, xs.size() + 1) {
            if (xs.find(i) == xs.end()) {
                values[n] = i;
                return i;
            }
        }
    };

    unordered_set<i64> in_cycles(cycle.begin(), cycle.end());
    REP (i, N) {
        if (in_cycles.find(i) != in_cycles.end()) {
            continue;
        }

        fill_value(i, fill_value);
    }

    bool possible = false;
    auto start = cycle.back();
    unordered_set<i64> start_edges;
    i64 v1 = 0;
    i64 cnt = 0;
    FORE(e, edges[start]) {
        if (values[e] == -1) {
            cnt += 1;
        }
        v1 = max(v1, values[e]);
        start_edges.insert(values[e]);
    }
    auto max_candidate = v1 + 2 + cnt;
    REP (x, max_candidate) {
        if (start_edges.find(x) != start_edges.end()) {
            continue ;
        }

        auto vs = values;
        vs[start] = x;
        FORR (i, 1, cycle.size() - 1) {
            auto n = cycle[i];
            unordered_set<i64> xs;
            FORE (n2, edges[n]) {
                xs.insert(vs[n2]);
            }

            REP(j, xs.size() + 1) {
                if (xs.find(j) == xs.end()) {
                    vs[n] = j;
                    break ;
                }
            }
        }

        auto n = cycle[0];
        auto v = vs[n];
        V<bool> exists(v);
        bool ok = true;
        FORE (n2, edges[n]) {
            auto v2 = vs[n2];
            if (v2 == v) {
                ok = false;
                break;
            }
            if (v2 < v) {
                exists[v2] = true;
            }
        }
        REP (i, v) {
            auto f = exists[i];
            if (!f) {
                ok = false;
                break;
            }
        }

        if (ok) {
            possible = true;
            break;
        }
    }

    cout << (possible ? "POSSIBLE": "IMPOSSIBLE") << endl;

    return 0;
}