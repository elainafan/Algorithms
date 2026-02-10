template <typename T, typename F>
class LazySegmentTree {
    const F TODO_INIT = 0;  // 懒标记初始值
    struct Node {
        T val;
        F todo;
    };
    int n;
    vector<Node> tree;
    T merge_val(const T& a, const T& b) const { return a + b; }   // 合并两个val
    F merge_todo(const F& a, const F& b) const { return a + b; }  // 合并两个懒标记
    void apply(int node, int l, int r, F todo) {
        Node& cur = tree[node];
        cur.val += todo * (r - l + 1);
        cur.todo = merge_todo(todo, cur.todo);
    }  // 把懒标记作用到node子树
    void pushdown(int node, int l, int r) {
        Node& cur = tree[node];
        F todo = cur.todo;
        if (todo == TODO_INIT) return;
        int m = (l + r) >> 1;
        apply(node << 1, l, m, todo);
        apply(node << 1 | 1, m + 1, r, todo);
        cur.todo = TODO_INIT;
    }  // 把当前节点的懒标记下传
    void maintain(int node) { tree[node].val = merge_val(tree[node << 1].val, tree[node << 1 | 1].val); }
    // 合并线段树
    void build(const vector<T>& a, int node, int l, int r) {
        Node& cur = tree[node];
        cur.todo = TODO_INIT;
        if (l == r) {
            cur.val = a[l];
            return;
        }
        int m = (l + r) >> 1;
        build(a, node << 1, l, m);
        build(a, node << 1 | 1, m + 1, r);
        maintain(node);
    }  // 建树，复杂度O(n)
    void update(int node, int l, int r, int ql, int qr, F f) {
        if (ql <= l && r <= qr) {
            apply(node, l, r, f);
            return;
        }
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        if (ql <= m) update(node << 1, l, m, ql, qr, f);
        if (qr > m) update(node << 1 | 1, m + 1, r, ql, qr, f);
        maintain(node);
    }  // 区间更新[ql,qr]
    T query(int node, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tree[node].val;
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        if (qr <= m) return query(node << 1, l, m, ql, qr);
        if (ql > m) return query(node << 1 | 1, m + 1, r, ql, qr);
        T l_res = query(node << 1, l, m, ql, qr);
        T r_res = query(node << 1 | 1, m + 1, r, ql, qr);
        return merge_val(l_res, r_res);
    }  // 区间查找
    int find_first(int node, int l, int r, int ql, int qr, T val) {
        if (r < ql || l > qr) return -1;
        if (tree[node] < val) return -1;
        if (l == r) return l;
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        int res = find_first(node << 1, l, m, ql, qr, val);
        if (res != -1) return res;
        return find_first(node << 1 | 1, m + 1, r, ql, qr, val);
    }  // 若遇到固定左端点的情况，需要使用全局变量（或者传入引用）记录前缀分段最大值，加一个被待求区间完全覆盖的剪枝

    int find_last(int node, int l, int r, int ql, int qr, T val) {
        if (r < ql || l > qr) return -1;
        if (tree[node] < val) return -1;
        if (l == r) return l;
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        int res = find_last(node << 1 | 1, m + 1, r, ql, qr, val);
        if (res != -1) return res;
        return find_last(node << 1, l, m, ql, qr, val);
    }

public:
    LazySegmentTree(int n, T init_val = 0) : LazySegmentTree(vector<T>(n, init_val)) {}
    // 维护下标为[0,n-1],初始值为init_val的区间，或者数组a
    LazySegmentTree(const vector<T>& a) : n(a.size()), tree(2 << bit_width(a.size() - 1)) { build(a, 1, 0, n - 1); }
    // 更新[ql,qr]为f
    void update(int ql, int qr, F f) { update(1, 0, n - 1, ql, qr, f); }
    // 区间查询[ql,qr]
    T query(int ql, int qr) { return query(1, 0, n - 1, ql, qr); }

    int find_first(int ql, int qr, T val) { return find_first(1, 0, n - 1, ql, qr, val); }  // 查询[ql,qr]中第一个满足条件的下标

    int find_last(int ql, int qr, T val) { return find_last(1, 0, n - 1, ql, qr, val); }  // 查询[ql,qr]中最后一个满足条件的下标
};
// 注：懒标记线段树无论做什么都需要pushdown
// 此时其它与线段树二分同

