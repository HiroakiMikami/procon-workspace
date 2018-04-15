/*
URL https://beta.atcoder.jp/contests/arc073/tasks/arc073_b
SCORE 400
AC true
WA false
TLE false
MLE false
TASK_TYPE 全探索 ナップザック
FAILURE_TYPE 考察力不足
NOTES
重さが4パターンに絞れる制約には気づいたが、そこからどうすればよいのかわからなかった。
それぞれの重さにつき個数を決めて全探索していくのが厳しいように思っていたが、たしかに4重forループで良い
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
    using namespace debug;

    auto N = read<u16>();
    auto W = read<i64>();
    auto ts = read<i64, i32>(N);

    auto w_1 = ts[0].first;
    auto vs = V<V<i32>>(4);
    FORE (v, vs) {
        v.reserve(N);
    }
    FORE (t, ts) {
        auto w_i = t.first;
        auto v_i = t.second;
        vs[w_i - w_1].push_back(v_i);
    }
    FORE (v, vs) {
        sort(v.begin(), v.end(), greater<i32>());
    }
    i64 max = 0;
    REP (i_0, vs[0].size() + 1) {
        REP (i_1, vs[1].size() + 1) {
            REP (i_2, vs[2].size() + 1) {
                REP (i_3, vs[3].size() + 1) {
                    i64 weight = 0;
                    i64 tmp = 0;
                    REP(i, i_0) {
                        weight += w_1;
                        tmp += vs[0][i];
                    }
                    REP(i, i_1) {
                        weight += w_1 + 1;
                        tmp += vs[1][i];
                    }
                    REP(i, i_2) {
                        weight += w_1 + 2;
                        tmp += vs[2][i];
                    }
                    REP(i, i_3) {
                        weight += w_1 + 3;
                        tmp += vs[3][i];
                    }
                    if (weight <= W) {
                        max = std::max(max, tmp);
                    }
                }
            }
        }
    }

    cout << max << endl;
}

// main function (DO NOT EDIT)
int main () {
    ios_base::sync_with_stdio(false);
    body();
    return 0;
}