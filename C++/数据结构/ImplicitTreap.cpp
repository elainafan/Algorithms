// 隐式FHQ Treap / 无旋Treap维护序列，0-based。

struct ImplicitTreap {
    static constexpr ll INF = (1LL << 60);

    struct Info {
        int siz = 0;
        ll sum = 0, mn = INF, mx = -INF;
    };

    struct Node {
        int l = 0, r = 0;
        int siz = 0;
        ll val = 0, sum = 0, mn = INF, mx = -INF;
        ll add = 0, cover_val = 0;
        bool rev = false, cover = false;
        unsigned pri = 0;
    };

    vector<Node> tr;
    int root = 0;
    unsigned seed = chrono::steady_clock::now().time_since_epoch().count();

    ImplicitTreap() : tr(1) {}
    ImplicitTreap(const vl& a) : ImplicitTreap() { build(a); }

    unsigned rng() {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        return seed;
    }

    int size(int p) const {
        return p ? tr[p].siz : 0;
    }

    int size() const {
        return size(root);
    }

    int new_node(ll x) {
        tr.push_back(Node());
        int p = sz(tr) - 1;
        tr[p].siz = 1;
        tr[p].val = tr[p].sum = tr[p].mn = tr[p].mx = x;
        tr[p].pri = rng();
        return p;
    }  // 新建值为x的节点

    void maintain(int p) {
        if (!p) return;
        tr[p].siz = size(tr[p].l) + size(tr[p].r) + 1;
        tr[p].sum = tr[tr[p].l].sum + tr[p].val + tr[tr[p].r].sum;
        tr[p].mn = min({tr[tr[p].l].mn, tr[p].val, tr[tr[p].r].mn});
        tr[p].mx = max({tr[tr[p].l].mx, tr[p].val, tr[tr[p].r].mx});
    }  // 维护子树信息

    void apply_rev(int p) {
        if (!p) return;
        swap(tr[p].l, tr[p].r);
        tr[p].rev ^= 1;
    }  // 区间翻转标记

    void apply_add(int p, ll x) {
        if (!p) return;
        tr[p].val += x;
        tr[p].sum += x * tr[p].siz;
        tr[p].mn += x;
        tr[p].mx += x;
        if (tr[p].cover)
            tr[p].cover_val += x;
        else
            tr[p].add += x;
    }  // 区间加标记

    void apply_assign(int p, ll x) {
        if (!p) return;
        tr[p].val = x;
        tr[p].sum = x * tr[p].siz;
        tr[p].mn = tr[p].mx = x;
        tr[p].add = 0;
        tr[p].cover = true;
        tr[p].cover_val = x;
    }  // 区间赋值标记

    void pushdown(int p) {
        if (!p) return;
        if (tr[p].cover) {
            apply_assign(tr[p].l, tr[p].cover_val);
            apply_assign(tr[p].r, tr[p].cover_val);
            tr[p].cover = false;
        }
        if (tr[p].add) {
            apply_add(tr[p].l, tr[p].add);
            apply_add(tr[p].r, tr[p].add);
            tr[p].add = 0;
        }
        if (tr[p].rev) {
            apply_rev(tr[p].l);
            apply_rev(tr[p].r);
            tr[p].rev = false;
        }
    }  // 下传懒标记

    int merge(int a, int b) {
        if (!a || !b) return a | b;
        if (tr[a].pri < tr[b].pri) {
            pushdown(a);
            tr[a].r = merge(tr[a].r, b);
            maintain(a);
            return a;
        } else {
            pushdown(b);
            tr[b].l = merge(a, tr[b].l);
            maintain(b);
            return b;
        }
    }  // 拼接两段序列，a在前，b在后

    void split(int p, int k, int& a, int& b) {
        if (!p) {
            a = b = 0;
            return;
        }
        pushdown(p);
        if (size(tr[p].l) >= k) {
            b = p;
            split(tr[p].l, k, a, tr[p].l);
            maintain(b);
        } else {
            a = p;
            split(tr[p].r, k - size(tr[p].l) - 1, tr[p].r, b);
            maintain(a);
        }
    }  // 分裂出前k个元素