// 双标记，注意顺序
const int MOD = 1e9 + 7;
template <typename T, typename F>
class LazySegmentTree {
    const F TODO_INIT = 0;  // 懒标记初始值
    const F TODO_INIT2 = 1;
    struct Node {
        T val;
        F todo;
        F todo2;
    };
    int n;
    vector<Node> tree;
    T merge_val(const T& a, const T& b) const { return (a + b) % MOD; }                               // 合并两个val
    F merge_todo(const F& a, const F& b, const F& c) const { return (a * c % MOD + b % MOD) % MOD; }  // 合并两个懒标记
    F merge_todo2(const F& a, const F& b) const { return (a * b) % MOD; }                             // 合并两个乘法标记
    void apply(int node, int l, int r, F todo, F todo2) {
        Node& cur = tree[node];
        cur.val *= todo2;
        cur.val %= MOD;
        cur.val += (todo % MOD) * ((r - l + 1) % MOD) % MOD;
        cur.val %= MOD;
        cur.todo = merge_todo(cur.todo, todo, todo2);
        cur.todo2 = merge_todo2(todo2, cur.todo2);
    }  // 把懒标记作用到node子树
    void pushdown(int node, int l, int r) {
        Node& cur = tree[node];
        F todo = cur.todo;
        F todo2 = cur.todo2;
        if (todo == TODO_INIT && todo2 == TODO_INIT2) return;
        int m = (l + r) >> 1;
        apply(node << 1, l, m, todo, todo2);
        apply(node << 1 | 1, m + 1, r, todo, todo2);
        cur.todo = TODO_INIT;
        cur.todo2 = TODO_INIT2;
    }  // 把当前节点的懒标记下传
    void maintain(int node) { tree[node].val = merge_val(tree[node << 1].val, tree[node << 1 | 1].val); }
    // 合并线段树
    void build(const vector<T>& a, int node, int l, int r) {
        Node& cur = tree[node];
        cur.todo = TODO_INIT;
        cur.todo2 = TODO_INIT2;
        if (l == r) {
            cur.val = a[l];
            return;
        }
        int m = (l + r) >> 1;
        build(a, node << 1, l, m);
        build(a, node << 1 | 1, m + 1, r);
        maintain(node);
    }  // 建树，复杂度O(n)
    void update(int node, int l, int r, int ql, int qr, F f) {
        if (ql <= l && r <= qr) {
            apply(node, l, r, f, 1);
            return;
        }
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        if (ql <= m) update(node << 1, l, m, ql, qr, f);
        if (qr > m) update(node << 1 | 1, m + 1, r, ql, qr, f);
        maintain(node);
    }  // 区间更新[ql,qr]
    void update2(int node, int l, int r, int ql, int qr, F f) {
        if (ql <= l && r <= qr) {
            apply(node, l, r, 0, f);
            return;
        }
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        if (ql <= m) update2(node << 1, l, m, ql, qr, f);
        if (qr > m) update2(node << 1 | 1, m + 1, r, ql, qr, f);
        maintain(node);
    }
    T query(int node, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tree[node].val;
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        if (qr <= m) return query(node << 1, l, m, ql, qr);
        if (ql > m) return query(node << 1 | 1, m + 1, r, ql, qr);
        T l_res = query(node << 1, l, m, ql, qr) % MOD;
        T r_res = query(node << 1 | 1, m + 1, r, ql, qr) % MOD;
        return merge_val(l_res, r_res);
    }  // 区间查找

public:
    LazySegmentTree(int n, T init_val = 0) : LazySegmentTree(vector<T>(n, init_val)) {}
    // 维护下标为[0,n-1],初始值为init_val的区间，或者数组a
    LazySegmentTree(const vector<T>& a) : n(a.size()), tree(2 << bit_width(a.size() - 1)) { build(a, 1, 0, n - 1); }
    // 更新[ql,qr]为f
    void update(int ql, int qr, F f) { update(1, 0, n - 1, ql, qr, f); }
    // 更新[ql,qr]为f
    void update2(int ql, int qr, F f) { update2(1, 0, n - 1, ql, qr, f); }
    // 区间查询[ql,qr]
    T query(int ql, int qr) { return query(1, 0, n - 1, ql, qr); }
};