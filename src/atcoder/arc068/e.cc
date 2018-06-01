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

using namespace std; // TODO Remove this

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

/* internal code */
namespace internal {
    /* utils for std::tuple */
    namespace tuple_utils {
        template<size_t...>
        struct seq {};

        template<size_t N, size_t... Is>
        struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

        template<size_t... Is>
        struct gen_seq<0, Is...> : seq<Is...> {};

        template<class Tuple, size_t... Is>
        void read(istream &stream, Tuple &t, seq<Is...>) {
            static_cast<void>((int[]) {0, (void(stream >> get<Is>(t)), 0)...});
        }

        template<class Tuple, size_t... Is>
        void print(ostream &stream, Tuple const &t, seq<Is...>) {
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
            ForEach<tuple_size<tuple<Elems...>>::value - 1, F, A, Elems...>()(arg, t);
        }

        template<class F, class A, class... Elems>
        void for_each(A &arg, tuple<Elems...> &t) {
            ForEach<tuple_size<tuple<Elems...>>::value - 1, F, A, Elems...>()(arg, t);
        }

        struct hash_for_element {
            template<class V>
            void operator()(size_t &size, const V &v) const {
                size ^= hash<V>()(v);
            }
        };
    }

    /* utils for std::vector */
    namespace vector_utils {
        template <typename V, int N>
        struct matrix_t {
            using type = std::vector<typename matrix_t<V, N-1>::type>;
        };
        template <typename V>
        struct matrix_t<V, 0> {
            using type = V;
        };

        template <typename V, int N>
        using Matrix = typename internal::vector_utils::matrix_t<V, N>::type;

        template <typename V, typename It, int N>
        struct matrix_helper {
            static Matrix<V, N> create(const It &begin, const It &end, const V &default_value) {
                return Matrix<V, N>(*begin, matrix_helper<V, It, N - 1>::create(begin + 1, end, default_value));
            }
        };
        template <typename V, typename It>
        struct matrix_helper<V, It, 0> {
            static Matrix<V, 0> create(const It &begin, const It &end, const V &default_value) {
                return default_value;
            }
        };
    }
}


/* Primitive types */
using i8  =  int8_t; using  u8 =  uint8_t;
using i16 = int16_t; using u16 = uint16_t;
using i32 = int32_t; using u32 = uint32_t;
using i64 = int64_t; using u64 = uint64_t;
using usize = size_t;

/* Data structure type */
template <class T> using V = vector<T>; // TODO Replace with Vector
template <typename T> using Vector = internal::vector_utils::Matrix<T, 1>;
template <typename T, int N> using Matrix = internal::vector_utils::Matrix<T, N>;
template <typename V> using OrderedSet = std::set<V>;
template <typename V> using HashSet = std::unordered_set<V>;
template <typename K, typename V> using OrderedMap = std::map<K, V>;
template <typename K, typename V> using HashMap = std::unordered_map<K, V>;

/* utils for std::vector */
template <typename V>
std::vector<V> make_pre_allocated_vector(size_t N) {
    std::vector<V> retval;
    retval.reserve(N);
    return retval;
}

template <class V, int N>
Matrix<V, N> make_matrix(const std::array<size_t, N> &shape, V default_value = V()) {
    return internal::vector_utils::matrix_helper<V, decltype(shape.begin()), N>::create(shape.begin(), shape.end(), default_value);
}
/* utils for STL containers */
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

/* utils for STL iterators */
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

template <typename C, typename F>
auto iterator_map(const C &c, F function) {
    using Iterator = std::remove_const_t<std::remove_reference_t<decltype(c.begin())>>;
    return Container<MappedIterator<Iterator, F>>(MappedIterator<Iterator, F>(c.begin(), function),
            MappedIterator<Iterator, F>(c.end(), function));
}

template <typename C, typename P>
auto iterator_filter(const C &c, P predicate) {
    using Iterator = std::remove_const_t<std::remove_reference_t<decltype(c.begin())>>;
    return Container<FilteredIterator<Iterator, P>>(FilteredIterator<Iterator, P>(c.begin(), c.end(), predicate),
                                                    FilteredIterator<Iterator, P>(c.end(), c.end(), predicate));
}

