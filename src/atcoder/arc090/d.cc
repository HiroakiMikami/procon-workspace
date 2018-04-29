/*
URL https://beta.atcoder.jp/contests/arc090/tasks/arc090_b
SCORE 400
AC true
WA true
TLE false
MLE false
TASK_TYPE 判定 グラフ DFS
FAILURE_TYPE コーナーケース処理
NOTES
全条件を調べていく話なのでグラフなんて作らなくてもできそうな気が細書した。
ただ、連結な成分のどこから数字を決めていくかを考えるにはグラフとして扱ったほうが都合が良い。
コーナーケース処理は、グラフの連結性の判定を忘れていた。
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

#include <vector>
#include <unordered_map>
#include <type_traits>
#include <experimental/optional>

#ifndef MAIN
#include "common.cc"
#endif

template <class EdgeLabel>
using Edge = typename std::conditional<std::is_void<EdgeLabel>::value, tuple<size_t, size_t>, tuple<size_t, size_t, EdgeLabel>>::type;

template <
        class _EdgeLabel,
        class EntryList = vector<typename conditional<is_void<_EdgeLabel>::value, tuple<size_t>, tuple<size_t, _EdgeLabel>>::type>,
        class List = vector<EntryList>>
struct AdjacencyList {
    using EdgeLabel = _EdgeLabel;

    struct EdgeIteratorFunctions {
        EdgeIteratorFunctions(size_t to): to(to) {}
        EdgeIteratorFunctions(): to() {}
        using State = std::tuple<size_t, size_t, const AdjacencyList *>;
        bool is_begin(const State &state) const {
            return get<0>(state) == 0 && get<1>(state) == 0;
        }
        bool is_end(const State &state) const {
            return get<0>(state) == get<2>(state)->m_list.size();
        }
        bool is_valid(const State &state) const {
            auto i = get<0>(state);
            auto j = get<1>(state);

            if (get<2>(state)->m_list.size() == i) {
                return true;
            }
            if (get<2>(state)->m_list[i].size() == j) {
                return false;
            }

            if (this->to) {
                return get<0>(get<2>(state)->m_list[i][j]) == this->to.value();
            } else {
                return true;
            }
        }
        Edge<EdgeLabel> get_value(const State &state) const {
            return to_edge(get<0>(state), get<2>(state)->m_list[get<0>(state)][get<1>(state)]);
        }

        Edge<EdgeLabel> get_value(State &state) {
            return to_edge(get<0>(state), get<2>(state)->m_list[get<0>(state)][get<1>(state)]);
        }

        void next(State &state) const {
            if ((get<2>(state)->m_list[get<0>(state)].size() == 0) ||
                (get<1>(state) + 1) == get<2>(state)->m_list[get<0>(state)].size()) {
                get<0>(state) += 1;
                get<1>(state) = 0;
            } else {
                get<1>(state) += 1;
            }
        }
        void previous(State &state) const {
            if (get<1>(state) == 0) {
                get<0>(state) -= 1;
                get<1>(state) = get<2>(state)->m_list[get<0>(state)].size() - 1;
            } else {
                get<1>(state) -= 1;
            }
        }
    private:
        std::experimental::optional<size_t> to;
    };
    using EdgeIterator = BaseIterator<EdgeIteratorFunctions>;

    AdjacencyList() {}
    AdjacencyList(size_t vertex_num) : m_list(vertex_num) {}

    const size_t vertices_size() const {
        return this->m_list.size();
    }

    Container<EdgeIterator> edges() const {
        auto begin = EdgeIterator(std::make_tuple(0, static_cast<size_t>(0), this));
        auto end = EdgeIterator(std::make_tuple(this->m_list.size(), static_cast<size_t>(0), this));
        return Container<EdgeIterator>(begin, end);
    }
    Container<EdgeIterator> edges(size_t n1, size_t n2) const {
        auto begin = EdgeIterator(std::make_tuple(n1, static_cast<size_t>(0), this), EdgeIteratorFunctions(n2));
        auto end = EdgeIterator(std::make_tuple(n1 + 1, static_cast<size_t>(0), this), EdgeIteratorFunctions(n2));
        return Container<EdgeIterator>(begin, end);
    }

    Container<EdgeIterator> outgoings(size_t n) const {
        auto begin = EdgeIterator(std::make_tuple(n, static_cast<size_t>(0), this));
        auto end = EdgeIterator(std::make_tuple(n + 1, static_cast<size_t>(0), this));
        return Container<EdgeIterator>(begin, end);
    }

    auto has_edge(size_t n1, size_t n2) const {
        return find_if(
                this->m_list[n1].begin(), this->m_list[n1].end(),
                [n2](const auto &edge) { return get<0>(edge) == n2; }
        ) != this->m_list[n1].end();
    }

    void add_edge(const Edge<EdgeLabel>& edge) {
        this->m_list[get<0>(edge)].push_back(to_entry<EdgeLabel>(edge));
    }
    void remove_edge(const Edge<EdgeLabel>& edge) {
        this->m_list[get<0>(edge)].erase(
                remove(this->m_list[get<0>(edge)].begin(), this->m_list[get<0>(edge)].end(), to_entry<EdgeLabel>(edge)),
                this->m_list[get<0>(edge)].end()
        );
    }
    void remove_edge(size_t n1, size_t n2) {
        this->m_list[n1].erase(
                remove_if(
                        this->m_list[n1].begin(), this->m_list[n1].end(),
                        [n2](const auto &edge) { return get<0>(edge) == n2; }
                ),
                this->m_list[n1].end()
        );
    }
    size_t add_vertex() {
        this->m_list.push_back({});
        return this->m_list.size() - 1;
    }
    void remove_vertex(size_t n) {
        for (auto i = 0; i < this->m_list.size(); ++i) {
            if (i == n) {
                this->m_list[i].clear();
            } else {
                auto &edges = this->m_list[i];
                edges.erase(
                        remove_if(edges.begin(), edges.end(), [n](const auto &edge) { return get<0>(edge) == n; }),
                        edges.end()
                );
            }
        }
    }

    void to_undirected() {
        std::vector<Edge<EdgeLabel>> es;
        for (auto edge: this->edges()) {
            std::swap(get<0>(edge), get<1>(edge));
            es.push_back(edge);
        }

        FORE (edge, es) {
            this->add_edge(edge);
        }
    }
private:
    List m_list;

    using EdgeEntry = typename conditional<is_void<EdgeLabel>::value, tuple<size_t>, tuple<size_t, EdgeLabel>>::type;

    template <class EdgeLabel, typename std::enable_if_t<std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
    static EdgeEntry to_entry(const Edge<EdgeLabel> &edge) {
        return std::make_tuple(get<1>(edge));
    }
    template <class EdgeLabel, typename std::enable_if_t<!std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
    static EdgeEntry to_entry(const Edge<EdgeLabel> &edge) {
        return std::make_tuple(get<1>(edge), get<2>(edge));
    }

    static Edge<EdgeLabel> to_edge(size_t from, const EdgeEntry &edge) {
        return std::tuple_cat(std::make_tuple(from), edge);
    }
};

using SimpleAdjacencyList = AdjacencyList<void>;
using WeightedAdjacencyList = AdjacencyList<i64>;
using UnsignedWeightedAdjacencyList = AdjacencyList<u64>;

template <
        class _EdgeLabel,
        class Element = typename std::conditional<
                std::is_void<_EdgeLabel>::value, bool,
                std::experimental::optional<_EdgeLabel>>::type,
        class Row = std::vector<Element>,
        class Matrix = std::vector<Row>>
struct AdjacencyMatrix {
    using EdgeLabel = _EdgeLabel;

    struct EdgeIteratorFunctions {
        EdgeIteratorFunctions() {}
        using State = std::tuple<size_t, size_t, const AdjacencyMatrix *>;
        bool is_begin(const State &state) const {
            return get<0>(state) == 0 && get<1>(state) == 0;
        }
        bool is_end(const State &state) const {
            return get<0>(state) == get<2>(state)->m_matrix.size();
        }
        bool is_valid(const State &state) const {
            return static_cast<bool>(get<2>(state)->m_matrix[get<0>(state)][get<1>(state)]);
        }
        Edge<EdgeLabel> get_value(const State &state) const {
            return to_edge<EdgeLabel>(get<0>(state), get<1>(state), get<2>(state)->m_matrix[get<0>(state)][get<1>(state)]);
        }

        Edge<EdgeLabel> get_value(State &state) {
            return to_edge<EdgeLabel>(get<0>(state), get<1>(state), get<2>(state)->m_matrix[get<0>(state)][get<1>(state)]);
        }

        void next(State &state) const {
            if ((get<2>(state)->m_matrix[get<0>(state)].size() == 0) ||
                (get<1>(state) + 1) == get<2>(state)->m_matrix[get<0>(state)].size()) {
                get<0>(state) += 1;
                get<1>(state) = 0;
            } else {
                get<1>(state) += 1;
            }
        }
        void previous(State &state) const {
            if (get<1>(state) == 0) {
                get<0>(state) -= 1;
                get<1>(state) = get<2>(state)->m_matrix[get<0>(state)].size() - 1;
            } else {
                get<1>(state) -= 1;
            }
        }
    };
    using EdgeIterator = BaseIterator<EdgeIteratorFunctions>;

    AdjacencyMatrix() {}

    AdjacencyMatrix(size_t vertex_num) : m_matrix(vertex_num, Row(vertex_num)) {}

    const size_t vertices_size() const {
        return this->m_matrix.size();
    }

    Container<EdgeIterator> edges() const {
        auto begin = EdgeIterator(std::make_tuple(0, static_cast<size_t>(0), this));
        auto end = EdgeIterator(std::make_tuple(this->m_matrix.size(), static_cast<size_t>(0), this));
        return Container<EdgeIterator>(begin, end);
    }
    Container<EdgeIterator> edges(size_t n1, size_t n2) const {
        auto begin = EdgeIterator(std::make_tuple(n1, n2, this));
        auto end = EdgeIterator(std::make_tuple(n1, n2 + 1, this));
        return Container<EdgeIterator>(begin, end);
    }

    Container<EdgeIterator> outgoings(size_t n) const {
        auto begin = EdgeIterator(std::make_tuple(n, static_cast<size_t>(0), this));
        auto end = EdgeIterator(std::make_tuple(n + 1, static_cast<size_t>(0), this));
        return Container<EdgeIterator>(begin, end);
    }

    auto has_edge(size_t n1, size_t n2) const {
        return static_cast<bool>(this->m_matrix[n1][n2]);
    }

    void add_edge(const Edge<EdgeLabel> &edge) {
        assert(!this->m_matrix[get<0>(edge)][get<1>(edge)]);
        this->m_matrix[get<0>(edge)][get<1>(edge)] = to_element<EdgeLabel>(edge);
    }

    void remove_edge(const Edge<EdgeLabel> &edge) {
        auto elem = to_element<EdgeLabel>(edge);
        if (this->m_matrix[get<0>(edge)][get<1>(edge)] == elem) {
            this->m_matrix[get<0>(edge)][get<1>(edge)] = Element();
        }
    }

    void remove_edge(size_t n1, size_t n2) {
        this->m_matrix[n1][n2] = Element();
    }

    size_t add_vertex() {
        auto n = this->m_matrix.size();
        REP (i, this->m_matrix.size()) {
            this->m_matrix[i].resize(n + 1);
        }
        this->m_matrix.push(Row(n + 1));
        return n;
    }

    void remove_vertex(size_t n) {
        REP (i, this->m_matrix.size()) {
            REP (j, this->m_matrix.size()) {
                if (i == n || j == n) {
                    this->m_matrix[i][j] = Element();
                }
            }
        }
    }

    void to_undirected() {
        std::vector<Edge<EdgeLabel>> es;
        for (auto edge: this->edges()) {
            std::swap(get<0>(edge), get<1>(edge));
            es.push_back(edge);
        }

        FORE (edge, es) {
            this->add_edge(edge);
        }
    }
private:
    Matrix m_matrix;
    template<class EdgeLabel, typename std::enable_if_t<std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
    static Edge<EdgeLabel> to_edge(size_t n1, size_t n2, bool element) {
        return std::make_tuple(n1, n2);
    }
    template<class EdgeLabel, typename std::enable_if_t<!std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
    static Edge<EdgeLabel> to_edge(
            size_t n1, size_t n2,
            const std::experimental::optional<typename std::enable_if_t<!std::is_void<EdgeLabel>::value, EdgeLabel>> &element
    ) {
        return std::make_tuple(n1, n2, element.value());
    }

    template<class EdgeLabel, typename std::enable_if_t<std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
    static bool to_element(const Edge<EdgeLabel> &edge) {
        return true;
    }
    template<class EdgeLabel, typename std::enable_if_t<!std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
    static std::experimental::optional<EdgeLabel> to_element(const Edge<EdgeLabel> &edge) {
        return std::experimental::make_optional(get<2>(edge));
    }
};

using SimpleAdjacencyMatrix = AdjacencyMatrix<void>;
using WeightedAdjacencyMatrix = AdjacencyMatrix<i64>;
using UnsignedWeightedAdjacencyMatrix = AdjacencyMatrix<u64>;

#ifndef MAIN
#include "common.cc"
#endif

struct UnionFind {
    UnionFind() : m_parents(0), m_rank(0) {}
    UnionFind(size_t N) : m_parents(N), m_rank(N) {
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


void body() {
    using namespace debug;
    auto N = read<i32>();
    auto M = read<i32>();
    auto I = read<i32, i32, i64>(M);

    WeightedAdjacencyList graph(N);
    UnionFind cs(N);

    FORE (x, I) {
        auto L = get<0>(x) - 1;
        auto R = get<1>(x) - 1;
        auto D = get<2>(x);

        graph.add_edge(make_tuple(L, R, D));
        graph.add_edge(make_tuple(R, L, -D));
        cs.merge(L, R);
    }

    auto Max = numeric_limits<i64>::max();
    V<i64> xs(N, Max);
    bool ans = true;
    if (!I.empty()) {
        unordered_set<tuple<size_t, size_t, i64>> visited;
        stack<tuple<size_t, size_t, i64>> s;

        unordered_set<size_t> added;
        REP (i, N) {
            if (added.find(cs.parent(i)) == added.end()) {
                added.insert(cs.parent(i));

                xs[i] = 0;
                for (auto edge: graph.outgoings(i)) {
                    s.push(edge);
                }
            }
        }

        while (!s.empty()) {
            auto edge = s.top();
            s.pop();
            visited.insert(edge);

            auto x1 = get<0>(edge);
            auto x2 = get<1>(edge);
            auto d = get<2>(edge);

            if ((xs[x1] != Max) && (xs[x2] != Max)) {
                if (xs[x2] != xs[x1] + d) {
                    ans = false;
                    break;
                }
                continue;
            }

            if (xs[x1] == Max) {
                xs[x1] = xs[x2] - d;
            }
            if (xs[x2] == Max) {
                xs[x2] = xs[x1] + d;
            }
            for (auto edge: graph.outgoings(x2)) {
                if (visited.find(edge) == visited.end()) {
                    s.push(edge);
                }
            }
        }
    }

    i64 MIN = -1;
    i64 MAX = *(max_element(CTR(xs)));
    REP (i, N) {
        if (xs[i] != -1) {
            MIN = min(MIN, xs[i]);
        }
    }

    if (MAX - MIN > static_cast<i64>(1e9)) {
        ans = false;
    }

    cout << (ans ? "Yes" : "No") << endl;
}