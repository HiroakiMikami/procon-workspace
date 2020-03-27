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
            this->find_valid();
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
        bool is_end() const {
            return this->it.first == this->end;
        }
        bool is_valid() const {
            if (this->is_end()) return false;
            if (this->it.second == this->it.first->end()) return false;
            return true;
        }
        void _increment() {
            if (this->it.second == this->it.first->end()) {
                ++this->it.first;
                if (this->it.first != this->end) {
                    this->it.second = this->it.first->begin();
                }
            } else {
                ++this->it.second;
            }
        }
        void find_valid() {
            while (!this->is_end() && !this->is_valid()) {
                this->_increment();
            }
        }
        void increment() {
            this->_increment();
            while (!this->is_end() && !this->is_valid()) {
                this->_increment();
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
    using F = typename oneline<tuple<V1, V2, Args...>>::oneline_tuple;
    auto x = std::make_tuple(v1, v2, args...);
    Vector<std::string> strs;
    internal::tuple_utils::for_each<F, Vector<std::string>, V1, V2, Args...>(strs, x);
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
#ifndef MAIN
#include "common.cc"
#endif

static i64 mod(i64 a, i64 m) {
    return (a % m + m) % m;
}

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

static i64 gcd_ctr(const Vector<i64> &xs) {
    return gcd_ctr(xs.begin(), xs.end());
}



/*
 * a * get<0>(r) + b * get<1>(r) = get<2>(r), get<2>(r) = gcd(a, b)
 */
static tuple<i64, i64, i64> ext_gcd(i64 a, i64 b) {
    auto ext_gcd_ = [](i64 a, i64 b, i64& p, i64 &q, auto f) -> i64 {
        if (b == 0) {
            p = 1;
            q = 0;
            return a;
        }
        i64 d = f(b, a % b, q, p, f);
        q -= a / b * p;
        return d;
    };
    i64 p = 0, q = 0;
    auto d = ext_gcd_(a, b, p, q, ext_gcd_);
    return make_tuple(p, q, d);
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

static i64 lcm_ctr(const Vector<i64> &xs) {
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

static Vector<i64> divisor(i64 n) {
    Vector<i64> res;
    for (i64 i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            res.push_back(i);
            if (i != n / i) res.push_back(n / i);
        }
    }
    return res;
}

static HashMap<i64, size_t> prime_factor(i64 n) {
    HashMap<i64, size_t> res;
    for (i64 i = 2; i * i <= n; i++) {
        while (n % i == 0) {
            res[i] += 1;
            n /= i;
        }
    }
    if (n != 1) res[n] = 1;
    return res;
}

static pair<Vector<i64>, Vector<bool>> sieve(i64 n) {
    Vector<i64> prime;
    Vector<bool> is_prime_(n + 1, true);
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

/*
 * x = b1 (mod m1)
 * x = b2 (mod m2)
 * => x = r.first (mod r.second) (r is a return value)
 */
static std::experimental::optional<pair<i64, i64>> chinese_rem(i64 b1, i64 m1, i64 b2, i64 m2) {
    auto elem = ext_gcd(m1, m2);
    auto p = get<0>(elem);
    auto d = get<2>(elem);
    if ((b2 - b1) % d != 0) return {};

    i64 m = m1 * (m2 / d); //< lcm(m1, m2)
    i64 r = mod(b1 + m1 * ((b2 - b1) / d * p % (m2 / d)), m);
    return make_optional(std::make_pair(r, m));

}
template <typename Iterator1, typename Iterator2>
static std::experimental::optional<pair<i64, i64>> chinese_rem_ctr(Iterator1 b_begin, Iterator1 b_end, Iterator2 m_begin, Iterator2 m_end) {
    i64 r = 0, M = 1;
    auto b = b_begin;
    auto m = m_begin;
    for (; b != b_end && m != m_end; ++b, ++m) {
        auto elem = ext_gcd(M, *m);
        auto p = get<0>(elem);
        auto d = get<2>(elem);

        if ((*b - r) % d != 0) return {};
        r += M * ((*b - r) / d * p % (*m / d));
        M *= *m / d;
    }
    return make_optional(std::make_pair(mod(r, M), M));
}
static std::experimental::optional<pair<i64, i64>> chinese_rem_ctr(const Vector<i64> &b, const Vector<i64> &m) {
    return chinese_rem_ctr<decltype(b.begin()), decltype(m.begin())>(CTR(b), CTR(m));
}

void body() {
    auto N = read<i64>();
    auto M = read<i64>();
    auto as = read<i64>(N);
    i64 p = -1;
    i64 p_power = 0;
    auto As = Vector<i64>();
    EACH (a, as) {
        a /= 2;
        i64 i = 0;
        while (a % 2 == 0) {
            a /= 2;
            i += 1;
        }
        if (p < 0) {
            p = i;
            p_power = pow(i64(2), i);
        }
        if (p != i) {
            cout << 0 << endl;
            return ;
        }
        As.push_back(a);
    }

    auto Y = lcm_ctr(CTR(As));
    if (M < p_power * Y) {
        cout << 0 << endl;
        return ;
    }
    dump(p_power, Y, M);
    auto ans = (M - p_power * Y) / (2 * p_power * Y) + 1;
    cout << ans << endl;
}
