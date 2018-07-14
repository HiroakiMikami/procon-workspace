/*
URL https://
SCORE 0
AC false
WA false
TLE false
MLE false
TASK_TYPE
FAILURE_TYPE
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
#include <functional>
#include <experimental/optional>

/* import STL */
// stream
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::flush;

// basic types
using std::nullptr_t;
using std::experimental::optional;
using std::pair;
using std::tuple;
using std::string;

// function for basic types
using std::experimental::make_optional;
using std::make_pair;
using std::make_tuple;
using std::get;

/* TODO remove them */
using std::vector;
using std::queue;
using std::stack;

// algorithms
using std::upper_bound;
using std::lower_bound;
using std::min_element;
using std::max_element;
using std::nth_element;
using std::accumulate;

/* macros */
// loops
#define REP(i, n) for (i64 i = 0; i < static_cast<decltype(i)>(n); ++i)
#define REPR(i, n) for (i64 i = (n) - 1; i >= static_cast<decltype(i)>(0); --i)
#define FOR(i, n, m) for (i64 i = (n); i < static_cast<decltype(i)>(m); ++i)
#define FORR(i, n, m) for (i64 i = (m) - 1; i >= static_cast<decltype(i)>(n); --i)
#define EACH(x, xs) for (auto &x: (xs))
#define EACH_V(x, xs) for (auto x: (xs))

// helpers
#define CTR(x) (x).begin(), (x).end()

/* utils for std::tuple */
namespace internal { namespace tuple_utils { // TODO rename to "internal::tuple"
    template<size_t...>
    struct seq {};

    template<size_t N, size_t... Is>
    struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

    template<size_t... Is>
    struct gen_seq<0, Is...> : seq<Is...> {};

    template<class Tuple, size_t... Is>
    void read(std::istream &stream, Tuple &t, seq<Is...>) {
        static_cast<void>((int[]) {0, (void(stream >> get<Is>(t)), 0)...});
    }

    template<class Tuple, size_t... Is>
    void print(std::ostream &stream, Tuple const &t, seq<Is...>) {
        static_cast<void>((int[]) {0, (void(stream << (Is == 0 ? "" : ", ") << get<Is>(t)), 0)...});
    }

    template<size_t I, class F, class A, class... Elems>
    struct ForEach {
        void operator()(A &arg, tuple<Elems...> const &t) const {
            F()(arg, get<I>(t));
            ForEach<I - 1, F, A, Elems...>()(arg, t);
        }

        void operator()(A &arg, tuple<Elems...> &t) const {
            F()(arg, get<I>(t));
            ForEach<I - 1, F, A, Elems...>()(arg, t);
        }
    };

    template<class F, class A, class... Elems>
    struct ForEach<0, F, A, Elems...> {
        void operator()(A &arg, tuple<Elems...> const &t) const {
            F()(arg, get<0>(t));
        }

        void operator()(A &arg, tuple<Elems...> &t) const {
            F()(arg, get<0>(t));
        }
    };

    template<class F, class A, class... Elems>
    void for_each(A &arg, tuple<Elems...> const &t) {
        ForEach<std::tuple_size<tuple<Elems...>>::value - 1, F, A, Elems...>()(arg, t);
    }

    template<class F, class A, class... Elems>
    void for_each(A &arg, tuple<Elems...> &t) {
        ForEach<std::tuple_size<tuple<Elems...>>::value - 1, F, A, Elems...>()(arg, t);
    }
}}

/* utils for Matrix (definition of Matrix) */
namespace internal { namespace matrix {
    template <typename V, int N>
    struct matrix_t {
        using type = std::vector<typename matrix_t<V, N-1>::type>;
    };
    template <typename V>
    struct matrix_t<V, 0> {
        using type = V;
    };

    template <typename V, int N>
    using Matrix = typename matrix_t<V, N>::type;

    template <typename V, typename It, int N>
    struct matrix_helper {
        static Matrix<V, N> create(const It &begin, const It &end, const V &default_value) {
            return Matrix<V, N>(*begin, matrix_helper<V, It, N - 1>::create(begin + 1, end, default_value));
        }
    };
    template <typename V, typename It>
    struct matrix_helper<V, It, 0> {
        static Matrix<V, 0> create(const It &begin __attribute__((unused)),
                                   const It &end __attribute__((unused)),
                                   const V &default_value) {
            return default_value;
        }
    };
}}

/* Primitive types */
using i8  =  int8_t; using  u8 =  uint8_t;
using i16 = int16_t; using u16 = uint16_t;
using i32 = int32_t; using u32 = uint32_t;
using i64 = int64_t; using u64 = uint64_t;
using usize = size_t;

