 /*
URL https://beta.atcoder.jp/contests/arc097/tasks/arc097_d
SCORE 800
AC true
WA false
TLE true
MLE false
TASK_TYPE 木 グラフ 木DP 前計算
FAILURE_TYPE 考察不足 考察漏れ 計算量見積もりミス
NOTES
最初の段階（テストWA）
・最後元の場所に戻る必要がないことを見落とし

最初の段階（RE)
・dp tableを配列で取ってMLE+RE
・木DPのdp tableは二分木でとるべき

次の段階（TLE）
・ルートノードにN-1個のコードがぶら下がっているような場合、result, cnt, diffをループで計算してはO(N^2)
・前計算というかメモ化というかしてうまいこと高速化
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

void body() {
    auto N = read<i64>();
    auto xy = read<i64, i64>(N - 1);
    auto cs = read<string>();

    auto edges = Vector<Vector<i64>>(N);
    auto edge_set = OrderedMap<i64, OrderedSet<i64>>();
    REP (i, N) {
        edge_set.emplace(i, OrderedSet<i64>());
    }
    EACH (edge, xy) {
        edge.first -= 1;
        edge.second -= 1;
        edges[edge.first].push_back(edge.second);
        edges[edge.second].push_back(edge.first);
        edge_set[edge.first].insert(edge.second);
        edge_set[edge.second].insert(edge.first);
    }

    // dp[i][j] = 頂点iからjへ移動したあと、jの子要素（i側をrootとする）をすべて黒にしてjに戻ってくるまでの最小時間
    OrderedMap<i64, OrderedMap<i64, i64>> dp;
    // dp2[i][j] = 頂点iからjへ移動したあと、jの子要素（i側をrootとする）をすべて黒にするまでの最小時間
    OrderedMap<i64, OrderedMap<i64, i64>> dp2;
    REP (i, N) {
        dp.emplace(i, OrderedMap<i64, i64>());
        dp2.emplace(i, OrderedMap<i64, i64>());
    }

    Vector<i64> sum(N, 0); // sum[i] = iに隣接する頂点jに対するdp[i][j] != 1であるdp[i][j]の和
    Vector<i64> cnt(N, 0); // sum[i] = iに隣接する頂点jに対するdp[i][j] != 1であるjの個数
    /*
     * diff[i][0] = (dp2[i][j] - dp[i][j])が最小となるjとその値
     * diff[i][1] = (dp2[i][j] - dp[i][j])が番目に小さいjとその値
     */
    Vector<Vector<pair<i64, i64>>> diff(N);
    auto _dfs = [&](i64 i, i64 j, auto dfs) -> void {
        // i -> jへ移動した時を考える (dp[i][j], dp2[i][j]の更新）
        Vector<i64> tmp;
        tmp.reserve(N);
        EACH (k, edge_set[j]) {
            if (k == i) continue; // 移動してくるもとなので無視
            dfs(j, k, dfs); // dp[j][k], dp2[j][k]の更新
            tmp.push_back(k);
        }
        EACH (k, tmp) {
            edge_set[j].erase(k); // もうDFSでは辿らなくて良いので無視
        }

        if(i == -1) return;
        auto cnt2 = cnt[j];
        if (dp[j].find(i) != dp[j].end() && dp[j][i] != 1) {
            // dp[j][i]がsumとcntに含まれる
            cnt2 -= 1;
        }
        i64 result = sum[j] + 2 * cnt2;
        if (dp[j].find(i) != dp[j].end() && dp[j][i] != 1) {
            // dp[j][i]がsumとcntに含まれる
            result -= dp[j][i];
        }
        i64 diff2 = 0;
        REP (k, 2) {
            if (diff[j].size() <= k) break; // 配列範囲外

            if (i == diff[j][k].first) continue; // j -> iへ戻る辺は無視

            diff2 = diff[j][k].second;
            break;
        }

        auto result2 = result;
        // dp[i][j], dp2[i][j]の更新
        if (cs[j] == 'W') {
            if (cnt2 % 2 == 0) {
                /*
                 * i->jの移動でjの色は白->黒
                 * cntが偶数回のとき、jの色は黒 -> 白 -> 黒 ... -> 黒
                 * となるので、戻ってきた時jの色は黒
                 * 一方、戻ってこない場合は最後にjを出る時白なので、反転が必要
                 */
                if(cnt2 != 0) result2 += 1; // ただし、cnt==0のときは黒にして終わりなので大丈夫
            } else {
                /*
                 * 上の逆なので、戻ってきた時jの色は白なので、反転が必要
                 * 戻ってこない場合は黒にして出ていくので反転は不要
                 */
                result += 1;
            }
        } else {
            if (cnt2 % 2 == 0) {
                /*
                 * i->jの移動でjの色は黒->白
                 * cntが偶数回のとき、jの色は白 -> 黒 -> 白 ... -> 白
                 * となるので、戻ってきた時jの色は白なので、戻ってきた場合は反転が必要
                 */
                result += 1;
            } else {
                // 上の逆なので、反転は必要ないが、戻ってこないときは反転が必要
                result2 += 1;
            }
        }
        dp[i][j] = result;
        dp2[i][j] = result2 + diff2;

        /*
         * dp[i][j] == 1の時、iからjに行き、jの色を反転し、jからiに戻るのが最短となる
         * この場合、iからjに行かないほうが短い時間ですべて黒にできるので無視する
         */
        if (dp[i][j] != 1) {
            sum[i] += dp[i][j];
            cnt[i] += 1;

            // 戻ってこなくて良い場合、どれだけ時間を短くできるかをメモ化
            diff[i].emplace_back(j, (dp2[i][j] + 1) - (dp[i][j] + 2));
            push_heap(CTR(diff[i]), [](auto rhs, auto lhs) { return rhs.second > lhs.second; });
        }

    };

    /* dpテーブルの更新 */
    /*
     * dp[i][j]は、iとjが隣接している時のみ更新される。
     * よって、dp[i][j]のうち更新されるセルの数は2*(N-1)個（木なので辺数はN-1）
     *
     * _dfsはdp[i][j]の更新回数だけ呼ばれるので、この計算量はO(N)
     */
    REP (i, N) {
        _dfs(-1, i, _dfs);
    }

    /* 最小値の探索 */
    i64 ans = numeric_limits<i64>::max();
    REP (i, N) {
        i64 x = 0;
        i64 diff = 0;
        i64 cnt = 0;
        EACH (j, edges[i]) {
            if (dp[i][j] == 1) continue ; // dfs中のコメント同様、この頂点には行かなくて良い
            x += dp[i][j] + 2;
            diff = min(diff, (dp2[i][j] + 1) - (dp[i][j] + 2)); // 戻ってこなくて良い場合、一番時間を短くできる部分木を最後にする
            cnt += 1;
        }
        if (cs[i] == 'W') {
            if (cnt % 2 == 0) {
                /*
                 * cntが偶数回のとき、jの色は白 -> 黒 -> 白 ... -> 黒
                 * よって、反転は不要（ただし、cnt==0のときは白のままなので反転が必要
                 */
                if (cnt == 0) x += 1;
            } else {
                // 上の逆なので、戻ってきた時jの色は白
                x += 1;
            }
        } else {
            if (cnt % 2 == 0) {
                /*
                 * i->jの移動でjの色は黒->白
                 * cntが偶数回のとき、jの色は黒 -> 白 -> 黒 ... -> 白
                 * となるので、戻ってきた時jの色は白
                 */
                if (cnt != 0) x += 1; // cnt == 0の時、黒のままなのではんｔ年は不要
            } else {
                // 上の逆なので、反転が不要
            }
        }
        ans = min(ans, x + diff);
    }

    cout << ans << endl;
}
