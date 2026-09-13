// 普通线性基，最大子序列异或和；处理非负 ll，第 0 到 n-1 位，n <= 63。
class XorBasis {
    vl b;

public:
    XorBasis(int n) : b(n) {}
    void insert(ll x) {
        frep(i, sz(b) - 1, 0) {
            if (x >> i & 1) {
                if (b[i] == 0) {
                    b[i] = x;
                    return;
                }
                x ^= b[i];
            }
        }
    }
    ll max_xor() {
        ll res = 0;
        frep(i, sz(b) - 1, 0) { res = max(res, res ^ b[i]); }
        return res;
    }
};
class Solution {
public:
    ll maxXorSubsequences(vl& nums) {
        if (nums.empty()) return 0;
        int m = bit_width((uint64_t)ranges::max(nums));
        XorBasis b(m);
        for (ll x : nums) b.insert(x);
        return b.max_xor();
    }
};

/*
前缀线性基：查询区间 [l, r] 内任选若干个数能得到的异或值，允许选空集。

普通线性基不能像前缀和一样相减，需要在每个基向量上额外维护位置：
    b[r][i]：前 r 个数中，最高位为 i 的基向量。
    pos[r][i]：这个基向量的一种表示中，所用原数组元素的最小下标；空位为 0。
插入时，同一最高位优先保留下标更靠右的向量，较旧的向量继续向低位消元。
这样对任意 l，b[r] 中满足 pos[r][i] >= l 的向量恰好构成区间 [l, r] 的线性基。
注意 pos 不是“最后一次插入的下标”，向量经过异或后可能由多个原数合成。

原数组 a 是 0-based，版本 r 表示前 r 个元素；查询 [l, r] 是 1-based 闭区间。
版本 0 为空，每次 insert 都追加一个元素，即使插入的是 0 或线性相关的数。
查询需满足 1 <= l <= r <= 已插入元素个数；支持末尾追加，不支持直接修改历史元素。

m 是二进制位数，处理第 0 到 m-1 位，要求 1 <= m <= 63。
插入值和查询 x 均为非负 ll，且小于 2^m；默认 m=63 覆盖非负 ll。
建好 n 个前缀需要 O(nm) 时间、O(nm) 空间，单次查询 O(m)，追加均摊 O(m)。
每个前缀都保存一份基，按值域选取满足要求的最小 m 可以节省内存。
*/
class PrefixXorBasis {
    int m;
    vvl b;
    vector<vi> pos;

public:
    PrefixXorBasis(int m = 63) : m(m), b(1, vl(m)), pos(1, vi(m)) {}

    PrefixXorBasis(const vl& a, int m = 63) : PrefixXorBasis(m) {
        b.reserve(sz(a) + 1);
        pos.reserve(sz(a) + 1);
        for (ll x : a) insert(x);
    }

    void insert(ll x) {
        int r = sz(b), p = r;
        b.push_back(b.back());
        pos.push_back(pos.back());  // 复制上一个前缀，再插入新数
        frep(i, m - 1, 0) {
            if (!(x >> i & 1)) continue;
            if (b[r][i] == 0) {
                b[r][i] = x;
                pos[r][i] = p;
                return;
            }
            if (pos[r][i] < p) {
                swap(b[r][i], x);
                swap(pos[r][i], p);  // 数值和位置一起换，留下较新的向量
            }
            x ^= b[r][i];  // 此时 p 较小，消元后仍用 p 标记，继续处理较旧的向量
        }
    }

    // 最大子集异或和；传入 x 时，求 max(x ^ 区间子集异或和)。
    ll max_xor(int l, int r, ll x = 0) const {
        ll res = x;
        frep(i, m - 1, 0) {
            if (pos[r][i] >= l) res = max(res, res ^ b[r][i]);
        }
        return res;
    }

    // 判断区间内能否选出若干个数异或得到 x；允许空集，所以 x=0 总是可行。
    bool contains(int l, int r, ll x) const {
        frep(i, m - 1, 0) {
            if (x >> i & 1) {
                if (pos[r][i] < l) return false;  // 这一位没有可用的基向量
                x ^= b[r][i];
            }
        }
        return x == 0;
    }

    // 区间线性基的秩 k，不同子集异或值共有 2^k 个（包含 0）。
    int rank(int l, int r) const {
        int res = 0;
        rep(i, 0, m - 1) { res += pos[r][i] >= l; }
        return res;
    }
};

/*
用法：
    // a 为已读入的 vl 数组，m 为满足值域要求的二进制位数。
    PrefixXorBasis b(a, m);        // 构造 a 的所有前缀线性基
    // 从空数组开始时，将构造语句改为 PrefixXorBasis b(m)。

    b.insert(x);                   // 在数组末尾追加 x，并保存新的前缀线性基
    ll ans = b.max_xor(l, r);       // 查询 [l, r] 内的最大子集异或和
    ans = b.max_xor(l, r, x);       // 查询 max(x ^ s)，s 遍历 [l, r] 内的所有子集异或值
    bool ok = b.contains(l, r, x);  // 判断 [l, r] 内是否存在子集，其异或和为 x
    int k = b.rank(l, r);           // 查询 [l, r] 的秩，不同子集异或值共有 2^k 个

    // [l, r] 为 1-based 闭区间，需满足 1 <= l <= r <= 已插入元素个数，以上查询均允许空集。
    // 若要求非空子集异或得到 0，判断 b.rank(l, r) < r-l+1，即区间内线性相关。
    // 2^k 是数学表达式；k=63 时不要使用有符号的 1LL << k。
*/
