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

public:
    LazySegmentTree(int n, T init_val = 0) : LazySegmentTree(vector<T>(n, init_val)) {}
    // 维护下标为[0,n-1],初始值为init_val的区间，或者数组a
    LazySegmentTree(const vector<T>& a) : n(a.size()), tree(2 << bit_width(a.size() - 1)) { build(a, 1, 0, n - 1); }
    // 更新[ql,qr]为f
    void update(int ql, int qr, F f) { update(1, 0, n - 1, ql, qr, f); }
    // 区间查询[ql,qr]
    T query(int ql, int qr) { return query(1, 0, n - 1, ql, qr); }
};
// 注：懒标记线段树无论做什么都需要pushdown
// 此时其它与线段树二分同