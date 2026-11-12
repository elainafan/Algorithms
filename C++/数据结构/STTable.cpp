// ST 表：静态区间查询，0-based 闭区间；预处理和空间 O(n log n)，查询 O(1)。
// 默认维护最大值；merge_val 需满足结合律和幂等性，即合并重复信息不影响结果。
template <typename T = ll>
class STTable {
    vector<vector<T>> st;

    T merge_val(const T& a, const T& b) const {
        return max(a, b);
    }  // 可改为 min(a,b)、gcd(a,b)、a&b、a|b；不能直接改为求和或异或

public:
    STTable(const vector<T>& a) {
        int n = sz(a);
        int lg = bit_width(unsigned(n));
        st.resize(lg);
        if (!n) return;
        st[0] = a;
        rep(j, 1, lg - 1) {
            int len = 1 << j;
            st[j].resize(n - len + 1);
            rep(i, 0, n - len) {
                st[j][i] = merge_val(st[j - 1][i], st[j - 1][i + (len >> 1)]);
            }
        }
    }  // st[j][i] 维护 [i,i+2^j-1]

    T query(int l, int r) const {
        int k = bit_width(unsigned(r - l + 1)) - 1;
        return merge_val(st[k][l], st[k][r - (1 << k) + 1]);
    }  // 要求 0 <= l <= r < n；用两段长度为 2^k 的区间覆盖 [l,r]，允许重叠
};

/*
用法：

STTable<ll> st(a);          // 从 0-based 数组 a 建表
ll ans = st.query(l, r);    // 查询闭区间 [l,r] 的最大值
ll val = st.query(i, i);    // 查询 a[i]

修改 merge_val 后可维护其他满足条件的运算；建表后不支持修改元素。
*/
