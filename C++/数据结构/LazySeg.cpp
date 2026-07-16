struct Tag {
    ll add = 0;  // 懒标记初始值

    Tag(ll add = 0) : add(add) {}

    bool empty() const { return add == 0; }

    void apply(const Tag& t) {
        add += t.add;
    }  // 合并懒标记：先已有操作，再做t
};

struct Info {
    ll sum = 0;

    Info(ll sum = 0) : sum(sum) {}

    void apply(const Tag& t, int l, int r) {
        sum += t.add * (r - l + 1);
    }  // 把懒标记作用到当前节点
};

Info operator+(const Info& a, const Info& b) {
    return {a.sum + b.sum};
}  // 合并两个Info

bool operator<(const Info& a, const Info& b) {
    return a.sum < b.sum;
}  // 线段树二分用，不需要时可删

template <typename Info, typename Tag>
class LazySegmentTree {
    int n;
    vector<Info> info;
    vector<Tag> tag;

    void apply(int node, int l, int r, const Tag& v) {
        info[node].apply(v, l, r);
        tag[node].apply(v);
    }

    void pushdown(int node, int l, int r) {
        if (tag[node].empty()) return;
        int m = (l + r) >> 1;
        apply(node << 1, l, m, tag[node]);
        apply(node << 1 | 1, m + 1, r, tag[node]);
        tag[node] = Tag();
    }  // 把当前节点的懒标记下传

    void maintain(int node) {
        info[node] = info[node << 1] + info[node << 1 | 1];
    }

    void build(const vector<Info>& a, int node, int l, int r) {
        if (l == r) {
            info[node] = a[l];
            return;
        }
        int m = (l + r) >> 1;
        build(a, node << 1, l, m);
        build(a, node << 1 | 1, m + 1, r);
        maintain(node);
    }  // 建树，复杂度O(n)

    void update(int node, int l, int r, int ql, int qr, const Tag& v) {
        if (ql <= l && r <= qr) {
            apply(node, l, r, v);
            return;
        }
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        if (ql <= m) update(node << 1, l, m, ql, qr, v);
        if (qr > m) update(node << 1 | 1, m + 1, r, ql, qr, v);
        maintain(node);
    }  // 区间更新[ql,qr]

    void assign(int node, int l, int r, int p, const Info& v) {
        if (l == r) {
            info[node] = v;
            tag[node] = Tag();
            return;
        }
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        if (p <= m)
            assign(node << 1, l, m, p, v);
        else
            assign(node << 1 | 1, m + 1, r, p, v);
        maintain(node);
    }  // 单点赋值

    Info query(int node, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return info[node];
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        if (qr <= m) return query(node << 1, l, m, ql, qr);
        if (ql > m) return query(node << 1 | 1, m + 1, r, ql, qr);
        return query(node << 1, l, m, ql, qr) + query(node << 1 | 1, m + 1, r, ql, qr);
    }  // 区间查找

    template <typename F>
    int find_first(int node, int l, int r, int ql, int qr, F&& check) {
        if (r < ql || l > qr || !check(info[node])) return -1;
        if (l == r) return l;
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        int res = find_first(node << 1, l, m, ql, qr, check);
        if (res != -1) return res;
        return find_first(node << 1 | 1, m + 1, r, ql, qr, check);
    }  // 若遇到固定左端点的情况，需要使用全局变量（或者传入引用）记录前缀分段最大值，加一个被待求区间完全覆盖的剪枝

    template <typename F>
    int find_last(int node, int l, int r, int ql, int qr, F&& check) {
        if (r < ql || l > qr || !check(info[node])) return -1;
        if (l == r) return l;
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        int res = find_last(node << 1 | 1, m + 1, r, ql, qr, check);
        if (res != -1) return res;
        return find_last(node << 1, l, m, ql, qr, check);
    }

public:
    LazySegmentTree(int n, Info init_val = Info()) : LazySegmentTree(vector<Info>(n, init_val)) {}
    // 维护下标为[0,n-1],初始值为init_val的区间，或者数组a
    LazySegmentTree(const vector<Info>& a) : n(sz(a)), info(2 << bit_width((unsigned)sz(a) - 1)), tag(2 << bit_width((unsigned)sz(a) - 1)) {
        build(a, 1, 0, n - 1);
    }
    // 更新[ql,qr]为f
    void update(int ql, int qr, const Tag& v) { update(1, 0, n - 1, ql, qr, v); }
    // 单点赋值a[p]=v
    void assign(int p, const Info& v) { assign(1, 0, n - 1, p, v); }
    // 区间查询[ql,qr]
    Info query(int ql, int qr) { return query(1, 0, n - 1, ql, qr); }

