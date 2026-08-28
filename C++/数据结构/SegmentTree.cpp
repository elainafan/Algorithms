struct Info {
    ll sum, pre, suf, ans;
    Info(ll x = 0) : sum(x), pre(x), suf(x), ans(x) {}
};
Info operator+(const Info& a, const Info& b) {
    Info c;
    c.sum = a.sum + b.sum;
    c.pre = max(a.pre, a.sum + b.pre);
    c.suf = max(b.suf, b.sum + a.suf);
    c.ans = max({a.ans, b.ans, a.suf + b.pre});
    return c;
}
template <typename T>
class SegmentTree {
    int n;
    vector<T> tree;
    T merge_val(T a, T b) const { return a + b; }  // 合并子树

    void maintain(int node) {  // 维护整棵树
        tree[node] = merge_val(tree[node * 2], tree[node * 2 + 1]);
    }

    void build(const vector<T>& a, int node, int l, int r) {
        if (l == r) {
            tree[node] = a[l];
            return;
        }
        int m = (l + r) / 2;
        build(a, node * 2, l, m);
        build(a, node * 2 + 1, m + 1, r);
        maintain(node);
    }  // 建树

    void update(int node, int l, int r, int i, T val) {
        if (l == r) {
            tree[node] = val;
            return;
        }
        int m = (l + r) / 2;
        if (i <= m)
            update(node * 2, l, m, i, val);
        else
            update(node * 2 + 1, m + 1, r, i, val);
        maintain(node);
    }  // 更新i处的值为val

    T query(int node, int l, int r, int ql, int qr) const {
        if (ql <= l && r <= qr) return tree[node];
        int m = (l + r) / 2;
        if (qr <= m) return query(node * 2, l, m, ql, qr);
        if (ql > m) return query(node * 2 + 1, m + 1, r, ql, qr);
        T l_res = query(node * 2, l, m, ql, qr);
        T r_res = query(node * 2 + 1, m + 1, r, ql, qr);
        return merge_val(l_res, r_res);
    }  // 查询[ql,qr]的值

    int find_first(int node, int l, int r, int ql, int qr, T val) const {
        if (r < ql || l > qr) return -1;
        if (tree[node].val < val) return -1;
        if (l == r) return l;
        int m = (l + r) >> 1;
        int res = find_first(node << 1, l, m, ql, qr, val);
        if (res != -1) return res;
        return find_first(node << 1 | 1, m + 1, r, ql, qr, val);
    }
    // 若固定左端点，需要记录前缀分段最大值，并加被待求区间完全覆盖的剪枝

    int find_last(int node, int l, int r, int ql, int qr, T val) const {
        if (r < ql || l > qr) return -1;
        if (tree[node].val < val) return -1;
        if (l == r) return l;
        int m = (l + r) >> 1;
        int res = find_last(node << 1 | 1, m + 1, r, ql, qr, val);
        if (res != -1) return res;
        return find_last(node << 1, l, m, ql, qr, val);
    }

public:
    SegmentTree(int n, T init_val) : SegmentTree(vector<T>(n, init_val)) {}

    // 传入一个数组维护
    SegmentTree(const vector<T>& a)
        : n(a.size()), tree(2 << bit_width(a.size() - 1)) {
        build(a, 1, 0, n - 1);
    }

    void update(int i, T val) { update(1, 0, n - 1, i, val); }  // 更新i的值为val

    T query(int ql, int qr) const { return query(1, 0, n - 1, ql, qr); }  // 查询[ql,qr]的值

    T get(int i) const { return query(1, 0, n - 1, i, i); }  // 取出i处的值

    // 查询[ql,qr]中第一个满足条件的下标
    int find_first(int ql, int qr, T val) const { return find_first(1, 0, n - 1, ql, qr, val); }

    // 查询[ql,qr]中最后一个满足条件的下标
    int find_last(int ql, int qr, T val) const { return find_last(1, 0, n - 1, ql, qr, val); }
};

