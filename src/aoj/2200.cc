/*
URL http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2170
SCORE 0
AC true
WA false
TLE false
MLE false
TASK_TYPE グラフ 最短経路 ワーシャルフロイド DP
FAILURE_TYPE 考察不足
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

        auto es = read<i32, i32, u64, char>(M);
        auto R = read<i32>();
        auto zs = read<i32>(R);
        FORE (z, zs) {
            z -= 1;
        }

        auto edges_l = V<V<pair<i32, u64>>>(N);
        auto edges_s = V<V<pair<i32, u64>>>(N);

        FORE (e, es) {
            auto a = get<0>(e) - 1;
            auto b = get<1>(e) - 1;
            auto d = get<2>(e);
            auto type = get<3>(e);

            if (type == 'S') {
                edges_s[a].push_back(make_pair(b, d));
                edges_s[b].push_back(make_pair(a, d));
            } else {
                edges_l[a].push_back(make_pair(b, d));
                edges_l[b].push_back(make_pair(a, d));
            }
        }

        // Warshall Floyd
        V<V<u64>> distance_l(N, V<u64>(N, (u64)1e10));
        V<V<u64>> distance_s(N, V<u64>(N, (u64)1e10));
        REP (k, N) {
            distance_s[k][k] = 0;
            FORE (e, edges_s[k]) {
                distance_s[k][e.first] = e.second;
            }

            distance_l[k][k] = 0;
            FORE (e, edges_l[k]) {
                distance_l[k][e.first] = e.second;
            }
        }
        REP (k, N) {
            REP (i, N) {
                REP (j, N) {
                    distance_l[i][j] = min(distance_l[i][j], distance_l[i][k] + distance_l[k][j]);
                    distance_s[i][j] = min(distance_s[i][j], distance_s[i][k] + distance_s[k][j]);
                }
            }
        }

        // DP
        auto dp = V<V<u64>>(R, V<u64>(N)); // dp[i][j] = z_iまでいって、船がjにあるときの最短コスト

        // Initialize
        REP (i, R) {
            REP (j, N) {
                dp[i][j] = 1e10;
            }
        }
        dp[0][zs[0]] = 0;

        FOR (i, 1, R) {
            auto z1 = zs[i - 1];
            auto z2 = zs[i];

            REP (s1, N) {
                REP (s2, N) {
                    if (s1 == s2) {
                        // 陸路
                        auto d = distance_l[z1][z2];
                        dp[i][s2] = min(dp[i][s2], min((u64)1e10, (dp[i - 1][s1] + d)));
                    } else {
                        // 陸路->海路->陸路
                        auto d1 = distance_l[z1][s1];
                        auto d2 = distance_l[s2][z2];
                        auto d3 = distance_s[s1][s2];
                        auto d = d1 + d2 + d3;
                        dp[i][s2] = min(dp[i][s2], min((u64)1e10, (dp[i - 1][s1] + d)));
                    }
                }
            }
        }

        cout << *min_element(CTR(dp[R - 1])) << endl;
    }

    return 0;
}