    template <typename F>
    int find_first(int ql, int qr, F&& check) {
        return find_first(1, 0, n - 1, ql, qr, check);
    }  // 查询[ql,qr]中第一个满足条件的下标

    template <typename F>
    int find_last(int ql, int qr, F&& check) {
        return find_last(1, 0, n - 1, ql, qr, check);
    }  // 查询[ql,qr]中最后一个满足条件的下标

    int find_first(int ql, int qr, const Info& val) {
        return find_first(ql, qr, [&](const Info& x) { return !(x < val); });
    }

    int find_last(int ql, int qr, const Info& val) {
        return find_last(ql, qr, [&](const Info& x) { return !(x < val); });
    }
};
// 注：懒标记线段树无论做什么都需要pushdown
// 此时其它与线段树二分同

// 双标记，注意顺序。使用同一个LazySegmentTree<ModInfo, ModTag>
struct ModTag {
    static constexpr int MOD = 1e9 + 7;
    ll add = 0;
    ll mul = 1;

    ModTag(ll add = 0, ll mul = 1) : add(add), mul(mul) {}

    bool empty() const { return add == 0 && mul == 1; }

    void apply(const ModTag& t) {
        add = (add * t.mul + t.add) % MOD;
        mul = mul * t.mul % MOD;
    }  // 先已有操作，再做t
};

struct ModInfo {
    static constexpr int MOD = 1e9 + 7;
    ll sum = 0;

    ModInfo(ll sum = 0) : sum(sum) {}

    void apply(const ModTag& t, int l, int r) {
        sum = (sum * t.mul + t.add * ((r - l + 1) % MOD)) % MOD;
    }
};

ModInfo operator+(const ModInfo& a, const ModInfo& b) {
    return {(a.sum + b.sum) % ModInfo::MOD};
}

// 区间加：tree.update(l, r, {x, 1})
// 区间乘：tree.update(l, r, {0, x})

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
};// 这个模板用的是左闭右开
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

// 动态开点懒标记线段树
struct Tag {
    ll add = 0;  // 懒标记初始值

    Tag(ll add = 0) : add(add) {}

    bool empty() const { return add == 0; }

    void apply(const Tag& t) {
        add += t.add;
    }  // 合并懒标记：先已有操作，再做t
};

struct Info {
    ll sum = 0;

    Info(ll sum = 0) : sum(sum) {}

    void apply(const Tag& t, int l, int r) {
        sum += t.add * (r - l + 1);
    }  // 把懒标记作用到当前节点
};

Info operator+(const Info& a, const Info& b) {
    return {a.sum + b.sum};
}  // 合并两个Info

bool operator<(const Info& a, const Info& b) {
    return a.sum < b.sum;
}  // 线段树二分用，不需要时可删

template <typename Info, typename Tag>
class DynamicLazySegmentTree {
    struct Node {
        int left = -1, right = -1;
        Info info = Info();
        Tag tag = Tag();
    };

    int n;
    vector<Node> tree;

    int new_node() {
        tree.emplace_back();
        return tree.size() - 1;
    }

    Info get_info(int node) const { return node == -1 ? Info() : tree[node].info; }

    int left_child(int node) {
        if (tree[node].left == -1) {
            int child = new_node();
            tree[node].left = child;
        }
        return tree[node].left;
    }

    int right_child(int node) {
        if (tree[node].right == -1) {
            int child = new_node();
            tree[node].right = child;
        }
        return tree[node].right;
    }

    void apply(int node, int l, int r, const Tag& v) {
        tree[node].info.apply(v, l, r);
        tree[node].tag.apply(v);
    }

    void pushdown(int node, int l, int r) {
        if (tree[node].tag.empty() || l == r) return;
        Tag t = tree[node].tag;
        int m = (l + r) >> 1;
        int left = left_child(node);
        int right = right_child(node);
        apply(left, l, m, t);
        apply(right, m + 1, r, t);
        tree[node].tag = Tag();
    }  // 把当前节点的懒标记下传

