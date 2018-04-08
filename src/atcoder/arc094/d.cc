/*
URL https://beta.atcoder.jp/contests/arc094/tasks/arc094_b
SCORE 700
AC true
WA false
TLE false
MLE false
TASK_TYPE 場合分け 数え上げ
FAILURE_TYPE 場合分けミス 考察力不足
NOTES
解説を見ると、A=B、B=A+1の時のようなかんたんな場合から順を追って考えている。
具体的な数字で考えるだけでなく、条件をきつくする（適当に等号を入れるなど）してその条件での最適を考えると場合分けはうまく行きやすい気がする
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

void body() {
    using namespace debug;
    auto Q = read<i64>();
    auto qs = read<i64, i64>(Q);

    FORE (q, qs) {
        auto A = q.first;
        auto B = q.second;

        if (B > A) {
            swap(A, B);
        }

        i64 ans = 0;

        auto C = static_cast<i64>(sqrt(A * B));
        if (A == B) {
            /*
             * どちらか一方は、A以下でないといけないので、
             * (A-1, A+1), (A-2, A+2), ..., (1, 2A-1)
             * の組でスコアが小さくなる
             * 1stがA-k, 2ndがA+kの人と、逆の人が存在できるので、各組に対しスコアが小さい人は2人ずつ
             * 上記割当で、
             *   1) A-1より小さい順位を取ったことのある人すべてが(A，B)よりスコアが小さくなる
             *   2) A-1より小さい順位を取ったことがある人がすべて異なる
             * ので、これが最大
             */
            ans = 2 * (A - 1);
        } else if (C * C == A * B) {
            /*
             * どちらか一方は、C未満でないといけないので、
             * (C-1, C+1), (C-2, C+2), ..., (1, 2C-1)
             * の組でスコアが小さくなる。
             * A==Bの時と同様に、基本的に各々2人ずつ存在する。
             *
             * A!=Bより、A < Cなので、(A, 2C-A)という組が存在して、この時本人のスコアと競合するので1人しか対応しない
             * また、B<=2C-1の時は、(B, 2C-B)という組が存在して、この時本人のスコアと競合するので1人しか対応しない
             *
             * [C, 2C-A]、[C, 2C-B]が使える数字で割当が決まっていないものであるが、
             * (C, C), (2C-A, 2C-B)はスコアがABとなり条件を満たさない
             * (C, 2C-A)は、2C-A>C (A<Cより)なので、AB以上となる
             * (C, 2C-B)は、C < BよりAB未満となるので、この割当は条件を満たす。
             *
             * 結局、B<=2C-1のとき、割当から1つ外れて別の割当ができるので、B<=2C-1の条件は考えなくて良い
             */
            ans = 2 * (C - 1) - 1;
        } else {
            if (C * (C + 1) < A * B) {
                /*
                 * どちらか一方はC以下でないといけなく、(C, C+1)が(A, B)よりスコアが小さいので、
                 * (C, C+1), (C-1, C+2), (C-3, C+3), ..., (1, 2C)
                 * の組でスコアが小さくなる。
                 * A==Bの時と同様に、基本的に各々2人ずつ存在する。
                 *
                 * A < C, C < Bなので、(A, 2C-A+1)という組が存在して、1人しか対応しない
                 * また、B<=2Cのときは、(B, 2C-B+1)という組が存在して、1人しか対応しない
                 *
                 * B <= 2Cの時、(2C-A+1, 2C-B+1)という割当がありうる。これが条件を満たすかは計算すればわかる
                 * B > 2Cの時、2C-A+1に対し、C以下の数字が存在しないので、割り当てられない
                 */
                if (B <= 2 * C) {
                    ans = 2 * C - 2; // (C-k, C+k+1), (A, 2C-A+1), (B, 2C-B+1)の分を引く
                    if ((2 * C - B + 1) * (2 * C - A + 1) < A * B) {
                        ans += 1; //(2C-B+1,2C-A+1)
                    }
                } else {
                    ans = 2 * C - 1; // (C-k, C+k), (A, 2C-A+1)のぶんを引く
                }
            } else {
                /*
                 * どちらか一方はC以下でないといけないので、
                 * (C, C), (C-1, C+1), (C-2, C+2), ..., (1, 2C-1)
                 * の組でスコアが小さくなる。
                 * A==Bの時と同様に、基本的に各々2人ずつ存在する((C, C)は当然1人)。
                 *
                 * A < C, C < Bなので、(A, 2C-A)という組が存在して、1人しか対応しない
                 * また、B<=2C-1のときは、(B, 2C-B)という組が存在して、1人しか対応しない
                 *
                 * B <= 2C-1の時、(2C-A, 2C-B)という割当がありうる。これが条件を満たすかは計算すればわかる
                 * B > 2C-1の時、2C-Aに対し、C以下の数字が存在しないので、割り当てられない
                 */
                if (B <= 2 * C - 1) {
                    ans = 2 * (C - 1) - 2; // (C-k, C+k), (A, 2C-A), (B, 2C-B)の分を引く
                    ans += 1; // (C, C)
                    if ((2 * C - B) * (2 * C - A) < A * B) {
                        ans += 1; //(2C-B,2C-A)
                    }
                } else {
                    ans = 2 * (C - 1) - 1; // (C-k, C+k), (A, 2C-A)のぶんを引く
                    ans += 1; // (C, C)
                }
            }
        }
        cout << ans << endl;
    }
}