// 可持久化线段树：保留所有历史版本，支持从任意版本分叉、单点赋值和区间查询。
// 数组下标为 0-indexed，区间均为闭区间 [l, r]，0 号版本是初始数组。

struct Info {
    ll sum = 0;

    Info(ll x = 0) : sum(x) {}
};

Info operator+(const Info& a, const Info& b) {
    return {a.sum + b.sum};
}  // 按顺序合并左右子树

template <typename Info>
class PersistentSegmentTree {
    struct Node {
        int left, right;
        Info val;
    };

    int n;
    vector<Node> tree;
    vi root;

    Info merge_val(const Info& a, const Info& b) const {
        return a + b;
    }

    int new_node(int left, int right, const Info& val) {
        tree.push_back({left, right, val});
        return sz(tree) - 1;
    }

    int build(const vector<Info>& a, int l, int r) {
        if (l == r) return new_node(0, 0, a[l]);
        int m = (l + r) >> 1;
        int left = build(a, l, m);
        int right = build(a, m + 1, r);
        return new_node(left, right, merge_val(tree[left].val, tree[right].val));
    }

    int build(int l, int r, const Info& init_val) {
        if (l == r) return new_node(0, 0, init_val);
        int m = (l + r) >> 1;
        int left = build(l, m, init_val);
        int right = build(m + 1, r, init_val);
        return new_node(left, right, merge_val(tree[left].val, tree[right].val));
    }

    int update(int old, int l, int r, int p, const Info& val) {
        if (l == r) return new_node(0, 0, val);
        int m = (l + r) >> 1;
        int left = tree[old].left;
        int right = tree[old].right;
        if (p <= m)
            left = update(left, l, m, p, val);
        else
            right = update(right, m + 1, r, p, val);
        return new_node(left, right, merge_val(tree[left].val, tree[right].val));
    }  // 沿根到叶子复制一条链

    Info query(int node, int l, int r, int ql, int qr) const {
        if (ql <= l && r <= qr) return tree[node].val;
        int m = (l + r) >> 1;
        if (qr <= m) return query(tree[node].left, l, m, ql, qr);
        if (ql > m) return query(tree[node].right, m + 1, r, ql, qr);
        return merge_val(query(tree[node].left, l, m, ql, qr),
                         query(tree[node].right, m + 1, r, ql, qr));
    }

    template <typename F>
    int find_first(int node, int l, int r, int ql, int qr, F& check) const {
        if (r < ql || qr < l || !check(tree[node].val)) return -1;
        if (l == r) return l;
        int m = (l + r) >> 1;
        int res = find_first(tree[node].left, l, m, ql, qr, check);
        if (res != -1) return res;
        return find_first(tree[node].right, m + 1, r, ql, qr, check);
    }

    template <typename F>
    int find_last(int node, int l, int r, int ql, int qr, F& check) const {
        if (r < ql || qr < l || !check(tree[node].val)) return -1;
        if (l == r) return l;
        int m = (l + r) >> 1;
        int res = find_last(tree[node].right, m + 1, r, ql, qr, check);
        if (res != -1) return res;
        return find_last(tree[node].left, l, m, ql, qr, check);
    }

    int nodes_per_update() const {
        return bit_width((unsigned)n - 1) + 1;
    }

    void reserve_memory(int max_updates) {
        tree.reserve((size_t)2 * n - 1 + (size_t)max_updates * nodes_per_update());
        root.reserve((size_t)max_updates + 1);
    }

public:
    // 维护长度为 n、初值均为 init_val 的数组；n > 0，max_updates >= 0。
    PersistentSegmentTree(int size, const Info& init_val = Info(), int max_updates = 0) : n(size) {
        reserve_memory(max_updates);
        root.push_back(build(0, n - 1, init_val));
    }

    // 维护初始数组 a；max_updates >= 0 且仅用于预留内存，不影响正确性。
    PersistentSegmentTree(const vector<Info>& a, int max_updates = 0) : n(sz(a)) {
        reserve_memory(max_updates);
        root.push_back(build(a, 0, n - 1));
    }

    // 从 version 版本分叉，令 a[p] = val，返回新版本编号。
    int update(int version, int p, const Info& val) {
        root.push_back(update(root[version], 0, n - 1, p, val));
        return versions() - 1;
    }

