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
    auto s = read<string>();


    i64 ans = 0;
    i32 b = 0;
    i32 l = s.size() - 1;
    while (b < l) {
        auto s1 = s[b];
        auto s2 = s[l];
        if (s1 == s2) {
            b += 1;
            l -= 1;
        } else if (s1 == 'x') {
            b += 1;
            ans += 1;
        } else if (s2 == 'x') {
            l -= 1;
            ans += 1;
        } else {
            cout << -1 << endl;
            return 0;
        }
    }
    cout << ans << endl;

#if 0
    V<i32> os(26);

    REP (i, s.size()) {
        if (s[i] != 'x') {
            os[s[i] - 'a'] += 1;
        }
    }

    i32 center = -1;
    REP(i, 26) {
        auto c = os[i];
        if (c % 2 == 1) {
            if (center != -1) {
                cout << -1 << endl;
                return 0;
            }
            center = i;
        }
    }

    if (center == -1) {
        // (tested)
        i64 ans = 0;
        i32 b = 0;
        i32 l = s.size() - 1;
        while (b < l) {
            auto s1 = s[b];
            auto s2 = s[l];
            if (s1 == s2) {
                b += 1;
                l -= 1;
            } else if (s1 == 'x') {
                b += 1;
                ans += 1;
            } else if (s2 == 'x') {
                l -= 1;
                ans += 1;
            } else {
                cout << -1 << endl;
                return 0;
            }
        }
        cout << ans << endl;
    } else {
        i32 index = 0;
        i32 cnt = 0;

        // 中央とする場所
        REP (i, s.size()) {
            if (s[i] - 'a' == center) {
                if (os[center] / 2 == cnt) {
                    index = i;
                    break;
                }
                cnt += 1;
            }
        }

        auto N = max<i32>(index, s.size() - index - 1);
        i64 ans = 0;
        auto b = 0;
        auto l = s.size() - 1;
        while (b < l) {
            auto s1 = s[b];
            auto s2 = s[l];

            if (s1 == s2) {

            } else if (s1 == 'x') {

            } else if (s2 == 'x') {

            } else {
                cout << -1 << endl;
                return 0;
            }
        }
        FOR(i, 1, N + 1) {
            // s[index - i]とs[index + i]
            auto x = index - i;
            auto y = index + i;
            cout << x << " " << y << endl;

            if (x < 0 && y >= s.size()) {
                // ありえないはず
            } else if (x < 0) {
                // s[y]がxの時のみ達成可能
                if (s[y] == 'x') {
                    ans += 1;
                } else {
                    cout << -1 << endl;
                    return 0;
                }
            } else if (y >= s.size()) {
                // s[x]がxの時のみ達成可能
                if (s[x] == 'x') {
                    ans += 1;
                } else {
                    cout << -1 << endl;
                    return 0;
                }
            } else {
                auto s1 = s[x];
                auto s2 = s[y];
                if (s1 == s2) {

                } else if (s1 == 'x' || s2 == 'x') {
                    ans += 1;
                } else {
                    cout << -1 << endl;
                    return 0;
                }
            }
        }
        cout << ans << endl;
    }
#endif

    return 0;
}