/* Data structure type */
template <typename T> using Vector = std::vector<T>;
template <typename V> using OrderedSet = std::set<V>;
template <typename V> using HashSet = std::unordered_set<V>;
template <typename K, typename V> using OrderedMap = std::map<K, V>;
template <typename K, typename V> using HashMap = std::unordered_map<K, V>;
template <typename T, int N> using Matrix = internal::matrix::Matrix<T, N>;
template <typename T, typename Compare = std::less<T>, typename Container = std::vector<T>>
using PriorityQueue = std::priority_queue<T, Container, Compare>;

/* utils for Vector */
template <typename V>
Vector<V> make_pre_allocated_vector(size_t N) {
    Vector<V> retval;
    retval.reserve(N);
    return retval;
}

/* utils for Matrix */
template <class V, int N>
Matrix<V, N> make_matrix(const std::array<size_t, N> &shape, V default_value = V()) {
    return internal::matrix::matrix_helper<V, decltype(shape.begin()), N>::create(shape.begin(), shape.end(), default_value);
}

/* utils for STL iterators */
namespace internal {
    template <typename Iterator, typename F>
    struct MappedIterator {
        MappedIterator(const Iterator &it, const F &function) : it(it), function(function) {}
        auto operator *() const {
            return this->function(this->it);
        }
        void operator++() { ++this->it; }
        void operator+=(size_t n) { this->it += n; }
        auto operator+(size_t n) const {
            return MappedIterator<Iterator, F>(this->it + n, this->function);
        }
        bool operator==(const MappedIterator<Iterator, F> &rhs) const {
            return this->it == rhs.it;
        }
        bool operator!=(const MappedIterator<Iterator, F> &rhs) const {
            return !(*this == rhs);
        }
    private:
        Iterator it;
        F function;
    };
    template <typename Iterator, typename P>
    struct FilteredIterator {
        FilteredIterator(const Iterator &it, const Iterator &end, const P &predicate)
                : it(it), end(end), predicate(predicate) {
            if (this->it != end) {
                if (!predicate(this->it)) {
                    this->increment();
                }
            }
        }
        decltype(auto) operator *() const {
            return *this->it;
        }
        auto operator ->() const {
            return this->it;
        }
        void operator++() {
            this->increment();
        }
        void operator+=(size_t n) {
            REP (i, n) {
                this->increment();
            }
        }
        auto operator+(size_t n) const {
            auto retval = *this;
            retval += n;
            return retval;
        }
        bool operator==(const FilteredIterator<Iterator, P> &rhs) const {
            return this->it == rhs.it;
        }
        bool operator!=(const FilteredIterator<Iterator, P> &rhs) const {
            return !(*this == rhs);
        }
    private:
        void increment() {
            if (this->it == this->end) {
                return ;
            }
            ++this->it;
            while (this->it != this->end && !this->predicate(this->it)) {
                ++this->it;
            }
        }
        Iterator it;
        Iterator end;
        P predicate;
    };
    template <typename Iterator, typename ElementIterator>
    struct FlattenedIterator {
        FlattenedIterator(const Iterator &it, const Iterator &end) : it(make_pair(it, ElementIterator())), end(end) {
            if (this->it.first != this->end) {
                this->it.second = it->begin();
            }
        }
        decltype(auto) operator *() const {
            return this->it;
        }
        const pair<Iterator, ElementIterator> *operator ->() const {
            return &this->it;
        }
        void operator++() {
            this->increment();
        }
        void operator+=(size_t n) {
            REP (i, n) {
                this->increment();
            }
        }
        auto operator+(size_t n) const {
            auto retval = *this;
            retval += n;
            return retval;
        }
        bool operator==(const FlattenedIterator<Iterator, ElementIterator> &rhs) const {
            if (this->it.first != rhs.it.first) {
                return false;
            }
            if (this->it.first == this->end || rhs.it.first == rhs.end) {
                if (this->end == rhs.end) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return this->it.second == rhs.it.second;
            }
        }
        bool operator!=(const FlattenedIterator<Iterator, ElementIterator> &rhs) const {
            return !(*this == rhs);
        }
    private:
        void increment() {
            auto is_end = [this]() {
                return this->it.first == this->end;
            };
            auto is_valid = [this, &is_end]() {
                if (is_end()) return false;
                if (this->it.second == this->it.first->end()) return false;
                return true;
            };

            auto increment = [this]() {
                if (this->it.second == this->it.first->end()) {
                    ++this->it.first;
                    if (this->it.first != this->end) {
                        this->it.second = this->it.first->begin();
                    }
                } else {
                    ++this->it.second;
                }
            };

            increment();
            while (!is_end() && !is_valid()) {
                increment();
            }

        }
        pair<Iterator, ElementIterator> it;
        Iterator end;
    };
}

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