// 线段树优化建图：原图点编号为 [0, n - 1]，区间均为闭区间 [l, r]
// 支持 u -> [l, r]、[l, r] -> v，每次加边 O(log n)，建图 O(n)
struct SegmentTreeGraph {
    using i64 = long long;

    int n;
    vector<int> out_id, in_id;
    vector<vector<pair<int, i64>>> g;

    SegmentTreeGraph(int n) : n(n), out_id(4 * n), in_id(4 * n), g(n) {
        build_out(1, 0, n - 1);
        build_in(1, 0, n - 1);
    }

    int new_node() {
        g.emplace_back();
        return (int)g.size() - 1;
    }

    void add(int u, int v, i64 w) {
        g[u].emplace_back(v, w);
    }

    // 单点 u 向区间 [l, r] 中的所有点连权值为 w 的边
    void add_out(int u, int l, int r, i64 w) {
        add_out(1, 0, n - 1, l, r, u, w);
    }

    // 区间 [l, r] 中的所有点向单点 v 连权值为 w 的边
    void add_in(int l, int r, int v, i64 w) {
        add_in(1, 0, n - 1, l, r, v, w);
    }

private:
    void build_out(int p, int l, int r) {
        if (l == r) {
            out_id[p] = l;
            return;
        }
        out_id[p] = new_node();
        int m = (l + r) / 2;
        build_out(p * 2, l, m);
        build_out(p * 2 + 1, m + 1, r);
        add(out_id[p], out_id[p * 2], 0);
        add(out_id[p], out_id[p * 2 + 1], 0);
    }

    void build_in(int p, int l, int r) {
        if (l == r) {
            in_id[p] = l;
            return;
        }
        in_id[p] = new_node();
        int m = (l + r) / 2;
        build_in(p * 2, l, m);
        build_in(p * 2 + 1, m + 1, r);
        add(in_id[p * 2], in_id[p], 0);
        add(in_id[p * 2 + 1], in_id[p], 0);
    }

    void add_out(int p, int l, int r, int ql, int qr, int u, i64 w) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            add(u, out_id[p], w);
            return;
        }
        int m = (l + r) / 2;
        add_out(p * 2, l, m, ql, qr, u, w);
        add_out(p * 2 + 1, m + 1, r, ql, qr, u, w);
    }

    void add_in(int p, int l, int r, int ql, int qr, int v, i64 w) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            add(in_id[p], v, w);
            return;
        }
        int m = (l + r) / 2;
        add_in(p * 2, l, m, ql, qr, v, w);
        add_in(p * 2 + 1, m + 1, r, ql, qr, v, w);
    }
};

/*
用法：

int n;
cin >> n;

SegmentTreeGraph graph(n);

// 添加普通单向边 u -> v，边权为 w
graph.add(u, v, w);

// 添加 u -> [l, r]，即 u 向区间内每个点连一条权值为 w 的边
graph.add_out(u, l, r, w);

// 添加 [l, r] -> v，即区间内每个点向 v 连一条权值为 w 的边
graph.add_in(l, r, v, w);

// 建图完成后的邻接表，节点总数为 graph.g.size()
auto& g = graph.g;
// 在 g 上自行运行 Dijkstra、拓扑排序等图算法。
// 原图节点仍是 [0, n - 1]，其余节点均为线段树辅助节点。

原图节点编号为 [0, n - 1]，传入的区间为闭区间 [l, r]。
普通边 O(1)，每次点连区间或区间连点 O(log n)。
*/

// 动态开点线段树
struct Info {
    ll sum, pre, suf, ans;
    Info(ll x = 0) : sum(x), pre(x), suf(x), ans(x) {}
};
Info operator+(const Info& a, const Info& b) {
    Info c;
    c.sum = a.sum + b.sum;
    c.pre = max(a.pre, a.sum + b.pre);
    c.suf = max(b.suf, b.sum + a.suf);
    c.ans = max({a.ans, b.ans, a.suf + b.pre});
    return c;
}

