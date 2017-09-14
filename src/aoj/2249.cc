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

u64 dfs(const V<V<pair<i32, u64>>> &edges, const V<V<u64>> &distances, i32 node, unordered_set<i32> &remained) {
    u64 dist = 1e10;

    if (remained.empty()) {
        return 0;
    }

    FORE (r, remained) {
        auto set = remained;
        set.erase(r);
        auto d = dfs(edges, distances, r, set);
        if (node != -1) {
            d += distances[node][r];
        }
        dist = min(dist, d);
    }
    return dist;
}

int body(int &argc, char **argv) {
    using namespace debug;

    while (true) {
        auto N = read<i32>();
        auto M = read<i32>();

        if (N == 0 && M == 0) {
            break;
        }

        auto es = read<i32, i32, u64, u64>(M);
        FORE(e, es) {
            get<0>(e) -= 1;
            get<1>(e) -= 1;
        }

        using Label = pair<u64, u64>;

        auto edges = V<V<pair<i32, Label>>>(N);
        FORE(e, es) {
            auto a = get<0>(e);
            auto b = get<1>(e);
            auto d = get<2>(e);
            auto c = get<3>(e);

            edges[a].push_back(make_pair(b, make_pair(d, c)));
            edges[b].push_back(make_pair(a, make_pair(d, c)));
        }


        auto distance = V<u64>(N);
        auto prev = V<pair<i32, u64>>(N);
        auto used_vertex = V<bool>(N, false);
        auto cmp = [&](auto v1, auto v2) {
            return v1.second > v2.second;
        };
        auto Q = priority_queue<pair<i32, u64>, vector<pair<i32, u64>>, decltype(cmp)>(cmp);

        REP (i, N) {
            if (i == 0) {
                distance[i] = 0;
            } else {
                distance[i] = numeric_limits<u64>::max();
            }

            prev[i] = { -1, 0 };
            Q.push(make_pair(i, distance[i]));
        }

        while (!Q.empty()) {
            auto u = Q.top().first;
            Q.pop();

            if (used_vertex[u]) {
                continue ;
            }

            used_vertex[u] = true;

            FORE (e, edges[u]) {
                auto v = e.first;
                auto label = e.second;
                auto d = label.first;
                auto c = label.second;

                if (distance[v] > distance[u] + d) {
                    distance[v] = distance[u] + d;
                    prev[v] = make_pair(u, c);
                    Q.push(make_pair(v, distance[v]));
                } else if (distance[v] == distance[u] + d) {
                    if (prev[v].first != -1) {
                        auto c2 = prev[v].second;
                        if (c2 > c) {
                            prev[v] = make_pair(u, c);
                        }
                    } else {
                        prev[v] = make_pair(u, c);
                    }
                }
            }
        }

        u64 ans = 0;
        unordered_set<pair<i32, i32>> used;
        used.reserve(M);
        FOR (i, 1, N) {
            i32 v1 = i;
            i32 v2 = prev[v1].first;
            auto edge = make_pair(min(v1, v2), max(v1, v2));
            if (used.find(edge) == used.end()) {
                used.insert(edge);
                ans += prev[v1].second;
            }
        }

        cout << ans << endl;
    }

    return 0;
}