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

int n;
int dat_odd[2 * (1 << 17) - 1];
int dat_even[2 * (1 << 17) - 1];

void init(int n_) {
    n = 1;
    while (n < n_) {
        n *= 2;
    }

    REP (i, 2 * n - 1) {
        dat_odd[i] = 100000;
        dat_even[i] = 100000;
    }
}


int body(int &argc, char **argv);

// main function (DO NOT EDIT)
int main (int argc, char **argv) {
    cin.tie(0);
    ios_base::sync_with_stdio(false);
    return body(argc, argv);
}

vector<int> ps;

void update(int k, int a) {
    int *dat = nullptr;
    if (k % 2 == 0) {
        dat = dat_even;
        k /= 2;
    } else {
        dat = dat_odd;
        k /= 2;
    }

    k += n - 1;
    dat[k] = a;
    while (k > 0) {
        k = (k - 1) / 2;
        auto n1 = dat[k * 2 + 1];
        auto n2 = dat[k * 2 + 2];
        if (n1 == 100000) {
            dat[k] = n2;
        } else if (n2 == 100000) {
            dat[k] = n1;
        } else {
            if (ps[n1] < ps[n2]) {
                dat[k] = n1;
            } else {
                dat[k] = n2;
            }
        }
    }
}

int query(int * dat,int a, int b, int k, int l, int r) {
    if (r <= a || b <= l) {
        return 100000;
    }

    if (a <= l && r <= b) {
        return dat[k];
    } else {
        int vl = query(dat, a, b, k * 2 + 1, l, (l + r) / 2);
        int vr = query(dat, a, b, k * 2 + 2, (l + r) / 2, r);
        if (vl >= 100000) {
            return vr;
        } else if (vr >= 100000) {
            return vl;
        }
        if (ps[vl] < ps[vr]) {
            return vl;
        } else {
            return vr;
        }
    }
}

// [a, b)の最小値
int query_int(int a, int b, int k, int l, int r) {
    int *dat = nullptr;
    if (a % 2 == 0) {
        dat = dat_even;
    } else {
        dat = dat_odd;
    }
    a /= 2;
    b /= 2;

    return query(dat, a, b, k, l, r);

}
int body(int &argc, char **argv) {
    auto N = read<i32>();
    ps = read<i32>(N);

    init(N / 2);
    REP(i, N) {
        update(i, i);
    }

    using namespace debug;

    auto cmp = [&](auto e1, auto e2) {
        return ps[e1.second] > ps[e2.second];
    };
    priority_queue<pair<pair<i32, i32>, i32>, vector<pair<pair<i32, i32>, i32>>, decltype(cmp)> candidates(cmp);

    auto calc = [&](i32 begin, i32 end) -> i32 {
        return query_int(begin, end, 0, 0, n);
    };

    candidates.push(make_pair(make_pair(0, N), calc(0, N)));

    auto qs = V<i32>();
    while (qs.size() != (size_t)N) {
        auto next = candidates.top();
        candidates.pop();

        auto begin = next.first.first;
        auto end = next.first.second;
        auto first = next.second;
        auto second = query_int(first + 1, end + 1, 0, 0, n);

        qs.push_back(ps[first]);
        qs.push_back(ps[second]);

        if (begin != first) {
            candidates.push(make_pair(make_pair(begin, first), calc(begin, first)));
        }
        if (first + 1 != second) {
            candidates.push(make_pair(make_pair(first + 1, second), calc(first + 1, second)));
        }
        if (second + 1 != end) {
            candidates.push(make_pair(make_pair(second + 1, end), calc(second + 1, end)));
        }
    }

    REP (i, N) {
        cout << qs[i];
        if (i != N - 1) {
            cout << " ";
        }
    }
    cout << endl;

    return 0;
}