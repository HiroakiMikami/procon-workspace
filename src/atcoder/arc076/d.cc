/*
URL https://beta.atcoder.jp/contests/arc076/tasks/arc076_b
SCORE 500
AC true
WA false
TLE true
MLE false
TASK_TYPE グラフ MST
FAILURE_TYPE 考察力不足
NOTES
*/
#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include <cstdint>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <limits>
#include <queue>

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

// Data Structure
template <class T, class Compare = std::less<T>>
struct heap {
    heap () : m_heap(), m_cmp() {
    }
    heap (const V<T> &v) : m_heap(v), m_cmp() {
        make_heap(this->m_heap.begin(), this->m_heap.end(), this->m_cmp);
    }
    heap (const Compare &cmp) : m_heap(), m_cmp(cmp) {
    }
    heap (const V<T> &v, const Compare &cmp) : m_heap(v), m_cmp(cmp) {
        make_heap(this->m_heap.begin(), this->m_heap.end(), this->m_cmp);
    }

    void reserve(const size_t size) {
        this->m_heap.reserve(size);
    }

    const T &top() const {
        return this->m_heap.front();
    }
    T pop() {
        pop_heap(this->m_heap.begin(), this->m_heap.end(), this->m_cmp);
        auto retval = this->m_heap.back();
        this->m_heap.pop_back();
        return retval;
    }
    void push(const T &v) {
        this->m_heap.push_back(v);
        push_heap(this->m_heap.begin(), this->m_heap.end(), this->m_cmp);
    }

    size_t size() const {
        return this->m_heap.size();
    }

    auto begin() const {
        return this->m_heap.cbegin();
    }
    auto cbegin() const {
        return this->m_heap.cbegin();
    }
    auto end() const {
        return this->m_heap.cend();
    }
    auto cend() const {
        return this->m_heap.cend();
    }
private:
    V<T> m_heap;
    Compare m_cmp;
};

template <class T>
struct binary_indexed_tree {
    binary_indexed_tree(const size_t size) : m_bit(size, 0) {}
    binary_indexed_tree(const vector<T> &vector) : m_bit(vector) {
        REP (i, m_bit.size()) {
            auto j = i + lsb(i + 1);
            if (j < this->m_bit.size()) {
                this->m_bit[j] += m_bit[i];
            }
        }
    }

    void add(size_t index, const T& delta) {
        while (index < this->size()) {
            this->m_bit[index] += delta;
            index += lsb(index + 1);
        }
    }
    auto sum(size_t length) const {
        size_t index = length;
        auto retval = T();
        while (index != 0) {
            retval += this->m_bit[index - 1];
            index -= lsb(index);
        }
        return retval;
    }
    auto sum(size_t begin, size_t end) const {
        // sum between begin ... end-1
        return this->sum(end) - this->sum(begin);
    }
    auto at(size_t index) const {
        return this->sum(index, index + 1);
    }

    size_t size() const {
        return this->m_bit.size();
    }

    auto begin() const {
        return this->m_bit.cbegin();
    }
    auto cbegin() const {
        return this->m_bit.cbegin();
    }
    auto end() const {
        return this->m_bit.cend();
    }
    auto cend() const {
        return this->m_bit.cend();
    }
private:
    static auto lsb(size_t i) {
        return i & -i;
    }
    vector<T> m_bit;
};
template <class T> using BIT = binary_indexed_tree<T>;

template <class T>
struct union_find {
    union_find(const vector<T> &elems) {
        this->m_parents.reserve(elems.size());
        for (const auto &elem: elems) {
            m_parents.insert({elem, elem});
            m_rank.insert({elem, 0});
        }
    }
    template <class Iterator>
    union_find(const Iterator& begin, const Iterator& end) {
        for (auto it = begin; it != end; it++) {
            m_parents.insert({*it, *it});
            m_rank.insert({*it, 0});
        }
    }

    void merge(const T& t1, const T& t2) {
        const auto& p1 = this->parent(t1);
        const auto& p2 = this->parent(t2);

        if (p1 == p2) {
            return ;
        }

        if (this->m_rank[p1] < this->m_rank[p2]) {
            this->m_parents[p1] = p2;
        } else {
            this->m_parents[p2] = p1;
            if (this->m_rank[p1] == this->m_rank[p2]) {
                this->m_rank[p1] += 1;
            }
        }
    }
    bool is_same(const T& t1, const T& t2) const {
        return this->parent(t1) == this->parent(t2);
    }

