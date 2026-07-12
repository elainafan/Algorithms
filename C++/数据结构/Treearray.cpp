template <typename T = long long>
class Tree {
    vector<T> tree;

public:
    // 构造函数：初始化大小为 n 的树状数组，初始所有元素值为 val（外部表现为 0-based）
    Tree(int n, T val = 0) : tree(n + 1) {
        for (int i = 1; i <= n; i++) {
            tree[i] += val;
            int nxt = i + (i & -i);
            if (nxt <= n) {
                tree[nxt] += tree[i];
            }
        }
    }

    // 构造函数：使用给定的 vector 在 O(N) 时间内快速初始化建树
    Tree(const vector<T>& data) {
        int n = data.size();
        tree.resize(n + 1);
        for (int i = 1; i <= n; i++) {
            tree[i] += data[i - 1];  // data是 0-based
            int nxt = i + (i & -i);
            if (nxt <= n) {
                tree[nxt] += tree[i];
            }
        }
    }

    // 单点修改：将 0-based 下标 i 处的元素增加 val
    void add(int i, T val = 1) {
        for (++i; i < tree.size(); i += i & (-i)) {
            tree[i] += val;
        }
    }

    // 前缀求和：计算 0-based 下标区间 [0, i] 内的所有元素之和
    T pre(int i) const {
        T res = 0;
        for (++i; i > 0; i &= i - 1) {
            res += tree[i];
        }
        return res;
    }

    // 区间求和：计算 0-based 下标区间 [l, r] 内的所有元素之和
    T query(int l, int r) const {
        if (r < l) {
            return 0;
        }
        return pre(r) - pre(l - 1);  // 当 l=0 时, pre(-1) 会合理地返回 0
    }

    // 树上二分查找：返回满足前缀和 >= val 的最小 0-based 下标
    int lower_bound(T val) const {
        int w = bit_width(tree.size() - 1);
        int res = 0;
        T s = 0;
        for (int i = w - 1; i >= 0; i--) {
            int nxt = res + (1 << i);
            if (nxt < tree.size() && tree[nxt] + s < val) {
                res += (1 << i);
                s += tree[nxt];
            }
        }
        return res;  // 返回 0-based 下标：内部 1-based 下标为 res + 1，因此 0-based 为 res
    }
};

template <typename T = long long>
class MaxTree {
    vector<T> tree;
    T identity;

public:
    // 构造函数：初始化大小为 n 的树状数组（外部表现为 0-based）
    explicit MaxTree(int n, T identity = numeric_limits<T>::lowest())
        : tree(n + 1, identity), identity(identity) {}

    // 构造函数：使用给定的 vector 在 O(N) 时间内快速初始化建树
    explicit MaxTree(const vector<T>& data, T identity = numeric_limits<T>::lowest())
        : tree(data.size() + 1, identity), identity(identity) {
        int n = data.size();
        for (int i = 1; i <= n; i++) {
            tree[i] = max(tree[i], data[i - 1]);  // data 是 0-based
            int nxt = i + (i & -i);
            if (nxt <= n) {
                tree[nxt] = max(tree[nxt], tree[i]);
            }
        }
    }

    // 单点修改：将 0-based 下标 i 处的值与 val 取最大值（只支持值变大）
    void chmax(int i, T val) {
        for (++i; i < tree.size(); i += i & -i) {
            tree[i] = max(tree[i], val);
        }
    }

    // 前缀最大值：计算 0-based 下标区间 [0, i] 内的最大值
    T pre(int i) const {
        T res = identity;
        for (++i; i > 0; i &= i - 1) {
            res = max(res, tree[i]);
        }
        return res;
    }
};