    void maintain(int node) {
        tree[node].info = get_info(tree[node].left) + get_info(tree[node].right);
    }

    void update(int node, int l, int r, int ql, int qr, const Tag& v) {
        if (ql <= l && r <= qr) {
            apply(node, l, r, v);
            return;
        }
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        if (ql <= m) update(left_child(node), l, m, ql, qr, v);
        if (qr > m) update(right_child(node), m + 1, r, ql, qr, v);
        maintain(node);
    }  // 区间更新[ql,qr]

    void assign(int node, int l, int r, int p, const Info& v) {
        if (l == r) {
            tree[node].info = v;
            tree[node].tag = Tag();
            return;
        }
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        if (p <= m)
            assign(left_child(node), l, m, p, v);
        else
            assign(right_child(node), m + 1, r, p, v);
        maintain(node);
    }  // 单点赋值

    Info query(int node, int l, int r, int ql, int qr) {
        if (node == -1) return Info();
        if (ql <= l && r <= qr) return tree[node].info;
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        if (qr <= m) return query(tree[node].left, l, m, ql, qr);
        if (ql > m) return query(tree[node].right, m + 1, r, ql, qr);
        return query(tree[node].left, l, m, ql, qr) + query(tree[node].right, m + 1, r, ql, qr);
    }  // 区间查询

    template <typename F>
    int find_first(int node, int l, int r, int ql, int qr, F&& check) {
        if (r < ql || l > qr || !check(get_info(node))) return -1;
        if (l == r) return l;
        if (node != -1) pushdown(node, l, r);
        int m = (l + r) >> 1;
        int left = node == -1 ? -1 : tree[node].left;
        int right = node == -1 ? -1 : tree[node].right;
        int res = find_first(left, l, m, ql, qr, check);
        if (res != -1) return res;
        return find_first(right, m + 1, r, ql, qr, check);
    }

    template <typename F>
    int find_last(int node, int l, int r, int ql, int qr, F&& check) {
        if (r < ql || l > qr || !check(get_info(node))) return -1;
        if (l == r) return l;
        if (node != -1) pushdown(node, l, r);
        int m = (l + r) >> 1;
        int left = node == -1 ? -1 : tree[node].left;
        int right = node == -1 ? -1 : tree[node].right;
        int res = find_last(right, m + 1, r, ql, qr, check);
        if (res != -1) return res;
        return find_last(left, l, m, ql, qr, check);
    }

public:
    DynamicLazySegmentTree(int n) : n(n), tree(1) {}

    // 更新[ql,qr]
    void update(int ql, int qr, const Tag& v) { update(0, 0, n - 1, ql, qr, v); }

    // 单点赋值a[p]=v
    void assign(int p, const Info& v) { assign(0, 0, n - 1, p, v); }

    // 区间查询[ql,qr]
    Info query(int ql, int qr) { return query(0, 0, n - 1, ql, qr); }

    template <typename F>
    int find_first(int ql, int qr, const F& check) {
        return find_first(0, 0, n - 1, ql, qr, check);
    }  // 查询[ql,qr]中第一个满足条件的下标

    template <typename F>
    int find_last(int ql, int qr, const F& check) {
        return find_last(0, 0, n - 1, ql, qr, check);
    }  // 查询[ql,qr]中最后一个满足条件的下标

    int find_first(int ql, int qr, const Info& val) {
        return find_first(ql, qr, [&](const Info& x) { return !(x < val); });
    }

    int find_last(int ql, int qr, const Info& val) {
        return find_last(ql, qr, [&](const Info& x) { return !(x < val); });
    }
};

/*
用法：

DynamicLazySegmentTree<Info, Tag> tree(1'000'000'001); // 维护[0, 1e9]
tree.update(l, r, Tag(add));                           // 区间修改
tree.assign(i, Info(val));                             // 单点赋值
ll res = tree.query(l, r).sum;                         // 区间查询
int first = tree.find_first(l, r, Info(val));          // 第一个满足条件的位置
int last = tree.find_last(l, r, Info(val));            // 最后一个满足条件的位置

每次操作时间复杂度O(log n)，空间复杂度O(操作次数 * log n)。
*/
