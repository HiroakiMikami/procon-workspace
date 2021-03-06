/*
URL https://beta.atcoder.jp/contests/ddcc2017-qual/tasks/ddcc2017_qual_d
SCORE 600
AC true
WA true
TLE false
MLE false
TASK_TYPE 場合分け 数え上げ
FAILURE_TYPE 場合分けミス 考察力不足
NOTES
問題のatomicな要素となるものをみつけたら、それを1単位として問題を再度考え直す。
今回だと、東西南北に対称な4点を1単位とする。
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

int body(int &argc, char **argv) {
    using namespace debug;
    auto H = read<i32>();
    auto W = read<i32>();
    auto A = read<i64>();
    auto B = read<i64>();

    auto m = read<string>(H);

    i64 S = 0;
    REP (i, H) {
        REP (j, W) {
            if (m[i][j] == 'S') {
                S += 1;
            }
        }
    }

    V<i32> num(6, 0);
    REP (i, H / 2) {
        REP (j, W / 2) {
            auto m1 = m[i][j];
            auto m2 = m[H - i - 1][j];
            auto m3 = m[i][W - j - 1];
            auto m4 = m[H - i - 1][W - j - 1];

            if (m1 == m2 && m2 == m3 && m3 == m4) {
                if (m1 == 'S') {
                    /*
                     * oo
                     * oo
                     */
                    num[0] += 1;
                }
            } else {
                auto n = 0;
                n += (m1 == 'S') ? 1 : 0;
                n += (m2 == 'S') ? 1 : 0;
                n += (m3 == 'S') ? 1 : 0;
                n += (m4 == 'S') ? 1 : 0;
                if (n == 1) {
                    /*
                     * ox
                     * xx
                     */
                    num[1] += 1;
                } else if (n == 2) {
                    if ((m1 == 'S' && m4 == 'S') || (m2 == 'S' && m3 == 'S')) {
                        /*
                         * ox
                         * xo
                         */
                        num[2] += 1;
                    } else if ((m1 == 'S' && m3 == 'S') || (m2 == 'S' && m4 == 'S')) {
                        /*
                         * oo
                         * xx
                         */
                        num[3] += 1;
                    } else {
                        /*
                         * ox
                         * ox
                         */
                        num[4] += 1;
                    }
                } else {
                    /*
                     * oo
                     * ox
                     */
                    num[5] += 1;
                }
            }
        }
    }
    // 南北方向から合わせる場合
    i64 c1 = 0;
    /// 1. 南北方向に対称にする
    if (num[1] != 0 || num[2] != 0 || num[3] != 0 || num[5] != 0) {
        c1 += A;
    }
    /// 2. 対称なものを取り除く
    if (num[4] != 0 || num[5] != 0) {
        c1 += (num[4] + num[5]) * A + B;
    } else if (c1 != 0 ){
        // 南北に対称なものがないなら、最初からA+B
        c1 += B;
    }

    // 東西方向から合わせる場合
    i64 c2 = 0;
    /// 1. 東西方向に対称にする
    if (num[1] != 0 || num[2] != 0 || num[4] != 0 || num[5] != 0) {
        c2 += B;
    }
    /// 2. 対称なものを取り除く
    if (num[3] != 0 || num[5] != 0) {
        c2 += (num[3] + num[5]) * B + A;
    } else if (c2 != 0){
        // 東西に対称なものがないなら、最初からA+B
        c2 += A;
    }

    auto ans = max(c1, c2);

    // 4つを取り除く
    ans += num[0] * (max(A, B) + A + B);

    cout << ans << endl;

    return 0;
}