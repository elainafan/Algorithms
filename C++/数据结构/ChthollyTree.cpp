struct Info {
    ll val = 0;

    Info(ll val = 0) : val(val) {}

    void apply(ll add) {
        val += add;
    }  // 区间修改，根据题意更改
};

bool operator==(const Info& a, const Info& b) {
    return a.val == b.val;
}  // 合并值相同的相邻区间

bool operator<(const Info& a, const Info& b) {
    return a.val < b.val;
}  // kth排序用，不需要时可删

template <typename Info>
class ChthollyTree {
    struct Node {
        int l;
        mutable int r;
        mutable Info info;

        Node(int l, int r, const Info& info) : l(l), r(r), info(info) {}

        bool operator<(const Node& other) const { return l < other.l; }
    };

    int n;
    set<Node> tree;
    using iterator = typename set<Node>::iterator;

    iterator split(int p) {
        if (p == n) return tree.end();
        auto it = prev(tree.upper_bound(Node(p, p, Info())));
        if (it->l == p) return it;
        int l = it->l, r = it->r;
        Info info = it->info;
        tree.erase(it);
        tree.emplace(l, p - 1, info);
        return tree.emplace(p, r, info).first;
    }

    iterator merge(iterator it) {
        if (it != tree.begin()) {
            auto pre = prev(it);
            if (pre->r + 1 == it->l && pre->info == it->info) {
                pre->r = it->r;
                tree.erase(it);
                it = pre;
            }
        }
        auto nxt = next(it);
        if (nxt != tree.end() && it->r + 1 == nxt->l && it->info == nxt->info) {
            it->r = nxt->r;
            tree.erase(nxt);
        }
        return it;
    }

    void merge_range(int l, int r) {
        auto it = tree.lower_bound(Node(l, l, Info()));
        if (it != tree.begin()) it--;
        while (it != tree.end()) {
            auto nxt = next(it);
            if (nxt == tree.end() || nxt->l > r + 1) break;
            if (it->r + 1 == nxt->l && it->info == nxt->info) {
                it->r = nxt->r;
                tree.erase(nxt);
            } else {
                it = nxt;
            }
        }
    }

public:
    ChthollyTree(int n, const Info& init_val = Info()) : n(n) {
        tree.emplace(0, n - 1, init_val);
    }

    ChthollyTree(const vector<Info>& a) : n(a.size()) {
        for (int l = 0, r; l < n; l = r + 1) {
            r = l;
            while (r + 1 < n && a[r + 1] == a[l]) r++;
            tree.emplace(l, r, a[l]);
        }
    }

    // 将区间[ql,qr]全部赋值为val
    void assign(int ql, int qr, const Info& val) {
        auto itr = split(qr + 1);
        auto itl = split(ql);
        tree.erase(itl, itr);
        merge(tree.emplace(ql, qr, val).first);
    }

    // 对区间[ql,qr]内每一种Info执行f(info)
    template <typename F>
    void update(int ql, int qr, F&& f) {
        auto itr = split(qr + 1);
        auto itl = split(ql);
        for (auto it = itl; it != itr; it++) f(it->info);
        merge_range(ql, qr);
    }

    // 对区间[ql,qr]内每个值相同的子段执行f(l,r,info)
    template <typename F>
    void query(int ql, int qr, F&& f) {
        auto itr = split(qr + 1);
        auto itl = split(ql);
        for (auto it = itl; it != itr; it++) f(it->l, it->r, it->info);
        merge_range(ql, qr);
    }

    Info get(int p) const {
        auto it = prev(tree.upper_bound(Node(p, p, Info())));
        return it->info;
    }

    // 查询区间[ql,qr]中第k小的Info，k从1开始
    Info kth(int ql, int qr, int k) {
        assert(1 <= k && k <= qr - ql + 1);
        vector<pair<Info, int>> a;
        auto itr = split(qr + 1);
        auto itl = split(ql);
        for (auto it = itl; it != itr; it++) {
            a.emplace_back(it->info, it->r - it->l + 1);
        }
        ranges::sort(a, [](const auto& x, const auto& y) { return x.first < y.first; });
        Info res;
        for (auto& [info, cnt] : a) {
            if (k <= cnt) {
                res = info;
                break;
            }
            k -= cnt;
        }
        merge_range(ql, qr);
        return res;
    }

    int size() const { return (int)tree.size(); }  // 当前区间数量
};

/*
用法：

ChthollyTree<Info> tree(n, Info(0));
tree.assign(l, r, Info(x)); // 区间推平

tree.update(l, r, [&](Info& info) {
    info.apply(add);
}); // 区间修改

ll sum = 0;
tree.query(l, r, [&](int l, int r, const Info& info) {
    sum += info.val * (r - l + 1);
}); // 区间查询

ll cur = tree.get(i).val;
ll kth = tree.kth(l, r, k).val;

复杂度与区间数量有关，适用于含有大量区间推平且数据随机的题目。
*/
