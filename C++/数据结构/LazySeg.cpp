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

// LC850:矩形面积并（扫描线）
const ll MOD = 1e9 + 7;
class SegmentTree {
private:
    struct Node {
        int l, r;
        int min_cover_len = 0;  // 区间内被覆盖的最小次数
        int min_cover = 0;      // 区间内为最小次数的区间长度
        int todo = 0;           // 懒标记
    };

    vector<Node> seg;

    void maintain(int o) {
        Node& lo = seg[o << 1];
        Node& ro = seg[(o << 1) | 1];
        int mn = min(lo.min_cover, ro.min_cover);
        seg[o].min_cover = mn;
        seg[o].min_cover_len = (lo.min_cover == mn ? lo.min_cover_len : 0) + (ro.min_cover == mn ? ro.min_cover_len : 0);
    }  // 根据左右儿子的信息，更新当前节点的信息

    void do_(int o, int v) {
        seg[o].min_cover += v;
        seg[o].todo += v;
    }  // 仅更新节点信息，不下传懒标记

    void pushdown(int o) {
        int& v = seg[o].todo;
        if (v) {
            do_(o << 1, v);
            do_(o << 1 | 1, v);
            v = 0;
        }
    }  // 下传懒标记

    void build(vi& xs, int o, int l, int r) {
        seg[o].l = l;
        seg[o].r = r;
        if (l == r) {
            seg[o].min_cover_len = xs[l + 1] - xs[l];
            return;
        }
        int m = (l + r) >> 1;
        build(xs, o << 1, l, m);
        build(xs, o << 1 | 1, m + 1, r);
        maintain(o);
        return;
    }

    void update(int o, int l, int r, int v) {
        if (l <= seg[o].l && seg[o].r <= r) {
            do_(o, v);
            return;
        }
        pushdown(o);
        int m = (seg[o].l + seg[o].r) >> 1;
        if (l <= m) update(o << 1, l, r, v);
        if (m < r) update(o << 1 | 1, l, r, v);
        maintain(o);
    }

public:
    SegmentTree(vi& xs) {
        unsigned n = sz(xs) - 1;  // 有这么多个差值
        seg.resize(2 << bit_width(n - 1));
        build(xs, 1, 0, n - 1);  // 根节点是1
    }

    void update(int l, int r, int v) { update(1, l, r, v); }

    int get_uncovered_length() { return seg[1].min_cover ? 0 : seg[1].min_cover_len; }
};
class Solution {
public:
    int rectangleArea(vector<vector<int>>& rectangles) {
        int n = sz(rectangles);
        vi xs;
        struct Event {
            int y, lx, rx, d;
        };
        vector<Event> tem;
        for (auto& p : rectangles) {
            int lx = p[0], rx = p[2], ly = p[1], ry = p[3];
            xs.push_back(lx);
            xs.push_back(rx);
            tem.emplace_back(ly, lx, rx, 1);
            tem.emplace_back(ry, lx, rx, -1);
        }
        ranges::sort(xs);
        xs.erase(unique(all(xs)), xs.end());
        SegmentTree tree(xs);
        sort(all(tem), [&](const Event& x, const Event& y) { return x.y < y.y; });
        int m = sz(tem);
        ll ans = 0;
        rep(i, 0, m - 2) {
            auto& [y, lx, rx, d] = tem[i];
            int l = ranges::lower_bound(xs, lx) - xs.begin();
            int r = ranges::lower_bound(xs, rx) - xs.begin() - 1;  // 注意点和区间的对应关系
            tree.update(l, r, d);
            int sum = xs.back() - xs[0] - tree.get_uncovered_length();
            ans += 1LL * sum * (tem[i + 1].y - y);
            ans %= MOD;
        }
        return ans % MOD;
    }
};