template <typename C, typename F>
auto iterator_map(const C &c, F function) {
    using Iterator = std::remove_const_t<std::remove_reference_t<decltype(c.begin())>>;
    return Container<internal::MappedIterator<Iterator, F>>(
            internal::MappedIterator<Iterator, F>(c.begin(), function),
            internal::MappedIterator<Iterator, F>(c.end(), function));
}

template <typename C, typename P>
auto iterator_filter(const C &c, P predicate) {
    using Iterator = std::remove_const_t<std::remove_reference_t<decltype(c.begin())>>;
    return Container<internal::FilteredIterator<Iterator, P>>(
            internal::FilteredIterator<Iterator, P>(c.begin(), c.end(), predicate),
            internal::FilteredIterator<Iterator, P>(c.end(), c.end(), predicate));
}

template <typename C>
auto iterator_flatten(const C &c) {
    using Iterator = std::remove_const_t<std::remove_reference_t<decltype(c.begin())>>;
    using ElementIterator = std::remove_const_t<std::remove_reference_t<decltype(c.begin()->begin())>>;
    return Container<internal::FlattenedIterator<Iterator, ElementIterator>>(
            internal::FlattenedIterator<Iterator, ElementIterator>(c.begin(), c.end()),
            internal::FlattenedIterator<Iterator, ElementIterator>(c.end(), c.end()));
}

