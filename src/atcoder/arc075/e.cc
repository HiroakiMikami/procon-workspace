#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include <cstdint>
#include <queue>
#include <unordered_map>
#include <algorithm>

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
        while (index > 0) {
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

int body(int &argc, char **argv);

// main function (DO NOT EDIT)
int main (int argc, char **argv) {
    ios_base::sync_with_stdio(false);
    return body(argc, argv);
}
int body(int &argc, char **argv) {
    auto N = read<i64>();
    auto K = read<i64>();
    auto as = read<i64>(N);
    FORE (a, as) {
        a -= K;
    }

    auto sum = V<i64>(N);
    REP (i, N) {
        if (i == 0) {
            sum[i] = as[i];
        } else {
            sum[i] = sum[i - 1] + as[i];
        }
    }

    // メモ：転倒数

    // 座標圧縮
    auto xs = sum;
    xs.push_back(0);
    sort(xs.begin(), xs.end());
    auto m = unordered_map<i64, i64>();
    m.reserve(N + 1);
    i64 n = 0;
    FORE (x, xs) {
        if (m.find(x) == m.end()) {
            m.insert({x, n});
            n += 1;
        }
    }

    u64 result = 0;
    BIT<i64> bit(N);
    bit.add(m.find(0)->second, 1);
    FORE (a, sum) {
        auto b = m.find(a)->second;
        result += bit.sum(b + 1);
        bit.add(b, 1);
    }

    cout << result << endl;

    return 0;
}
