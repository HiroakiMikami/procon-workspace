/*
URL https://beta.atcoder.jp/contests/arc063/tasks/arc063_c
SCORE 800
AC true
WA false
TLE false
MLE false
TASK_TYPE グラフ 木 DFS BFS PriorityQueue 判定 割り当て
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
#include <map>
#include <set>
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

#define EACH(x, xs) for (auto &x: (xs))

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

enum class Type { Empty, Odd, Even };
struct Range {
    i64 min;
    i64 max;
    Type t;
};

void body() {
    using namespace debug;
    auto N = read<i64>();
    auto es = read<i64, i64>(N - 1);
    EACH(e, es) {
        e.first -= 1;
        e.second -= 1;
    }
    auto K = read<i64>();
    auto ps = read<i64, i64>(K);
    EACH(p, ps) {
        p.first -= 1;
    }

    auto intersect = [](const Range &r1, const Range &r2) -> Range {
        if (r1.t != r2.t) {
            return {i64(0), i64(0), Type::Empty};
        }

        auto x = std::max(r1.min, r2.min);
        auto y = std::min(r1.max, r2.max);

        if (y < x) {
            return {i64(0), i64(0), Type::Empty};
        }

        return {x, y, r1.t};
    };

    auto root = ps.front().first;

    V<V<i64>> edges(N);
    EACH(e, es) {
        edges[e.first].push_back(e.second);
        edges[e.second].push_back(e.first);
    }

    // rootからの距離を求める（BFS)
    V<i64> distance(N, -1);
    {
        V<bool> visited(N, false);
        queue<pair<size_t, i64>> q;
        q.emplace(root, 0);

        while (!q.empty()) {
            auto x = q.front().first;
            auto d = q.front().second;
            q.pop();

            if (visited[x]) {
                continue ;
            }
            visited[x] = true;

            distance[x] = d;
            EACH (y, edges[x]) {
                q.emplace(y, d + 1);
            }
        }
    }

    // 入力された各頂点が取りうる値の範囲の制約を作る
    V<Range> constraint(N, {numeric_limits<i64>::max(), 0, Type::Empty});
    EACH(c, ps) {
        auto V = c.first;
        auto P = c.second;
        auto t = (P % 2 == 0) ? Type::Even : Type::Odd;
        constraint[V] = {P, P, t};
    }

    // rootから順にたどって取りうる値の制約を決める (DFS)
    {
        stack<pair<size_t, i64>> s;
        s.emplace(root, -1);
        V<bool> visited(N, false);

        while (!s.empty()) {
            auto x = s.top().first;
            auto prev = s.top().second;
            s.pop();

            if (visited[x]) continue;
            visited[x] = true;

            if (prev >= 0) {
                auto r = constraint[prev];
                r.min -= 1;
                r.max += 1;
                if (r.t != Type::Empty) {
                    r.t = (r.t == Type::Odd) ? Type::Even : Type::Odd;
                }
                if (constraint[x].min == numeric_limits<i64>::max()) {
                    constraint[x] = r;
                } else {
                    constraint[x] = intersect(constraint[x], r);
                }
            }

            EACH (y, edges[x]) {
                s.emplace(y, x);
            }
        }
    }

    // rootから離れた側から順にたどって取りうる値の制約を決める（priority queue）
    {
        auto cmp = [](auto x, auto y) { return x.second < y.second; };
        priority_queue<pair<size_t, i64>, std::vector<pair<size_t, i64>>, decltype(cmp)> q(cmp);
        V<bool> visited(N, false);
        EACH (c, ps) {
            auto V = c.first;
            q.emplace(V, distance[V]);
        }

        while (!q.empty()) {
            auto x = q.top().first;
            auto d = q.top().second;
            q.pop();

            if (visited[x]) continue;
            visited[x] = true;

            auto r = constraint[x];
            EACH (y, edges[x]) {
                if (distance[y] < d) {
                    q.emplace(y, distance[y]);
                } else {
                    auto r2 = constraint[y];
                    r2.min -= 1;
                    r2.max += 1;
                    if (r2.t != Type::Empty) {
                        r2.t = (r2.t == Type::Odd) ? Type::Even : Type::Odd;
                    }
                    r = intersect(r, r2);
                }
            }
            constraint[x] = r;
        }
    }

    // rootからDFSしながら実際の値を決める
    V<i64> ans(N, 0);
    bool flag = true;
    {
        stack<pair<size_t, i64>> s;
        V<bool> visited(N, false);
        s.emplace(root, -1);

        while (!s.empty()) {
            auto x = s.top().first;
            auto prev = s.top().second;
            s.pop();

            if (visited[x]) continue ;
            visited[x] = true;

            auto r = constraint[x];
            if (prev >= 0) {
                auto v = ans[prev];
                auto r2 = Range{v-1, v+1, (v % 2 == 0) ? Type::Odd : Type::Even};
                r = intersect(r, r2);
            }

            if (r.t == Type::Empty || r.max < r.min) {
                flag = false;
                break;
            }

            auto v = r.min;
            if (v % 2 == 0 && r.t == Type::Odd) {
                v += 1;
            }
            if (v % 2 == 1 && r.t == Type::Even) {
                v += 1;
            }

            if (v <= r.max) {
                ans[x] = v;
            } else {
                flag = false;
                break;
            }

            EACH(y, edges[x]) {
                s.emplace(y, x);
            }
        }
    }

    cout << (flag ? "Yes" : "No") << endl;
    if (flag) {
        REP (i, N) {
            cout << ans[i] << endl;
        }
    }

}