    // 从最新版本分叉，令 a[p] = val，返回新版本编号。
    int update(int p, const Info& val) {
        return update(latest_version(), p, val);
    }

    // 均摊 O(1) 复制 version 为一个新版本，不新建线段树节点。
    int copy_version(int version) {
        root.push_back(root[version]);
        return versions() - 1;
    }

    // 查询 version 版本的 [ql, qr]。
    Info query(int version, int ql, int qr) const {
        return query(root[version], 0, n - 1, ql, qr);
    }

    Info query(int ql, int qr) const {
        return query(latest_version(), ql, qr);
    }  // 查询最新版本的 [ql, qr]

    Info get(int version, int p) const {
        return query(version, p, p);
    }  // 查询 version 版本的 a[p]

    Info get(int p) const {
        return get(latest_version(), p);
    }  // 查询最新版本的 a[p]

    Info query_all(int version) const {
        return tree[root[version]].val;
    }  // O(1) 查询 version 版本的整个数组

    Info query_all() const {
        return query_all(latest_version());
    }  // 查询最新版本的整个数组

    template <typename F>
    int find_first(int version, int ql, int qr, F&& check) const {
        return find_first(root[version], 0, n - 1, ql, qr, check);
    }  // 查询 [ql, qr] 中第一个令叶子 Info 通过 check 的位置

    template <typename F>
    int find_first(int ql, int qr, F&& check) const {
        return find_first(latest_version(), ql, qr, check);
    }

    template <typename F>
    int find_last(int version, int ql, int qr, F&& check) const {
        return find_last(root[version], 0, n - 1, ql, qr, check);
    }  // 查询 [ql, qr] 中最后一个令叶子 Info 通过 check 的位置

    template <typename F>
    int find_last(int ql, int qr, F&& check) const {
        return find_last(latest_version(), ql, qr, check);
    }

    int versions() const {
        return sz(root);
    }

    int latest_version() const {
        return versions() - 1;
    }

    // 已经建树后才知道修改数时，可手动为后续修改预留内存。
    void reserve_updates(int max_updates) {
        tree.reserve(tree.size() + (size_t)max_updates * nodes_per_update());
        root.reserve(root.size() + (size_t)max_updates);
    }
};

/*
用法：

vector<Info> a(n);
rep(i, 0, n - 1) cin >> a[i].sum;

PersistentSegmentTree<Info> tree(a, q); // q 是预计的修改数，用于一次性 reserve

int v1 = tree.update(0, p, Info(x));    // 从初始版本修改，返回新版本 v1
int v2 = tree.update(v1, p2, Info(y));  // 从 v1 继续修改
int v3 = tree.update(0, p3, Info(z));   // 从初始版本分出另一条支线
int v4 = tree.update(p4, Info(w));      // 从当前最新版本 v3 修改
int v5 = tree.copy_version(v2);         // 复制 v2，不产生新节点

ll res = tree.query(v2, l, r).sum;      // v2 版本的区间和
ll cur = tree.get(v1, p).sum;           // v1 版本的 a[p]
ll total = tree.query_all(v3).sum;      // v3 版本的整个数组信息
int cnt = tree.versions();               // 当前版本数，版本编号为 [0, cnt - 1]
不传 version 的 update / query / get / query_all / find_first / find_last 默认操作最新版本。

find_first / find_last 需要传入能判断“整段是否可能含有答案”的剪枝函数。
例如 Info 维护区间最大值 mx 时：
int first = tree.find_first(v2, l, r, [&](const Info& x) { return x.mx >= need; });
int last = tree.find_last(v2, l, r, [&](const Info& x) { return x.mx >= need; });
剪枝有效时通常为 O(log n)；check 过弱时最坏会遍历整个待查区间。

所有接口均不做边界检查：需保证 n > 0、0 <= version < tree.versions()、
0 <= p < n、0 <= l <= r < n。旧版本永远不会被 update 修改。

建树时间 O(n)，单次修改/查询 O(log n)；初始树恰好有 2n-1 个节点，
每次修改只新建至多 bit_width(n-1)+1 个节点。设修改数为 U、纯复制版本数为 C，
总空间 O(n + U log n + C)。
Node 只保留两个 32 位儿子下标和一份 Info，节点池连续存储；传入 max_updates 可避免节点池反复扩容。
max_updates 只需统计 update 次数；copy_version 不产生节点，无需计入节点池容量。
*/