bool operator<(const Info& a, const Info& b) {
    return a.sum < b.sum;
}

template <typename Info>
class DynamicSegmentTree {
    struct Node {
        int left = -1, right = -1;
        Info val = Info();
    };

    int n;
    vector<Node> tree;

    Info merge_val(Info a, Info b) const { return a + b; }  // 合并子树

    int new_node() {
        tree.emplace_back();
        return tree.size() - 1;
    }

    Info get_val(int node) const { return node == -1 ? Info() : tree[node].val; }

    void maintain(int node) { tree[node].val = merge_val(get_val(tree[node].left), get_val(tree[node].right)); }

    void update(int node, int l, int r, int i, Info val) {
        if (l == r) {
            tree[node].val = val;
            return;
        }
        int m = (l + r) / 2;
        if (i <= m) {
            if (tree[node].left == -1) {
                int child = new_node();
                tree[node].left = child;
            }
            update(tree[node].left, l, m, i, val);
        } else {
            if (tree[node].right == -1) {
                int child = new_node();
                tree[node].right = child;
            }
            update(tree[node].right, m + 1, r, i, val);
        }
        maintain(node);
    }  // 更新i处的值为val

    Info query(int node, int l, int r, int ql, int qr) const {
        if (node == -1 || r < ql || l > qr) return Info();
        if (ql <= l && r <= qr) return tree[node].val;
        int m = (l + r) / 2;
        Info l_res = query(tree[node].left, l, m, ql, qr);
        Info r_res = query(tree[node].right, m + 1, r, ql, qr);
        return merge_val(l_res, r_res);
    }  // 查询[ql,qr]的值

    int find_first(int node, int l, int r, int ql, int qr, Info val) const {
        if (r < ql || l > qr || get_val(node) < val) return -1;
        if (l == r) return l;
        int m = (l + r) / 2;
        int left = node == -1 ? -1 : tree[node].left;
        int right = node == -1 ? -1 : tree[node].right;
        int res = find_first(left, l, m, ql, qr, val);
        if (res != -1) return res;
        return find_first(right, m + 1, r, ql, qr, val);
    }

    int find_last(int node, int l, int r, int ql, int qr, Info val) const {
        if (r < ql || l > qr || get_val(node) < val) return -1;
        if (l == r) return l;
        int m = (l + r) / 2;
        int left = node == -1 ? -1 : tree[node].left;
        int right = node == -1 ? -1 : tree[node].right;
        int res = find_last(right, m + 1, r, ql, qr, val);
        if (res != -1) return res;
        return find_last(left, l, m, ql, qr, val);
    }

public:
    DynamicSegmentTree(int n) : n(n), tree(1) {}

    void update(int i, Info val) { update(0, 0, n - 1, i, val); }  // 更新i处的值为val

    Info query(int ql, int qr) const { return query(0, 0, n - 1, ql, qr); }  // 查询[ql,qr]的值

    Info get(int i) const { return query(i, i); }  // 取出i处的值

    // 查询[ql,qr]中第一个满足条件的下标
    int find_first(int ql, int qr, Info val) const { return find_first(0, 0, n - 1, ql, qr, val); }

    // 查询[ql,qr]中最后一个满足条件的下标
    int find_last(int ql, int qr, Info val) const { return find_last(0, 0, n - 1, ql, qr, val); }
};

/*
用法：

DynamicSegmentTree<Info> tree(1'000'000'001); // 维护[0, 1e9]
tree.update(i, Info(val));                    // a[i] = val
ll res = tree.query(l, r).sum;                // 查询[l, r]
ll cur = tree.get(i).sum;                     // 查询a[i]
int first = tree.find_first(l, r, Info(val)); // 第一个满足条件的位置
int last = tree.find_last(l, r, Info(val));   // 最后一个满足条件的位置

每次操作时间复杂度O(log n)，空间复杂度O(修改次数 * log n)。
*/
