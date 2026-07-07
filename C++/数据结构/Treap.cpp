/*
FHQ Treap / 无旋 Treap，维护有序可重集合。

Treap tr;
tr.insert(x);       // 插入x
tr.erase(x);        // 删除一个x，没有则不删
tr.rank(x);         // x的排名：小于x的个数+1
tr.kth(k);          // 第k小，k从1开始
tr.pre(x);          // 严格小于x的最大值，不存在返回-INF
tr.nxt(x);          // 严格大于x的最小值，不存在返回INF
tr.count(x);        // x出现次数
tr.size();          // 总元素个数，重复元素重复算
*/

struct Treap {
    static constexpr ll INF = (1LL << 60);

    struct Node {
        int l = 0, r = 0;
        int siz = 1, cnt = 1;
        ll val = 0;
        unsigned pri = 0;
    };

    vector<Node> tr;
    int root = 0;
    unsigned seed = chrono::steady_clock::now().time_since_epoch().count();

    Treap() : tr(1) {}

    unsigned rng() {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        return seed;
    }

    int new_node(ll x) {
        tr.push_back(Node());
        int p = sz(tr) - 1;
        tr[p].val = x;
        tr[p].pri = rng();
        return p;
    }  // 新建值为x的节点

    int size(int p) const {
        return p ? tr[p].siz : 0;
    }

    int size() const {
        return size(root);
    }

    void maintain(int p) {
        if (p) tr[p].siz = size(tr[p].l) + size(tr[p].r) + tr[p].cnt;
    }  // 维护子树大小

    void split_less(int p, ll x, int& a, int& b) {
        if (!p) {
            a = b = 0;
            return;
        }
        if (tr[p].val < x) {
            a = p;
            split_less(tr[p].r, x, tr[p].r, b);
            maintain(a);
        } else {
            b = p;
            split_less(tr[p].l, x, a, tr[p].l);
            maintain(b);
        }
    }  // a中都<x，b中都>=x

    void split_leq(int p, ll x, int& a, int& b) {
        if (!p) {
            a = b = 0;
            return;
        }
        if (tr[p].val <= x) {
            a = p;
            split_leq(tr[p].r, x, tr[p].r, b);
            maintain(a);
        } else {
            b = p;
            split_leq(tr[p].l, x, a, tr[p].l);
            maintain(b);
        }
    }  // a中都<=x，b中都>x

    int merge(int a, int b) {
        if (!a || !b) return a | b;
        if (tr[a].pri < tr[b].pri) {
            tr[a].r = merge(tr[a].r, b);
            maintain(a);
            return a;
        } else {
            tr[b].l = merge(a, tr[b].l);
            maintain(b);
            return b;
        }
    }  // 合并两棵树，要求a中的值都<=b中的值

    void insert(ll x) {
        int a, b, c, d;
        split_less(root, x, a, b);
        split_leq(b, x, c, d);
        if (c) {
            tr[c].cnt++;
            maintain(c);
        } else {
            c = new_node(x);
        }
        root = merge(a, merge(c, d));
    }  // 插入x

    void erase(ll x) {
        int a, b, c, d;
        split_less(root, x, a, b);
        split_leq(b, x, c, d);
        if (c) {
            if (tr[c].cnt > 1) {
                tr[c].cnt--;
                maintain(c);
            } else {
                c = merge(tr[c].l, tr[c].r);
            }
        }
        root = merge(a, merge(c, d));
    }  // 删除一个x

    int count(ll x) const {
        int p = root;
        while (p) {
            if (tr[p].val == x) return tr[p].cnt;
            p = x < tr[p].val ? tr[p].l : tr[p].r;
        }
        return 0;
    }  // 查询x出现次数

    int rank(ll x) const {
        int p = root, ans = 1;
        while (p) {
            if (x <= tr[p].val) {
                p = tr[p].l;
            } else {
                ans += size(tr[p].l) + tr[p].cnt;
                p = tr[p].r;
            }
        }
        return ans;
    }  // 查询x的排名

    ll kth(int k) const {
        int p = root;
        while (p) {
            if (k <= size(tr[p].l)) {
                p = tr[p].l;
            } else if (k <= size(tr[p].l) + tr[p].cnt) {
                return tr[p].val;
            } else {
                k -= size(tr[p].l) + tr[p].cnt;
                p = tr[p].r;
            }
        }
        return 0;
    }  // 查询第k小，保证1<=k<=size()

    ll pre(ll x) const {
        int p = root;
        ll ans = -INF;
        while (p) {
            if (tr[p].val < x) {
                ans = tr[p].val;
                p = tr[p].r;
            } else {
                p = tr[p].l;
            }
        }
        return ans;
    }  // 查询前驱

    ll nxt(ll x) const {
        int p = root;
        ll ans = INF;
        while (p) {
            if (tr[p].val > x) {
                ans = tr[p].val;
                p = tr[p].l;
            } else {
                p = tr[p].r;
            }
        }
        return ans;
    }  // 查询后继
};

/*
P3369用法：

Treap tr;
while (q--) {
    int op;
    ll x;
    cin >> op >> x;
    if (op == 1) tr.insert(x);
    if (op == 2) tr.erase(x);
    if (op == 3) cout << tr.rank(x) << endl;
    if (op == 4) cout << tr.kth(x) << endl;
    if (op == 5) cout << tr.pre(x) << endl;
    if (op == 6) cout << tr.nxt(x) << endl;
}
*/
