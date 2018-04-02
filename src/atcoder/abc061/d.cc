/*
URL https://beta.atcoder.jp/contests/abc061/tasks/abc061_d
SCORE 400
AC true
WA true
TLE false
MLE false
TASK_TYPE グラフ 最短経路 ベルマンフォード
FAILURE_TYPE オーバーフロー 場合分けミス
NOTES
*/
#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include <cstdint>

using namespace std;

// Type aliases
using i8  =  int8_t; using  u8 =  uint8_t;
using i16 = int16_t; using u16 = uint16_t;
using i32 = int32_t; using u32 = uint32_t;
using i64 = int64_t; using u64 = uint64_t;

template <class T> using V = vector<T>;

// Loops
#define REP(i, n) for (int i = 0; i < n; ++i)
#define REPR(i, n) for (int i = n - 1; i >= 0; --i)
#define FOR(i, n, m) for (int i = n; i < m; ++i)
#define FORR(i, n, m) for (int i = m - 1; i >= n; --i)

#define FORE(x, xs) for (auto &x: xs)

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

        for (auto i = 0; i < vector.size(); i++) {
            stream << vector[i];

            if (i != vector.size() - 1) {
                stream << "," << ((i % 10 == 9) ? "\n " : "\t");
            }
        }

        stream << "]";
        return stream;
    }
}

// Body
void body() {
    auto N = read<u16>();
    auto M = read<u16>();
    auto edges = read<u16, u16, i64>(M);
    FORE(edge, edges) {
        get<0>(edge) -= 1;
        get<1>(edge) -= 1;
        get<2>(edge) *= -1;
    }

    // Bellman-Ford
    auto vertices = V<i64>(N, 999999999999999);
    vertices[0] = 0;

    REP(i, N - 1) {
        for (auto &edge: edges) {
            auto u = get<0>(edge);
            auto v = get<1>(edge);
            if (vertices[v] > vertices[u] + get<2>(edge)) {
                vertices[v] = vertices[u] + get<2>(edge);
            }
        }
    }

    auto is_inf = V<bool>(N, false);

    for (auto &edge: edges) {
        auto u = get<0>(edge);
        auto v = get<1>(edge);
        if (is_inf[u] || vertices[u] + get<2>(edge) < vertices[v])  {
            is_inf[v] = true;
        }
    }

    using namespace debug;
    
    if (is_inf[N - 1]) {
        cout << "inf" << endl;
    } else {
        cout << -vertices[N-1] << endl;
    }
}

// main function (DO NOT EDIT)
int main () {
    ios_base::sync_with_stdio(false);
    body();
    return 0;
}
