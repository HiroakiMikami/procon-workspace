/*
URL https://beta.atcoder.jp/contests/arc065/tasks/arc065_c
SCORE 900
AC true
WA true
TLE true
MLE false
TASK_TYPE マンハッタン距離 チェビシェフ距離 DFS BFS 二分探索
FAILURE_TYPE ライブラリ計算量見積もりミス 考察不足
NOTES
最終的にstd::setのiteratorはrandom access iteratorでないので、std::lower_boundを使うとO(N)になるところが問題だった。
 `std::set::lower_bound`を使うべき。
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
        return this->function(*this->it);
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
            if (!predicate(*this->it)) {
                this->increment();
            }
        }
    }
    decltype(auto) operator *() const {
        return *this->it;
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
        while (this->it != this->end && !this->predicate(*this->it)) {
            ++this->it;
        }
    }
    Iterator it;
    Iterator end;
    P predicate;
};
template <typename Iterator, typename ElementIterator>
struct FlattenedIterator {
    FlattenedIterator(const Iterator &it, const Iterator &end) : it(it), end(end), elem_it() {
        if (this->it != this->end) {
            this->elem_it = it->begin();
        }
    }
    decltype(auto) operator *() const {
        return *this->elem_it;
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
        if (this->it != rhs.it) {
            return false;
        }
        if (this->it == this->end || rhs.it == rhs.end) {
            if (this->end == rhs.end) {
                return true;
            } else {
                return false;
            }
        } else {
            return this->elem_it == rhs.elem_it;
        }
    }
    bool operator!=(const FlattenedIterator<Iterator, ElementIterator> &rhs) const {
        return !(*this == rhs);
    }
private:
    void increment() {
        if (this->it == this->end) return ;

        ++this->elem_it;
        if (this->elem_it == this->it->end()) {
            ++this->it;
            if (this->it != this->end) {
                this->elem_it = this->it->begin();
            }
        }
    }
    Iterator it;
    Iterator end;
    ElementIterator elem_it;
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

// main function (DO NOT EDIT)
int main (int argc, char **argv) {
    cin.tie(0);
    ios_base::sync_with_stdio(false);

    cout << fixed;
    body();

    return 0;
}

Vector<i64>::const_iterator _lower_bound(Vector<i64>::const_iterator first, Vector<i64>::const_iterator last, i64 x) {
    using diff = size_t;
    for (auto len = last - first; len != 0; ) {
        diff half = len / 2;
        auto mid = first;
        std::advance(mid, half);
        if (bool(*mid < x)) {
            len -= half + 1;
            first = ++mid;
        } else {
            len = half;
        }
    }
    return first;
}

Vector<i64>::const_iterator _upper_bound(Vector<i64>::const_iterator first, Vector<i64>::const_iterator last, i64 x) {
    using diff = size_t;
    for (auto len = last - first; len != 0; ) {
        diff half = len / 2;
        auto mid = first;
        std::advance(mid, half);
        if (!bool(x < *mid)) {
            len -= half + 1;
            first = ++mid;
        } else {
            len = half;
        }
    }
    return first;
}

#pragma optimization_level 3
void body() {
    auto N = read<i64>();
    auto a = read<i64>() - 1;
    auto b = read<i64>() - 1;
    auto hs = read<i64, i64>(N);


    auto rotate_45 = [](const pair<i64, i64> &p) -> pair<i64, i64>{
        // 45個回転し、整数にするためにsqrt(2)倍して返す
        return make_pair(p.first - p.second, p.first + p.second);
    };

    EACH (h, hs) {
        h = rotate_45(h);
    }

    OrderedMap<i64, Vector<i64>> xs; //< x座標でgroup化したSet (各配列はy座標でsort）
    OrderedMap<i64, Vector<i64>> ys; //< y座標でgroup化したSet (各配列はx座標でsort）
    OrderedMap<i64, OrderedSet<i64>> xs_set; //< x座標でgroup化したSet (各配列はy座標でsort）
    OrderedMap<i64, OrderedSet<i64>> ys_set; //< y座標でgroup化したSet (各配列はx座標でsort）
    REP (i, N) {
        auto h = hs[i];
        if (xs.find(h.first) == xs.end()) {
            xs.insert({h.first, {}});
            xs_set.insert({h.first, {}});
        }
        if (ys.find(h.second) == ys.end()) {
            ys.insert({h.second, {}});
            ys_set.insert({h.first, {}});
        }

        xs_set[h.first].emplace(h.second);
        ys_set[h.second].emplace(h.first);
    }

    EACH (X, xs_set) {
        xs[X.first] = Vector<i64>(CTR(X.second));
    }
    EACH (Y, ys_set) {
        ys[Y.first] = Vector<i64>(CTR(Y.second));
    }

    // aとbの距離
    auto d = std::max(std::abs(hs[a].first - hs[b].first), std::abs(hs[a].second - hs[b].second));

    i64 ans = 0;
    auto R = OrderedSet<pair<i64, i64>>();

    auto s = std::stack<pair<i64, i64>>();
    s.push(hs[a]);
    while (!s.empty()) {
        auto edge = s.top();
        s.pop();
        if (R.find(edge) != R.end()) {
            continue ; // 探索済
        }
        R.insert(edge);

        // x座標の差がd以下 = y座標が[edge.second-d:edge.second+d]以内のものを探す
        auto f_x = [&](auto x) {
            const auto &X = xs[x];
            const auto &X_set = xs_set[x];
            // [edge.second - d:edge.second+d]
            auto lower = _lower_bound(X.begin(), X.end(), edge.second - d);
            auto upper = _upper_bound(lower, X.end(), edge.second + d);
            ans += upper - lower;

            Vector<i64> tmp(X_set.lower_bound(edge.second - d), X_set.upper_bound(edge.second + d));
            EACH(y, tmp) {
                xs_set[x].erase(y);
                ys_set[y].erase(x);
                s.push({x, y});
            }
        };
        f_x(edge.first + d);
        f_x(edge.first - d);

        // y座標の差がd以下 = x座標が(edge.first-d:edge.first+d)以内のものを探す
        // x座標がedge.first-d, edge.first+dのものは、上ですでに行っている
        auto f_y= [&](auto y) {
            const auto &X = ys[y];
            const auto &X_set = ys_set[y];
            // [edge.second - d:edge.second+d]
            auto lower = _lower_bound(CTR(X), edge.first - d + 1);
            //auto lower = std::partition_point(CTR(X), [&](auto &e) { return e < edge.first - d + 1; });
            auto upper = _upper_bound(lower, X.end(), edge.first + d - 1);
            //auto upper = std::partition_point(lower, X.end(), [&](auto &e) { return !(edge.first + d - 1 < e); });
            ans += upper - lower;
            Vector<i64> tmp(X_set.lower_bound(edge.first - d + 1), X_set.upper_bound(edge.first + d - 1));
            EACH(x, tmp) {
                xs_set[x].erase(y);
                ys_set[y].erase(x);
                s.push({x, y});
            }
        };
        f_y(edge.second + d);
        f_y(edge.second - d);
    }
    cout << ans / 2 << endl;
}