    void split_range(int l, int r, int& a, int& b, int& c) {
        split(root, r + 1, a, c);
        split(a, l, a, b);
    }  // b为区间[l,r]

    int build_tree(const vl& a) {
        int p = 0;
        for (ll x : a) p = merge(p, new_node(x));
        return p;
    }  // O(nlogn)建树，够短好改

    void build(const vl& a) {
        tr.assign(1, Node());
        root = 0;
        for (ll x : a) root = merge(root, new_node(x));
    }  // 重新建树

    void insert(int pos, ll x) {
        int a, b;
        split(root, pos, a, b);
        root = merge(merge(a, new_node(x)), b);
    }  // 插到下标pos前面

    void insert(int pos, const vl& a) {
        insert_tree(pos, build_tree(a));
    }  // 插入一段序列

    void insert_tree(int pos, int mid) {
        int a, b;
        split(root, pos, a, b);
        root = merge(merge(a, mid), b);
    }  // 拼接一棵子树

    int cut(int l, int r) {
        int a, b, c;
        split_range(l, r, a, b, c);
        root = merge(a, c);
        return b;
    }  // 截取区间[l,r]

    void erase(int l, int r) {
        cut(l, r);
    }  // 删除区间[l,r]

    void reverse(int l, int r) {
        int a, b, c;
        split_range(l, r, a, b, c);
        apply_rev(b);
        root = merge(merge(a, b), c);
    }  // 翻转区间[l,r]

    void add(int l, int r, ll x) {
        int a, b, c;
        split_range(l, r, a, b, c);
        apply_add(b, x);
        root = merge(merge(a, b), c);
    }  // 区间加x

    void assign(int l, int r, ll x) {
        int a, b, c;
        split_range(l, r, a, b, c);
        apply_assign(b, x);
        root = merge(merge(a, b), c);
    }  // 区间赋值为x

    Info query(int l, int r) {
        int a, b, c;
        split_range(l, r, a, b, c);
        Info res{size(b), tr[b].sum, tr[b].mn, tr[b].mx};
        root = merge(merge(a, b), c);
        return res;
    }  // 查询区间信息

    ll kth(int k) {
        k++;
        int p = root;
        while (p) {
            pushdown(p);
            if (k <= size(tr[p].l)) {
                p = tr[p].l;
            } else if (k == size(tr[p].l) + 1) {
                return tr[p].val;
            } else {
                k -= size(tr[p].l) + 1;
                p = tr[p].r;
            }
        }
        return 0;
    }  // 查询a[k]，保证0<=k<size()

    void dfs(int p, vl& res) {
        if (!p) return;
        pushdown(p);
        dfs(tr[p].l, res);
        res.push_back(tr[p].val);
        dfs(tr[p].r, res);
    }

    vl to_vector() {
        vl res;
        dfs(root, res);
        return res;
    }  // 调试用，导出整段序列
};

/*
用法：

下标均为0-indexed，区间为闭区间[l,r]。
插入位置pos表示插到下标pos前面，pos=size()表示插到末尾。

ImplicitTreap tr(a);
tr.kth(pos);              // 查询a[pos]
tr.insert(pos, x);        // 插入单个数
tr.insert(pos, b);        // 插入一段序列
tr.erase(l, r);           // 删除区间[l,r]
tr.reverse(l, r);         // 翻转区间[l,r]
tr.add(l, r, x);          // 区间加x
tr.assign(l, r, x);       // 区间赋值为x
tr.query(l, r);           // 返回区间Info: siz,sum,mn,mx
int mid = tr.cut(l, r);   // 截取区间[l,r]，原树中删除这一段，返回这一段的根
tr.insert_tree(pos, mid); // 把mid插到下标pos前面
*/