template <typename C>
auto iterator_flatten(const C &c) {
    using Iterator = std::remove_const_t<std::remove_reference_t<decltype(c.begin())>>;
    using ElementIterator = std::remove_const_t<std::remove_reference_t<decltype(c.begin()->begin())>>;
    return Container<FlattenedIterator<Iterator, ElementIterator>>(
            FlattenedIterator<Iterator, ElementIterator>(c.begin(), c.end()),
            FlattenedIterator<Iterator, ElementIterator>(c.end(), c.end()));
}

/* utils for STL iterators (TODO deprecated because it is too complex) */
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

/* input */
template <class F, class S>
istream &operator>>(istream &stream, pair<F, S> &pair) {
    stream >> pair.first;
    stream >> pair.second;
    return stream;
}
template <class ...Args>
istream &operator>>(istream &stream, tuple<Args...> &tuple) {
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

/* debug output */
template <class F, class S>
ostream &operator<<(ostream& stream, const pair<F, S> &pair) {
    stream << "{" << pair.first << ", " << pair.second << "}";
    return stream;
}
template <class ...Args>
ostream &operator<<(ostream& stream, const tuple<Args...> &tuple) {
    stream << "{";
    internal::tuple_utils::print(stream, tuple, internal::tuple_utils::gen_seq<sizeof...(Args)>());
    stream << "}";
    return stream;
}

template <typename T>
void dump(const T& t) {
    std::cerr << t << std::endl;
}
template <typename F, typename S>
void dump(const pair<F, S>& p) {
    std::cerr << p << std::endl;
}
template <class ...Args>
void dump(const tuple<Args...> &tuple) {
    std::cerr << tuple << std::endl;
}
template <typename V1, typename V2, typename ...Args>
void dump(const V1 &v1, const V2 &v2, const Args&... args) {
    const auto x = std::make_tuple(v1, v2, args...);
    internal::tuple_utils::print(std::cerr, x, internal::tuple_utils::gen_seq<sizeof...(Args) + 2>());
    std::cerr << std::endl;
}

template <typename T>
void dump_matrix(const Matrix<T, 0> & matrix) {
    dump(matrix);
}
template <typename T>
void dump_matrix(const Matrix<T, 1> & matrix) {
    EACH (x, matrix) {
        std::cerr << x << " ";
    }
    std::cerr << std::endl;
}
template <typename T>
void dump_matrix(const Matrix<T, 2> & matrix) {
    EACH (x, matrix) {
        dump_matrix<T>(x);
    }
}

template <typename C>
void dump_set(const C &container) {
    EACH(c, container) {
        dump(c);
    }
}
template <typename C>
void dump_seq(const C &container) {
    int index = 0;
    EACH(c, container) {
        dump(index, c);
        index += 1;
    }
}
template <typename C>
void dump_map(const C &container) {
    EACH(c, container) {
        const auto &key = c.first;
        const auto &value = c.second;
        std::cerr << key << "\t:" << value << std::endl;
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

            internal::tuple_utils::for_each<internal::tuple_utils::hash_for_element, size_t, Args...>(retval, t);

            return retval;
        }
    };
}
#define MAIN
void body();

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


// main function (DO NOT EDIT)
int main (int argc, char **argv) {
    cin.tie(0);
    ios_base::sync_with_stdio(false);

    cout << fixed;
    body();

    return 0;
}

void body() {
    auto N = read<i64>();
    auto M = read<i64>();
    auto lr = read<i64, i64>(N);

    // 愚直解
    FOR (m, 1, M + 1) {
        i64 ans = 0;
        REP (i, N) {
            if (lr[i].first % m == 0) {
                ans += 1;
            } else if (lr[i].first / m != lr[i].second / m) {
                ans += 1;
            }
        }
        cout << ans << endl;
    }
}