/* input */
template <class F, class S>
std::istream &operator>>(std::istream &stream, pair<F, S> &pair) {
    stream >> pair.first;
    stream >> pair.second;
    return stream;
}
template <class ...Args>
std::istream &operator>>(std::istream &stream, tuple<Args...> &tuple) {
    internal::tuple_utils::read(stream, tuple, internal::tuple_utils::gen_seq<sizeof...(Args)>());
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
template <typename T, typename F = std::function<T()>>
Vector<T> read(const usize length, F r) {
    auto retval = make_pre_allocated_vector<T>(length);
    REP (i, length) {
        retval.emplace_back(r());
    }
    return retval;
}

template <class T>
Vector<T> read(const usize length) {
    return read<T>(length, [] { return read<T>(); });
}
template <class F, class S>
Vector<pair<F, S>> read(const usize length) {
    return read<pair<F, S>>(length);
}
template <class T1, class T2, class T3, class ...Args>
Vector<tuple<T1, T2, T3, Args...>> read(const usize length) {
    return read<tuple<T1, T2, T3, Args...>>(length);
}

namespace internal {
    template <typename T>
    struct oneline {
        std::string operator()(const T &t) const {
            std::ostringstream oss;
            oss << t;
            return oss.str();
        }
    };
    template <typename F, typename S>
    struct oneline<pair<F, S>> {
        std::string operator()(const pair<F, S> &p) const {
            std::ostringstream oss;
            oss << "{" << oneline<F>()(p.first) << ", " << oneline<S>()(p.second) << "}";
            return oss.str();
        }
    };
    template <typename ...Args>
    struct oneline<tuple<Args...>> {
        struct oneline_tuple {
            template<class V>
            void operator()(Vector<std::string> &strs, const V &v) const {
                strs.emplace_back(oneline<V>()(v));
            }
        };
        std::string operator()(const tuple<Args...> &t) const {
            std::ostringstream oss;
            Vector<std::string> strs;
            internal::tuple_utils::for_each<oneline_tuple, Vector<std::string>, Args...>(strs, t);
            oss << "{";
            REPR (i, strs.size()) {
                oss << strs[i];
                if (i != 0) {
                    oss << ", ";
                }
            }
            oss << "}";
            return oss.str();
        }
    };
    template <>
    struct oneline<bool> {
        std::string operator()(const bool b) const {
            return b ? "true" : "false";
        }
    };
    template <typename X>
    struct oneline<Vector<X>> {
        std::string operator()(const Vector<X> &vec) const {
            std::string retval = "[";
            auto f = oneline<X>();
            REP (i, vec.size()) {
                retval += f(vec[i]);
                if (i != static_cast<i64>(vec.size() - 1)) {
                    retval += ", ";
                }
            }
            retval += "]";
            return retval;
        }
    };
    template <typename X>
    struct oneline<OrderedSet<X>> {
        std::string operator()(const OrderedSet<X> &s) const {
            std::string retval = "{";
            auto f = oneline<X>();
            size_t ctr = 0;
            EACH (x, s) {
                retval += f(x);
                ctr += 1;
                if (ctr != s.size()) {
                    retval += ", ";
                }
            }
            retval += "}";
            return retval;
        }
    };
    template <typename X>
    struct oneline<HashSet<X>> {
        std::string operator()(const HashSet<X> &s) const {
            std::string retval = "{";
            auto f = oneline<X>();
            size_t ctr = 0;
            EACH (x, s) {
                retval += f(x);
                ctr += 1;
                if (ctr != s.size()) {
                    retval += ", ";
                }
            }
            retval += "}";
            return retval;
        }
    };
    template <typename K, typename V>
    struct oneline<OrderedMap<K, V>> {
        std::string operator()(const OrderedMap<K, V> &s) const {
            std::string retval = "{";
            auto f1 = oneline<K>();
            auto f2 = oneline<V>();
            size_t ctr = 0;
            EACH (x, s) {
                retval += f1(x.first);
                retval += ": ";
                retval += f2(x.second);
                ctr += 1;
                if (ctr != s.size()) {
                    retval += ", ";
                }
            }
            retval += "}";
            return retval;
        }
    };
    template <typename K, typename V>
    struct oneline<HashMap<K, V>> {
        std::string operator()(const HashMap<K,V> &s) const {
            std::string retval = "{";
            auto f1 = oneline<K>();
            auto f2 = oneline<V>();
            size_t ctr = 0;
            EACH (x, s) {
                retval += f1(x.first);
                retval += ": ";
                retval += f2(x.second);
                ctr += 1;
                if (ctr != s.size()) {
                    retval += ", ";
                }
            }
            retval += "}";
            return retval;
        }
    };

    template <typename V>
    struct keys { /* no implementation */ };
    template <typename X>
    struct keys<std::vector<X>> {
        Vector<size_t> operator()(const Vector<X> &v) const {
            Vector<size_t> keys;
            REP (i, v.size()) {
                keys.emplace_back(i);
            }
            return keys;
        }
    };
    template <typename K, typename V>
    struct keys<OrderedMap<K, V>> {
        Vector<K> operator()(const OrderedMap<K, V> &c) const {
            Vector<K> keys;
            EACH (elem, c) {
                keys.emplace_back(elem.first);
            }
            return keys;
        }
    };
    template <typename K, typename V>
    struct keys<HashMap<K, V>> {
        Vector<K> operator()(const HashMap<K, V> &c) const {
            Vector<K> keys;
            EACH (elem, c) {
                keys.emplace_back(elem.first);
            }
            return keys;
        }
    };
}

template <typename T>
void dump(const T& t) {
    using namespace internal;
    std::cerr << oneline<T>()(t) << std::endl;
}
template <typename V1, typename V2, typename ...Args>
void dump(const V1 &v1, const V2 &v2, const Args&... args) {
    using namespace internal;
    const auto x = std::make_tuple(v1, v2, args...);
    Vector<std::string> strs;
    internal::tuple_utils::for_each<typename internal::oneline<tuple<Args...>>::oneline_tuple, Vector<std::string>, Args...>(strs, x);
    REPR (i, strs.size()) {
        std::cerr << strs[i];
        if (i != 0) {
            std::cerr << ", ";
        }
    }
    std::cerr << std::endl;
}

template <typename C>
std::string as_set(const C& ctr) {
    Vector<std::string> values;

    using namespace internal;
    EACH (x, ctr) {
        values.emplace_back(oneline<decltype(x)>()(x));
    }
    std::string retval = "---\n";
    REP (i, values.size()) {
        retval += values[i];
        retval += "\n";
    }
    retval += "---";

    return retval;
}

template <typename C>
std::string as_map(const C& ctr) {
    using namespace internal;

    auto ks = keys<C>()(ctr);
    Vector<std::string> keys;
    Vector<std::string> values;

    EACH (key, ks) {
        keys.emplace_back(oneline<decltype(key)>()(key));
        values.emplace_back(oneline<decltype(ctr.at(key))>()(ctr.at(key)));
    }
    size_t l = 0;
    EACH (key, keys) {
        l = std::max(l, key.size());
    }
    std::string retval = "---\n";
    REP (i, values.size()) {
        retval += keys[i];
        REP (j, l - keys[i].size()) {
            retval += " ";
        }
        retval += ": ";
        retval += values[i];
        retval += "\n";
    }
    retval += "---";

    return retval;
}

template <typename C>
std::string as_table(const C &ctr) {
    using namespace internal;
    auto rkeys = keys<C>()(ctr);
    auto ckeys = OrderedSet<std::string>();

    auto values = Vector<pair<std::string, OrderedMap<std::string, std::string>>>();

    /* Stringify all data */
    EACH (rkey, rkeys) {
        auto rkey_str = oneline<decltype(rkey)>()(rkey);
        values.emplace_back(rkey_str, OrderedMap<std::string, std::string>());

        auto row = ctr.at(rkey);
        auto ks = keys<decltype(row)>()(row);
        EACH (ckey, ks) {
            auto ckey_str = oneline<decltype(ckey)>()(ckey);
            ckeys.emplace(ckey_str);
            values.back().second.emplace(ckey_str, oneline<decltype(row.at(ckey))>()(row.at(ckey)));
        }
    }

    /* Calculate string length */
    size_t max_row_key_length = 0;
    EACH (value, values) {
        max_row_key_length = std::max(max_row_key_length, value.first.size());
    }

    OrderedMap<std::string, size_t> max_col_length;
    EACH (ckey, ckeys) {
        max_col_length.emplace(ckey, ckey.size());
    }

    EACH (value, values) {
        EACH (elem, value.second) {
            auto ckey = elem.first;
            auto value = elem.second;
            max_col_length[ckey] = std::max(max_col_length[ckey], value.size());
        }
    }

    std::string retval = "---\n";
    /* Header */
    REP(i, max_row_key_length) {
        retval += " ";
    }
    retval += "  ";
    size_t cnt = 0;
    EACH (ckey, ckeys) {
        retval += ckey;
        REP (j, max_col_length[ckey] - ckey.size()) {
            retval += " ";
        }

        cnt += 1;
        if (cnt != ckeys.size()) {
            retval += ", ";
        }
    }
    retval += "\n------\n";

    /* Values */
    EACH (value, values) {
        retval += value.first;
        REP(i, max_row_key_length - value.first.size()) {
            retval += " ";
        }
        retval += "| ";

        size_t cnt = 0;
        EACH (ckey, ckeys) {
            auto v = std::string("");
            if (value.second.find(ckey) != value.second.end()) {
                v = value.second.at(ckey);
            }
            retval += v;
            REP (j, max_col_length[ckey] - v.size()) {
                retval += " ";
            }

            cnt += 1;
            if (cnt != ckeys.size()) {
                retval += ", ";
            }

        }

        retval += "\n";
    }
    retval += "---";
    return retval;
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
        struct hash_for_element {
            template<class V>
            void operator()(size_t &size, const V &v) const {
                size ^= std::hash<V>()(v);
            }
        };

        size_t operator ()(const tuple<Args...> &t) const {
            size_t retval = 0;

            internal::tuple_utils::for_each<hash_for_element, size_t, Args...>(retval, t);

            return retval;
        }
    };
}
#define MAIN
void body();