    const T &parent(const T& t) const {
        const auto& p = this->m_parents[t];
        if (p == t) {
            return t;
        } else {
            const auto& p2 = this->parent(p);
            this->m_parents[t] = p2;
            return p2;
        }
    }
private:
    mutable unordered_map<T, T> m_parents;
    unordered_map<T, u64> m_rank;
};

template <>
struct union_find<size_t> {
    union_find(size_t N) : m_parents(N), m_rank(N) {
        for (auto i = 0; i < N; i++) {
            m_parents[i] = i;
            m_rank[i] = 0;
        }
    }

    void merge(size_t t1, size_t t2) {
        auto p1 = this->parent(t1);
        auto p2 = this->parent(t2);

        if (p1 == p2) {
            return ;
        }

        if (this->m_rank[p1] < this->m_rank[p2]) {
            this->m_parents[p1] = p2;
        } else {
            this->m_parents[p2] = p1;
            if (this->m_rank[p1] == this->m_rank[p2]) {
                this->m_rank[p1] += 1;
            }
        }
    }
    bool is_same(size_t t1, size_t t2) const {
        return this->parent(t1) == this->parent(t2);
    }

    size_t parent(size_t t) const {
        auto p = this->m_parents[t];
        if (p == t) {
            return t;
        } else {
            auto p2 = this->parent(p);
            this->m_parents[t] = p2;
            return p2;
        }
    }
private:
    mutable vector<size_t> m_parents;
    vector<u64> m_rank;

};

int body(int &argc, char **argv);

// main function (DO NOT EDIT)
int main (int argc, char **argv) {
    cin.tie(0);
    ios_base::sync_with_stdio(false);
    return body(argc, argv);
}
struct Edge {
    i32 u;
    i32 v;
    i64 cost;
};
V<Edge> edges;
int body(int &argc, char **argv) {
    auto N = read<i32>();
    auto ps = read<i64, i64>(N);

    auto cost = [&](i32 v, i32 u) {
        auto &p1 = ps[v];
        auto &p2 = ps[u];
        return min(abs(p1.first - p2.first), abs(p1.second - p2.second));
    };
    i64 ans = 0;


#if 1
    V<pair<i32, i64>> xs(N);
    REP (i, N) { xs[i] = { i, ps[i].first };}
    V<pair<i32, i64>> ys(N);
    REP (i, N) { ys[i] = { i, ps[i].second };}
    sort(xs.begin(), xs.end(), [](pair<i32, i64> p1, pair<i32, i64> p2) { return p1.second < p2.second; });
    sort(ys.begin(), ys.end(), [](pair<i32, i64> p1, pair<i32, i64> p2) { return p1.second < p2.second; });

    REP (i, N) {
        if (i != 0) {
            edges.push_back({xs[i].first, xs[i - 1].first, cost(xs[i].first, xs[i - 1].first)});
            edges.push_back({ys[i].first, ys[i - 1].first, cost(ys[i].first, ys[i - 1].first)});
        }
        if (i != (N - 1)) {
            edges.push_back({xs[i].first, xs[i + 1].first, cost(xs[i].first, xs[i + 1].first)});
            edges.push_back({ys[i].first, ys[i + 1].first, cost(ys[i].first, ys[i + 1].first)});
        }
    }

    sort(edges.begin(), edges.end(), [](Edge e1, Edge e2) -> bool { return e1.cost < e2.cost; });
    union_find<size_t> vs(N);

    REP (i, edges.size()) {
        auto e = edges[i];
        if (!vs.is_same(e.u, e.v)) {
            vs.merge(e.u, e.v);
            ans += e.cost;
        }
    }
#else
    auto mincost = V<i64>(N, numeric_limits<i64>::max());
    auto used = V<bool>(N, false);

    mincost[0] = 0;

    while (true) {
        i32 v = -1;

        REP(i, N) {
            if (!used[i] && (v == -1 || mincost[i] < mincost[v])) {
                v = i;
            }
        }
        if (v == -1) {
            break;
        }
        used[v] = true;
        ans += mincost[v];
        REP (i, N) {
            mincost[i] = min(mincost[i], cost(v, i));
        }
    }
#endif

    cout << ans << endl;

    return 0;
}
