/*
URL https://beta.atcoder.jp/contests/abc061/tasks/abc061_c
SCORE 300
AC true
WA true
TLE false
MLE false
TASK_TYPE 数列
FAILURE_TYPE オーバーフロー
NOTES
*/
#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include <cstdint>
#include <algorithm>

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
    auto N = read<u32>();
    auto K = read<u64>();
    auto xs = read<u32, u32>(N);

    sort(xs.begin(), xs.end(), [](auto &x1, auto& x2) { return x1.first < x2.first; });

    u64 index = 0;
    FORE(x, xs) {
        auto a_i = x.first;
        auto b_i = x.second;

        if (index < K && K <= index + b_i) {
            cout << a_i << endl;
            break;
        }
        index += b_i;
    }
}

// main function (DO NOT EDIT)
int main () {
    ios_base::sync_with_stdio(false);
    body();
    return 0;
}