// main function (DO NOT EDIT)
int main (int argc, char **argv) {
    cin.tie(0);
    std::ios_base::sync_with_stdio(false);

    cout << std::fixed;
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
using Edge = typename std::conditional<std::is_void<EdgeLabel>::value,
        tuple<size_t, size_t>,
        tuple<size_t, size_t, EdgeLabel>
>::type;

namespace internal { namespace graph {
        template <typename Container, typename EdgeLabel>
        struct create {
            Container operator()(size_t vertex_num) const;
        };

        /* AdjacencyList */
        template <typename EdgeLabel>
        using AdjacencyListEntry = typename std::conditional<std::is_void<EdgeLabel>::value, tuple<size_t>, tuple<size_t, EdgeLabel>>::type;

        template <typename EdgeLabel>
        using AdjacencyList = Vector<Vector<AdjacencyListEntry<EdgeLabel>>>;

        template <typename EdgeLabel>
        struct create<AdjacencyList<EdgeLabel>, EdgeLabel> {
            AdjacencyList<EdgeLabel> operator()(size_t vertex_num) const {
                return AdjacencyList<EdgeLabel>(vertex_num);
            }
        };

        template <class EdgeLabel, typename std::enable_if_t<std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
        static AdjacencyListEntry<EdgeLabel> to_adjacency_list_entry(const Edge<EdgeLabel> &edge) {
            return std::make_tuple(get<1>(edge));
        }
        template <class EdgeLabel, typename std::enable_if_t<!std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
        static AdjacencyListEntry<EdgeLabel> to_adjacency_list_entry(const Edge<EdgeLabel> &edge) {
            return std::make_tuple(get<1>(edge), get<2>(edge));
        }

        template <typename EdgeLabel>
        static Edge<EdgeLabel> to_edge(size_t from, const AdjacencyListEntry<EdgeLabel> &edge) {
            return std::tuple_cat(std::make_tuple(from), edge);
        }

        template <typename EdgeLabel>
        size_t vertex_size(const AdjacencyList<EdgeLabel> &graph) {
            return graph.size();
        }

        template <typename EdgeLabel>
        auto edges(const AdjacencyList<EdgeLabel> &graph) {
            return iterator_map(
                    iterator_flatten(graph),
                    [&graph](const auto &it) -> Edge<EdgeLabel> {
                        auto v = std::distance(graph.begin(), it->first);
                        return to_edge<EdgeLabel>(v, *it->second);
                    });
        }
        template <typename EdgeLabel>
        auto edges(const AdjacencyList<EdgeLabel> &graph, size_t v1, size_t v2) {
            return iterator_map(iterator_filter(graph[v1],
                                                [&graph, v2](const auto &it) { return get<0>(*it) == v2; }),
                                [&graph, v1](const auto &it) -> Edge<EdgeLabel> { return to_edge<EdgeLabel>(v1, *it); });
        }

        template <typename EdgeLabel>
        auto outgoings(const AdjacencyList<EdgeLabel> &graph, size_t v) {
            return iterator_map(graph[v],
                                [&graph, v](const auto &it) { return to_edge<EdgeLabel>(v, *it); });
        }

        template <typename EdgeLabel>
        bool has_edge(const AdjacencyList<EdgeLabel> &graph, size_t v1, size_t v2) {
            return std::find_if(CTR(graph[v1]), [v2](const auto &elem) { return get<0>(elem) == v2; }) != graph[v1].end();
        }

        template <typename EdgeLabel>
        size_t add_vertex(AdjacencyList<EdgeLabel> &graph) {
            graph.push_back({});
            return graph.size() - 1;
        }
        template <typename EdgeLabel>
        void remove_vertex(AdjacencyList<EdgeLabel> &graph, size_t v) {
            REP (i, graph.size()) {
                if (static_cast<size_t>(i) == v) {
                    graph[i].clear();
                } else {
                    auto &edges = graph[i];
                    edges.erase(
                            remove_if(edges.begin(), edges.end(), [v](const auto &edge) { return get<0>(edge) == v; }),
                            edges.end()
                    );
                }
            }
        }

        template <typename EdgeLabel>
        void add_edge(AdjacencyList<EdgeLabel> &graph, const Edge<EdgeLabel>& edge) {
            graph[get<0>(edge)].push_back(to_adjacency_list_entry<EdgeLabel>(edge));
        }
        template <typename EdgeLabel>
        void remove_edge(AdjacencyList<EdgeLabel> &graph, const Edge<EdgeLabel>& edge) {
            graph[get<0>(edge)].erase(
                    remove(graph[get<0>(edge)].begin(), graph[get<0>(edge)].end(), to_adjacency_list_entry<EdgeLabel>(edge)),
                    graph[get<0>(edge)].end()
            );
        }
        template <typename EdgeLabel>
        void remove_edge(AdjacencyList<EdgeLabel> &graph, size_t n1, size_t n2) {
            graph[n1].erase(
                    remove_if(
                            graph[n1].begin(), graph[n1].end(),
                            [n2](const auto &edge) { return get<0>(edge) == n2; }
                    ),
                    graph[n1].end()
            );
        }

        /* AdjacencyMatrix */
        template <typename EdgeLabel>
        using AdjacencyMatrixElement = typename std::conditional<
                std::is_void<EdgeLabel>::value, bool,
                std::experimental::optional<EdgeLabel>>::type;

        template <typename EdgeLabel>
        using AdjacencyMatrix= Matrix<AdjacencyMatrixElement<EdgeLabel>, 2>;
        template <typename EdgeLabel>
        struct create<AdjacencyMatrix<EdgeLabel>, EdgeLabel> {
            AdjacencyMatrix<EdgeLabel> operator()(size_t vertex_num) const {
                return make_matrix<AdjacencyMatrixElement<EdgeLabel>, 2>({vertex_num, vertex_num});
            }
        };

        template<class EdgeLabel, typename std::enable_if_t<std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
        static Edge<EdgeLabel> to_edge(size_t n1, size_t n2, bool element __attribute__((unused))) {
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
        static bool to_adjacency_matrix_element(const Edge<EdgeLabel> &edge __attribute__((unused))) {
            return true;
        }
        template<class EdgeLabel, typename std::enable_if_t<!std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
        static std::experimental::optional<EdgeLabel> to_adjacency_matrix_element(const Edge<EdgeLabel> &edge) {
            return std::experimental::make_optional(get<2>(edge));
        }

        template <typename EdgeLabel>
        size_t vertex_size(const AdjacencyMatrix<EdgeLabel> &matrix) {
            return matrix.size();
        }

        template <typename EdgeLabel>
        auto has_edge(const AdjacencyMatrix<EdgeLabel> &matrix, size_t n1, size_t n2) {
            return static_cast<bool>(matrix[n1][n2]);
        }

        template <typename EdgeLabel>
        auto edges(const AdjacencyMatrix<EdgeLabel> &matrix) {
            return iterator_map(iterator_filter(iterator_flatten(matrix),
                                                [](const auto &elem) { return static_cast<bool>(*elem->second); }),
                                [&matrix](const auto &elem) {
                                    auto v = distance(matrix.begin(), elem->first);
                                    auto u = distance(elem->first->begin(), elem->second);
                                    return to_edge<EdgeLabel>(v, u, *elem->second);
                                });
        }
        template <typename EdgeLabel>
        auto edges(const AdjacencyMatrix<EdgeLabel> &matrix, size_t n1, size_t n2) {
            if (has_edge<EdgeLabel>(matrix, n1, n2)) {
                return Vector<Edge<EdgeLabel>>(1, to_edge<EdgeLabel>(n1, n2, matrix[n1][n2]));
            } else {
                return Vector<Edge<EdgeLabel>>();
            }
        }

        template <typename EdgeLabel>
        auto outgoings(const AdjacencyMatrix<EdgeLabel> &matrix, size_t n) {
            return iterator_map(iterator_filter(iterator_map(matrix[n],
                                                             [&matrix, n](const auto &elem) -> std::experimental::optional<Edge<EdgeLabel>> {
                                                                 if (*elem) {
                                                                     auto u = distance(matrix[n].begin(), elem);
                                                                     return std::experimental::make_optional(to_edge<EdgeLabel>(n, u, *elem));
                                                                 } else {
                                                                     return std::experimental::optional<Edge<EdgeLabel>>();
                                                                 }
                                                             }),
                                                [](const auto &elem) { return static_cast<bool>(*elem); }),
                                [](const auto &elem) { return (*elem).value(); });
        }

        template <typename EdgeLabel>
        void add_edge(AdjacencyMatrix<EdgeLabel> &matrix, const Edge<EdgeLabel> &edge) {
            assert(!matrix[get<0>(edge)][get<1>(edge)]);
            matrix[get<0>(edge)][get<1>(edge)] = to_adjacency_matrix_element<EdgeLabel>(edge);
        }

        template <typename EdgeLabel>
        void remove_edge(AdjacencyMatrix<EdgeLabel> &matrix, const Edge<EdgeLabel> &edge) {
            auto elem = to_adjacency_matrix_element<EdgeLabel>(edge);
            if (matrix[get<0>(edge)][get<1>(edge)] == elem) {
                matrix[get<0>(edge)][get<1>(edge)] = AdjacencyMatrixElement<EdgeLabel>();
            }
        }

        template <typename EdgeLabel>
        void remove_edge(AdjacencyMatrix<EdgeLabel> &matrix, size_t n1, size_t n2) {
            matrix[n1][n2] = AdjacencyMatrixElement<EdgeLabel>();
        }

        template <typename EdgeLabel>
        size_t add_vertex(AdjacencyMatrix<EdgeLabel> &matrix) {
            auto n = matrix.size();
            REP (i, matrix.size()) {
                matrix[i].resize(n + 1);
            }
            matrix.emplace(n + 1);
            return n;
        }

        template <typename EdgeLabel>
        void remove_vertex(AdjacencyMatrix<EdgeLabel> &matrix, size_t n) {
            REP (i, matrix.size()) {
                REP (j, matrix.size()) {
                    if (static_cast<size_t>(i) == n || static_cast<size_t>(j) == n) {
                        matrix[i][j] = AdjacencyMatrixElement<EdgeLabel>();
                    }
                }
            }
        }
    }}

template <typename _EdgeLabel, typename Container>
struct Graph {
    using EdgeLabel = _EdgeLabel;

    Graph() {}
    Graph(const Container& c) : container(c) {}
    Graph(size_t vertex_num) : container(internal::graph::create<Container, EdgeLabel>()(vertex_num)) {}

    size_t vertices_size() const {
        return internal::graph::vertex_size<EdgeLabel>(this->container);
    }

    auto edges() const {
        return internal::graph::edges<EdgeLabel>(this->container);
    }
    auto edges(size_t n1, size_t n2) const {
        return internal::graph::edges<EdgeLabel>(this->container, n1, n2);
    }

    auto outgoings(size_t n) const {
        return internal::graph::outgoings<EdgeLabel>(this->container, n);
    }

    auto has_edge(size_t n1, size_t n2) const {
        return internal::graph::has_edge<EdgeLabel>(this->container, n1, n2);
    }

    void add_edge(const Edge<EdgeLabel>& edge) {
        internal::graph::add_edge<EdgeLabel>(this->container, edge);
    }
    void remove_edge(const Edge<EdgeLabel>& edge) {
        internal::graph::remove_edge<EdgeLabel>(this->container, edge);
    }
    void remove_edge(size_t n1, size_t n2) {
        internal::graph::remove_edge<EdgeLabel>(this->container, n1, n2);
    }
    size_t add_vertex() {
        return internal::graph::add_vertex<EdgeLabel>(this->container);
    }
    void remove_vertex(size_t n) {
        internal::graph::remove_vertex<EdgeLabel>(this->container, n);
    }

    void to_undirected() {
        std::vector<Edge<EdgeLabel>> es;
        for (auto edge: this->edges()) {
            std::swap(get<0>(edge), get<1>(edge));
            es.push_back(edge);
        }

        EACH (edge, es) {
            this->add_edge(edge);
        }
    }
    Container container;
};

template <typename EdgeLabel>
using AdjacencyList = Graph<EdgeLabel, internal::graph::AdjacencyList<EdgeLabel>>;
using SimpleAdjacencyList = AdjacencyList<void>;
using WeightedAdjacencyList = AdjacencyList<i64>;
using UnsignedWeightedAdjacencyList = AdjacencyList<u64>;

template <typename EdgeLabel>
using AdjacencyMatrix = Graph<EdgeLabel, internal::graph::AdjacencyMatrix<EdgeLabel>>;
using SimpleAdjacencyMatrix = AdjacencyMatrix<void>;
using WeightedAdjacencyMatrix = AdjacencyMatrix<i64>;
using UnsignedWeightedAdjacencyMatrix = AdjacencyMatrix<u64>;

namespace internal {
    template <typename Label, typename Container>
    struct oneline<Graph<Label, Container>> {
        std::string operator()(const Graph<Label, Container> &g) const {
            Vector<size_t> V;
            REP (i, g.vertices_size()) {
                V.push_back(i);
            }
            Vector<Edge<Label>> E;
            EACH_V(e, g.edges()) {
                E.push_back(e);
            }
            using P = pair<decltype(V), decltype(E)>;
            return oneline<P>()(make_pair(V, E));
        }
    };
}

#ifdef __LOCAL__
#include <cstdio>
#include <fstream>

namespace internal { namespace graph {
        template <typename Label, typename Container, typename F>
        void show(const Graph<Label, Container> &g, F get_label) {
            /* Convert graph to dot format*/
            std::ostringstream oss;
            oss << "digraph{\n";
            // Vertices
            REP (i, g.vertices_size()) {
                oss << "node_" << i << " [label=\"" << i << "\"];" << "\n";
            }

            // Edges
            EACH_V(edge, g.edges()) {
                auto from = get<0>(edge);
                auto to = get<1>(edge);
                auto label = get_label(edge);
                std::ostringstream l;
                l << label;
                if (l.str().empty()) {
                    oss << "node_" << from << " -> " << "node_" << to << ";\n";
                } else {
                    oss << "node_" << from << " -> " << "node_" << to << " [label=" << label << "];\n";
                }
            }

            oss << "}\n";
            auto dot = oss.str();

            /* Generate temporary file */
            auto tmpfilename = std::tmpnam(nullptr);
            std::ofstream s(tmpfilename);
            s << dot << std::flush;

            std::string cmd = "dot -Tgtk \"";
            cmd += tmpfilename;
            cmd += "\"";
            std::system(cmd.c_str());

            /* Remove temporary file */
            std::remove(tmpfilename);
        }
    }}
namespace graph {
    template <typename Container>
    void show(const Graph<void, Container> &g) {
        internal::graph::show(g, [](auto x) { return ""; });
    }
    template <typename Label, typename Container>
    void show(const Graph<Label, Container> &g) {
        internal::graph::show(g, [](auto x) { return get<2>(x); });
    }
}
#else
namespace graph {
    template <typename Label, typename Container>
    void show(const Graph<Label, Container> &g) {
    }
}
#endif


void body() {
    auto N = read<i64>();
    auto M = read<i64>();
    auto Ps = read<i64>(N - 1);
    auto BC = read<i64, i64>(M);

    SimpleAdjacencyList graph(N);
    SimpleAdjacencyList graph2(N);
    REP (i, N - 1) {
        graph.add_edge(i, Ps[i]);
        graph2.add_edge(Ps[i], i);
    }

    std::queue<i64> q;
    OrderedMap<i64, i64> drops;
    REP (i, N) {
        drops[i] = -1;
    }
    EACH (elem, BC) {
        q.push(elem.first);
        drops[elem.first] = elem.scond;
    }

    while (!q.empty()) {
        auto n = q.front();
        q.pop();

        // 子機の必要個数を計算

        EACH_V(edge, graph.outgoings(n)) {
            q.push(get<1>(edge));
        }
    }

}
