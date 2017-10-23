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

#ifndef MAIN
#include "common.cc"
#endif

namespace math {
    /*
     * n^r
     */
    template <class V>
    static V pow(V n, i64 r) {
        if (r == 0) {
            return 1;
        }

        auto r2 = r / 2;
        auto x2 = pow(n, r2);

        return x2 * x2 * ((r % 2 == 0) ? 1 : n);
    }

    static i64 gcd(i64 a, i64 b) {
        if (b == 0) return a;
        return gcd(b, a % b);
    }

    template <class Iterator>
    static i64 gcd_ctr(const Iterator& begin, const Iterator &end) {
        if (begin == end) {
            return -1;
        } else {
            auto ans = *begin;
            auto it = begin;
            ++it;
            for (; it != end; ++it) {
                auto x = *it;
                ans = gcd(ans, x);
            }
            return ans;
        }
    }

    static i64 gcd_ctr(const V<i64> &xs) {
        return gcd_ctr(xs.begin(), xs.end());
    }

    static i64 lcm(i64 a, i64 b) {
        auto x = gcd(a, b);
        return a / x * b;
    }

    template <class Iterator>
    static i64 lcm_ctr(const Iterator &begin, const Iterator &end) {
        if (begin == end) {
            return -1;
        } else {
            auto ans = *begin;
            auto it = begin;
            ++it;
            for (; it != end; ++it) {
                auto x = *it;
                ans = lcm(ans, x);
            }
            return ans;
        }
    }
    static i64 lcm_ctr(const V<i64> &xs) {
        return lcm_ctr(xs.begin(), xs.end());
    }

    template <class V>
    static V combination(V n, i64 r) {
        if (r == 0) {
            return 1;
        }

        V  x = 1;
        FOR (d, 1, r + 1) {
            x *= n;
            n -= 1;
            x /= d;
        }
        return x;
    }

    static bool is_prime(i64 n) {
        if (n <= 1) return false;
        for (i64 i = 2; i * i <= n; i++) {
            if (n % i == 0) return false;
        }
        return true;
    }
    static V<i64> divisor(i64 n) {
        V<i64> res;
        for (i64 i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                res.push_back(i);
                if (i != n / i) res.push_back(n / i);
            }
        }
        return res;
    }
    static unordered_map<i64, size_t> prime_factor(i64 n) {
        unordered_map<i64, size_t> res;
        for (i64 i = 2; i * i <= n; i++) {
            while (n % i == 0) {
                res[i] += 1;
                n /= i;
            }
        }
        if (n != 1) res[n] = 1;
        return res;
    }

    static pair<V<i64>, V<bool>> sieve(i64 n) {
        V<i64> prime;
        V<bool> is_prime_(n + 1, true);
        is_prime_[0] = is_prime_[1] = false;

        FOR (i, 2, n + 1) {
            if (is_prime_[i]) {
                prime.push_back(i);
                for (i64 j = 2 * i; j <= n; j += i) {
                    is_prime_[j] = false;
                }
            }
        }
        return { prime, is_prime_ };
    }
}

int body(int &argc, char **argv) {
    using namespace debug;
    auto N = read<i32>();
    auto As = read<i32>(N);

    V<i64> dp(N, 0); // dp[i] a[i:]について

    dp[N - 1] = (As[N - 1] % 2 == 0) ? 1 : 2;
    REPR(i, N - 1) {
        auto x = math::pow(3, N - 1 - i);
        if (As[i] % 2 == 0) {
            dp[i] = x + 2 * dp[i + 1];
        } else {
            dp[i] = 2 * x + dp[i + 1];
        }
    }

    cout << dp[0] << endl;

    return 0;
}