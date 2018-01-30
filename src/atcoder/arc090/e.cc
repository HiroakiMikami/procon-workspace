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
#include "math.cc"
#endif

#ifndef MAIN
#include "common.cc"
#endif

/*
 * n^r
 */
template<class V>
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

template<class Iterator>
static i64 gcd_ctr(const Iterator &begin, const Iterator &end) {
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

template<class Iterator>
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

template<class V>
static V combination(V n, i64 r) {
    if (r == 0) {
        return 1;
    }

    V x = 1;
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
    return {prime, is_prime_};
}


namespace mod {
    const i64 MOD = 1000000007;

    struct Integer {
        Integer() : value(0) {}
        Integer(i64 value) {
            update_value((value));
        }

        Integer inverse() const {
            return pow(*this, 1000000005);
        }

        Integer operator+(const Integer &rhs) const {
            return this->value + rhs.value;
        }
        Integer operator-(const Integer &rhs) const {
            return this->value - rhs.value;
        }

        Integer operator*(const Integer &rhs) const {
            return this->value * rhs.value;
        }

        Integer operator /(const Integer &rhs) const {
            return this->value * rhs.inverse().value;
        }

        void operator +=(const Integer &rhs) {
            update_value(this->value + rhs.value);
        }

        void operator -=(const Integer &rhs) {
            update_value(this->value - rhs.value);
        }

        void operator *=(const Integer &rhs) {
            update_value(this->value * rhs.value);
        }

        void operator /=(const Integer &rhs) {
            update_value(this->value * rhs.inverse().value);
        }

        bool operator==(const Integer &rhs) const {
            return this->value == rhs.value;
        }
        template <class V>
        bool operator==(V rhs) const {
            return this->value == rhs;
        }

        bool operator!=(const Integer &rhs) const {
            return this->value != rhs.value;
        }
        template <class V>
        bool operator!=(V rhs) const {
            return this->value != rhs;
        }

        const i64 &get() const {
            return this->value;
        }

    private:
        void update_value(i64 new_value) {
            this->value = new_value;
            if (this->value < 0) {
                this->value += MOD;
            }
            if (this->value >= MOD) {
                this->value %= MOD;
            }
        }
        i64 value;
    };

    template <class V>
    static bool operator==(V lhs, const Integer &rhs) {
        return rhs ==lhs;
    }
    template <class V>
    static bool operator!=(V lhs, const Integer &rhs) {
        return rhs != lhs;
    }

    struct fact {
        Integer value;
        Integer inverse;
    };

    static V<fact> fact_table(const size_t n) {
        V<fact> retval(n + 1);

        retval[0].value = 1;
        FOR(i, 1, n + 1) {
            retval[i].value = retval[i - 1].value * i;
        }

        retval[n].inverse = retval[n].value.inverse();
        REPR(i, n) {
            retval[i].inverse = retval[i + 1].inverse * (i + 1);
        }

        return retval;
    }
}

void body() {
    using namespace debug;
    auto N = read<i32>();
    auto M = read<i32>();
    auto S = read<i32>() - 1;
    auto T = read<i32>() - 1;
    auto E = read<i32, i32, i64>(M);

    WeightedAdjacencyList g(N);
    FORE(e, E) {
        auto U = get<0>(e) - 1;
        auto V = get<1>(e) - 1;
        auto D = get<2>(e);

        g.add_edge(std::make_tuple(U, V, D));
        g.add_edge(std::make_tuple(V, U, D));
    }

    // Dijkstra (S => T)
    using optional = std::experimental::optional<size_t>;
    using std::experimental::make_optional;
    using std::vector;
    using std::priority_queue;
    using pair = std::pair<size_t, i64>;
    using std::make_pair;

    auto max = numeric_limits<i64>::max();
    auto distance = vector<i64>(N, max);
    auto is_used = vector<bool>(N, false);
    auto cmp = [&](const pair &v1, const pair &v2) { return v1.second > v2.second; };
    auto Q = priority_queue<pair, vector<pair>, decltype(cmp)>(cmp);

    distance[S] = 0;
    vector<vector<size_t>> previous(N, V<size_t>());
    REP (i, N) {
        Q.push(make_pair(i, distance[i]));
    }

    while (!Q.empty()) {
        auto u = Q.top().first;
        Q.pop();

        if (is_used[u]) {
            continue;
        }
        is_used[u] = true;

        for (auto edge: g.outgoings(u)) {
            auto v = get<1>(edge);
            auto weight = get<2>(edge);
            if (distance[u] != max) {
                if (distance[v] > distance[u] + weight) {
                    distance[v] = distance[u] + weight;
                    Q.push(make_pair(v, distance[v]));
                    previous[v] = {u};
                } else if (distance[v] == distance[u] + weight) {
                    previous[v].push_back(u);
                }
            }
        }
    }

    //　逆にする
    V<V<size_t>> next(N);
    REP (i, N) {
        FORE (x, previous[i]) {
            next[x].push_back(i);
        }
    }

    using namespace mod;
#if 0

    // 数え上げ

    mod::Integer ans(0);
    std::stack<size_t> s;
    s.push(T);
    while (!s.empty()) {
        auto n = s.top();
        s.pop();

        if (n == S) {
            ans += 1;
            continue ;
        }

        FORE (x, previous[n]) {
            s.push(x);
        }
    }
    cout << (ans * (ans - 1)).get() << endl;
#endif
}