/*
URL https://beta.atcoder.jp/contests/arc062/tasks/arc062_c
SCORE 900
AC true
WA false
TLE false
MLE false
TASK_TYPE 場合の数 数え上げ 順列 前計算 全探索
FAILURE_TYPE 考察速度不足
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
#include <iomanip>
#include <type_traits>

using namespace std;

// Type aliases
using i8  =  int8_t; using  u8 =  uint8_t;
using i16 = int16_t; using u16 = uint16_t;
using i32 = int32_t; using u32 = uint32_t;
using i64 = int64_t; using u64 = uint64_t;

template <class T> using V = vector<T>;

// Loops
#define REP(i, n) for (i64 i = 0; i < static_cast<decltype(i)>(n); ++i)
#define REPR(i, n) for (i64 i = (n) - 1; i >= static_cast<decltype(i)>(0); --i)
#define FOR(i, n, m) for (i64 i = (n); i < static_cast<decltype(i)>(m); ++i)
#define FORR(i, n, m) for (i64 i = (m) - 1; i >= static_cast<decltype(i)>(n); --i)

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

// STL support
template <class Iterator>
struct Container {
    Container(const Iterator &begin, const Iterator &end) : m_begin(begin), m_end(end) {}
    const Iterator& begin() const {
        return this->m_begin;
    }
    const Iterator& end() const {
        return this->m_end;
    }
    Iterator m_begin;
    Iterator m_end;
};

template <class Functions>
struct BaseIterator {
    using State = typename Functions::State;

    BaseIterator(const State &state, const Functions &func) : state(state), func(func) {
        while (!this->is_end() && !this->is_valid()) {
            this->next();
        }
    }
    BaseIterator(const State &state) : state(state), func() {
        while (!this->is_end() && !this->is_valid()) {
            this->next();
        }
    }

    decltype(auto) operator*() {
        return this->func.get_value(this->state);
    }

    decltype(auto) operator*() const {
        return this->func.get_value(this->state);
    }

    BaseIterator &operator++() {
        if (this->is_end()) {
            return *this;
        }

        this->next();
        while (!this->is_end() && !this->is_valid()) {
            this->next();
        }

        return *this;
    }

    BaseIterator &operator--() {
        if (this->is_begin()) {
            return *this;
        }

        this->previous();
        while (!this->is_begin() && !this->is_valid()) {
            this->previous();
        }

        return *this;
    }

    bool operator==(const BaseIterator<Functions> &rhs) const {
        return this->state == rhs.state;
    }
    bool operator!=(const BaseIterator<Functions> &rhs) const {
        return !(*this == rhs);
    }

    bool is_begin() const {
        return this->func.is_begin(this->state);
    }
    bool is_end() const {
        return this->func.is_end(this->state);
    }
    const State &get_state() const {
        return this->state;
    }
private:
    bool is_valid() const {
        return this->func.is_valid(this->state);
    }
    void next() {
        this->func.next(this->state);
    }
    void previous() {
        this->func.previous(this->state);
    }
    State state;
    Functions func;
};

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
    Container<Iterator> container(const Iterator &begin, const Iterator &end) {
        return Container<Iterator>(begin, end);
    }

    template <class Iterator>
    ostream &operator<<(ostream &stream, const Container<Iterator> &container) {
        stream << "[";

        size_t cnt = 0;
        for (const auto &it: container) {
            stream << it;
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
void body();

// main function (DO NOT EDIT)
int main (int argc, char **argv) {
    cin.tie(0);
    ios_base::sync_with_stdio(false);

    cout << fixed;
    body();

    return 0;
}

#include <cmath>

#include <map>
void body() {
    using namespace debug;
    auto N = read<i64>();
    auto Cs = V<V<i64>>(N);
    REP (i, N) {
        Cs[i] = read<i64>(4);
    }

    // 頂点の色情報をu64にエンコード
    auto encode = [&](const V<i64> &ps) {
        V<u64> xs;
        REP (k, 4) {
            u64 id = 0;
            REP (i, 4) {
                id += ps[(i + k) % 4] * std::pow(1000, i);
            }
            xs.push_back(id);
        }
        sort(CTR(xs));
        return xs;
    };
    auto rotate = [&](const V<u64> ids) {
        if (ids.front() == ids.back()) {
            return 4;
        }
        if (ids.front() == ids[1]) {
            return 2;
        }
        return 1;
    };

    // 各カードの色、色ごとの登場回数を前計算
    auto color_ids = V<u64>(N);
    map<u64, i64> colors;
    REP (i, N) {
        auto xs = encode(Cs[i]);
        color_ids[i] = xs.front();

        if (colors.find(xs.front()) == colors.end()) {
            colors.insert({xs.front(), i64(0)});
        }

        colors[xs.front()] += 1;
    }

    u64 ans = 0;
    REP (A, N - 1) { // Aを固定
        FOR(B, A + 1, N) { // Bを固定
            REP (k, 4) { // Bの回転方向を固定
                // 側面の色が決まっているので調査
                auto s1 = encode({Cs[A][1], Cs[B][(0+k)%4], Cs[B][(3+k)%4], Cs[A][2]});
                auto s2 = encode({Cs[A][0], Cs[B][(1+k)%4], Cs[B][(0+k)%4], Cs[A][1]});
                auto s3 = encode({Cs[B][(1+k)%4], Cs[A][0], Cs[A][3], Cs[B][(2+k)%4]});
                auto s4 = encode({Cs[A][3], Cs[A][2], Cs[B][(3+k)%4], Cs[B][(2+k)%4]});

                auto id1 = s1.front();
                auto id2 = s2.front();
                auto id3 = s3.front();
                auto id4 = s4.front();

                unordered_map<u64, size_t> Ns;
                Ns.reserve(4);
                Ns[id1] += 1;
                Ns[id2] += 1;
                Ns[id3] += 1;
                Ns[id4] += 1;

                auto get_num = [&](u64 id) {
                    auto n = colors[id];
                    if (color_ids[A] == id) {
                        n -= 1;
                    }
                    if (color_ids[B] == id) {
                        n -= 1;
                    }
                    return n;
                };

                // 重複で場合分け
                i64 num = 0;
                auto ns = V<pair<u64, size_t>>(CTR(Ns));
                sort(CTR(ns), [](auto &rhs, auto &lhs) { return rhs.second > lhs.second; });
                if (ns.front().second == 4) {
                    // X X X X
                    auto num_X = get_num(ns[0].first);
                    num = num_X * (num_X - 1) * (num_X - 2) * (num_X - 3);
                } else if (ns.front().second == 3) {
                    // X X X Y
                    auto num_X = get_num(ns.front().first);
                    auto num_Y = get_num(ns[1].first);

                    num = num_X * (num_X - 1) * (num_X - 2) * num_Y;
                } else if (ns.front().second == 2 && ns[1].second == 2) {
                    // X X Y Y
                    auto num_X = get_num(ns[0].first);
                    auto num_Y = get_num(ns[1].first);

                    num = num_X * (num_X - 1) * num_Y * (num_Y - 1);
                } else if (ns.front().second == 2 && ns[1].second == 1) {
                    // X X Y Z
                    auto num_X = get_num(ns[0].first);
                    auto num_Y = get_num(ns[1].first);
                    auto num_Z = get_num(ns[2].first);

                    num = num_X * (num_X - 1) * num_Y * num_Z;
                } else {
                    // X Y Z W
                    auto num_X = get_num(ns[0].first);
                    auto num_Y = get_num(ns[1].first);
                    auto num_Z = get_num(ns[2].first);
                    auto num_W = get_num(ns[3].first);

                    num = num_X * num_Y * num_Z * num_W;
                }
                num = std::max(i64(0), num);

                // 回転を考慮
                num *= rotate(s1);
                num *= rotate(s2);
                num *= rotate(s3);
                num *= rotate(s4);

                ans += num;
            }
        }
    }

    // 数え方から、同じ立方体を3回数えている
    // （最初に決めるA,B(=対面の組み合わせ)はA<Bより3パターン）
    cout << (ans / 3) << endl;
}