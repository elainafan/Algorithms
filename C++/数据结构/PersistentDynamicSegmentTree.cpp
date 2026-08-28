// 可持久化动态开点线段树：大值域上保留历史版本，支持从任意版本单点赋值和区间查询。
// 下标为 0-indexed，维护 [0,n-1]；版本 0 全为 Info()，不预先建树。

struct Info {
    ll sum = 0;

    Info(ll x = 0) : sum(x) {}
};

Info operator+(const Info& a, const Info& b) {
    return {a.sum + b.sum};
}  // Info() 必须是 operator+ 的左右单位元

template <typename Info>
class PersistentDynamicSegmentTree {
    struct Node {
        int left, right;
        Info val;
    };

    ll n;
    vector<Node> tree;
    vi root;

    Info merge_val(const Info& a, const Info& b) const {
        return a + b;
    }

    int new_node(int left, int right, const Info& val) {
        tree.push_back({left, right, val});
        return sz(tree) - 1;
    }

    int update(int old, ll l, ll r, ll p, const Info& val) {
        if (l == r) return new_node(0, 0, val);
        ll m = l + (r - l) / 2;
        int left = tree[old].left;
        int right = tree[old].right;
        if (p <= m)
            left = update(left, l, m, p, val);
        else
            right = update(right, m + 1, r, p, val);
        return new_node(left, right, merge_val(tree[left].val, tree[right].val));
    }  // 只复制根到 p 的一条链

    Info query(int node, ll l, ll r, ll ql, ll qr) const {
        if (!node) return Info();
        if (ql <= l && r <= qr) return tree[node].val;
        ll m = l + (r - l) / 2;
        if (qr <= m) return query(tree[node].left, l, m, ql, qr);
        if (ql > m) return query(tree[node].right, m + 1, r, ql, qr);
        return merge_val(query(tree[node].left, l, m, ql, qr),
                         query(tree[node].right, m + 1, r, ql, qr));
    }

    template <typename F>
    ll find_first(int node, ll l, ll r, ll ql, ll qr, F& check) const {
        if (r < ql || qr < l || !check(tree[node].val)) return -1;
        if (l == r) return l;
        ll m = l + (r - l) / 2;
        ll res = find_first(tree[node].left, l, m, ql, qr, check);
        if (res != -1) return res;
        return find_first(tree[node].right, m + 1, r, ql, qr, check);
    }

    template <typename F>
    ll find_last(int node, ll l, ll r, ll ql, ll qr, F& check) const {
        if (r < ql || qr < l || !check(tree[node].val)) return -1;
        if (l == r) return l;
        ll m = l + (r - l) / 2;
        ll res = find_last(tree[node].right, m + 1, r, ql, qr, check);
        if (res != -1) return res;
        return find_last(tree[node].left, l, m, ql, qr, check);
    }

    int nodes_per_update() const {
        return bit_width((ull)n - 1) + 1;
    }

public:
    // n > 0；max_updates >= 0，只用于一次性预留内存。
    PersistentDynamicSegmentTree(ll size, int max_updates = 0) : n(size) {
        tree.reserve(1 + (size_t)max_updates * nodes_per_update());
        root.reserve((size_t)max_updates + 1);
        tree.push_back({0, 0, Info()});
        root.push_back(0);
    }

    // 从 version 版本分叉，令 a[p] = val，返回新版本编号。
    int update(int version, ll p, const Info& val) {
        root.push_back(update(root[version], 0, n - 1, p, val));
        return versions() - 1;
    }

    int update(ll p, const Info& val) {
        return update(latest_version(), p, val);
    }  // 从最新版本赋值

    int copy_version(int version) {
        int old_root = root[version];
        root.push_back(old_root);
        return versions() - 1;
    }  // 均摊 O(1) 复制版本，不新建线段树节点

    Info query(int version, ll ql, ll qr) const {
        return query(root[version], 0, n - 1, ql, qr);
    }

    Info query(ll ql, ll qr) const {
        return query(latest_version(), ql, qr);
    }  // 查询最新版本的 [ql,qr]

    Info get(int version, ll p) const {
        return query(version, p, p);
    }

    Info get(ll p) const {
        return get(latest_version(), p);
    }

    Info query_all(int version) const {
        return tree[root[version]].val;
    }  // O(1) 查询整个值域

    Info query_all() const {
        return query_all(latest_version());
    }

    template <typename F>
    ll find_first(int version, ll ql, ll qr, F&& check) const {
        return find_first(root[version], 0, n - 1, ql, qr, check);
    }  // 查询 [ql,qr] 中第一个令叶子 Info 通过 check 的位置

    template <typename F>
    ll find_first(ll ql, ll qr, F&& check) const {
        return find_first(latest_version(), ql, qr, check);
    }

    template <typename F>
    ll find_last(int version, ll ql, ll qr, F&& check) const {
        return find_last(root[version], 0, n - 1, ql, qr, check);
    }  // 查询 [ql,qr] 中最后一个令叶子 Info 通过 check 的位置

    template <typename F>
    ll find_last(ll ql, ll qr, F&& check) const {
        return find_last(latest_version(), ql, qr, check);
    }

    int versions() const {
        return sz(root);
    }

    int latest_version() const {
        return versions() - 1;
    }

    void reserve_updates(int max_updates) {
        tree.reserve(tree.size() + (size_t)max_updates * nodes_per_update());
        root.reserve(root.size() + (size_t)max_updates);
    }  // max_updates >= 0，为后续修改预留内存
};

/*
用法：

PersistentDynamicSegmentTree<Info> tree(1'000'000'000'000'000'001LL, q); // 维护 [0,1e18]

int v1 = tree.update(0, p, Info(x));   // 从全为 Info() 的 0 号版本修改
int v2 = tree.update(v1, p2, Info(y)); // 从 v1 继续修改
int v3 = tree.update(0, p3, Info(z));  // 从 0 号版本分出另一支
int v4 = tree.update(p4, Info(w));     // 从当前最新版本 v3 修改
int v5 = tree.copy_version(v2);        // 复制 v2，不新建节点

ll res = tree.query(v2, l, r).sum;     // v2 的区间和
ll cur = tree.get(v1, p).sum;          // v1 的 a[p]
ll total = tree.query_all(v3).sum;     // v3 的整个值域信息

不传 version 的 update/query/get/query_all/find_first/find_last 默认操作最新版本。
find_first/find_last 的 check 必须能根据整段 Info 判断该段是否可能含有答案。
例如 Info 维护区间最大值 mx 时：
ll first = tree.find_first(v2, l, r, [&](const Info& x) { return x.mx >= need; });
ll last = tree.find_last(v2, l, r, [&](const Info& x) { return x.mx >= need; });
若 check(Info()) 成立，未开点位置也可以成为答案；剪枝过弱时最坏会遍历整个待查区间。

operator+ 需满足结合律，Info() 必须同时是它的左、右单位元，才能代表整段未开点区间。
所有接口均不做边界检查：需保证 n > 0、0 <= p < n、0 <= l <= r < n、
0 <= version < tree.versions()、max_updates >= 0。旧版本永远不会被 update 修改。

初始化 O(1)，单次修改/区间查询 O(log n)，query_all O(1)，copy_version 均摊 O(1)。
每次修改最多新建 bit_width(n-1)+1 个节点；即使 val==Info()，也会新建这条链，
因为通用模板不强制 Info 实现 operator==。Node 只保留两个 32 位儿子下标和一份 Info。
节点下标、节点数和版本数需能存入 int。
设修改数为 U、纯复制版本数为 C，总空间 O(U*(log n+1)+C)。
*/
