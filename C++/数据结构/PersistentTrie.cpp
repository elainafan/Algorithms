// 可持久化 01-Trie：保留历史 multiset，支持从任意版本增删整数和异或查询。
// LOG 是最高位编号；版本 0 为空集合，插入值和查询 x 均需满足 0 <= x < 2^(LOG+1)。

template <int LOG = 30>
class PersistentTrie {
    static_assert(0 <= LOG && LOG <= 62);

    struct Node {
        int son[2], cnt;
    };

    vector<Node> tree;
    vi root;

    static constexpr ull LIMIT = 1ULL << (LOG + 1);

    int node_count(int old, int now) const {
        return tree[now].cnt - tree[old].cnt;
    }

    int copy_node(int old, int delta) {
        Node cur = tree[old];
        cur.cnt += delta;
        tree.push_back(cur);
        return sz(tree) - 1;
    }

    int update(int old_root, ull x, int delta) {
        int old = old_root;
        int now = copy_node(old, delta);
        int new_root = now;
        for (int i = LOG; i >= 0; i--) {
            int b = x >> i & 1;
            int old_child = tree[old].son[b];
            int new_child = copy_node(old_child, delta);
            tree[now].son[b] = new_child;
            old = old_child;
            now = new_child;
        }
        return new_root;
    }  // 迭代复制根和 LOG+1 层儿子

    int count(int old, int now, ull x) const {
        for (int i = LOG; i >= 0; i--) {
            int b = x >> i & 1;
            old = tree[old].son[b];
            now = tree[now].son[b];
        }
        return node_count(old, now);
    }

    ll query_xor(int old, int now, ull x, bool maximum) const {
        if (node_count(old, now) <= 0) return -1;
        ull ans = 0;
        for (int i = LOG; i >= 0; i--) {
            int b = x >> i & 1;
            int to = b ^ maximum;
            int old_child = tree[old].son[to];
            int now_child = tree[now].son[to];
            if (node_count(old_child, now_child) <= 0) {
                to ^= 1;
                old_child = tree[old].son[to];
                now_child = tree[now].son[to];
            }
            ans |= (ull)(b ^ to) << i;
            old = old_child;
            now = now_child;
        }
        return (ll)ans;
    }

    ll kth_xor(int old, int now, ull x, int k) const {
        if (k < 1 || k > node_count(old, now)) return -1;
        ull ans = 0;
        for (int i = LOG; i >= 0; i--) {
            int b = x >> i & 1;
            int old_child = tree[old].son[b];
            int now_child = tree[now].son[b];
            int same = node_count(old_child, now_child);
            if (k > same) {
                k -= same;
                ans |= 1ULL << i;
                b ^= 1;
                old_child = tree[old].son[b];
                now_child = tree[now].son[b];
            }
            old = old_child;
            now = now_child;
        }
        return (ll)ans;
    }

    int count_xor_less(int old, int now, ull x, ull limit) const {
        if (!limit) return 0;
        if (limit >= LIMIT) return node_count(old, now);
        int ans = 0;
        for (int i = LOG; i >= 0; i--) {
            int xb = x >> i & 1;
            int kb = limit >> i & 1;
            if (kb) {
                int old_child = tree[old].son[xb];
                int now_child = tree[now].son[xb];
                ans += node_count(old_child, now_child);
                old = tree[old].son[xb ^ 1];
                now = tree[now].son[xb ^ 1];
            } else {
                old = tree[old].son[xb];
                now = tree[now].son[xb];
            }
            if (node_count(old, now) <= 0) break;
        }
        return ans;
    }

    void init(int max_updates) {
        tree.reserve(1 + (size_t)max_updates * (LOG + 2));
        root.reserve((size_t)max_updates + 1);
        tree.push_back({});
        root.push_back(0);
    }

public:
    // max_updates >= 0，只用于一次性预留内存。
    explicit PersistentTrie(int max_updates = 0) {
        init(max_updates);
    }

    // 依次插入 a，版本 i 表示前 i 个元素；max_updates >= 0，是建好后的预计增删数。
    template <typename T>
    explicit PersistentTrie(const vector<T>& a, int max_updates = 0) {
        init(sz(a) + max_updates);
        for (auto x : a) insert((ull)x);
    }

    // 从 version 版本分叉并插入一个 x，返回新版本编号。
    int insert(int version, ull x) {
        root.push_back(update(root[version], x, 1));
        return versions() - 1;
    }

    int insert(ull x) {
        return insert(latest_version(), x);
    }  // 从最新版本插入 x

    // 从 version 版本分叉并删除一个 x；需保证该版本中 x 存在。
    int erase(int version, ull x) {
        root.push_back(update(root[version], x, -1));
        return versions() - 1;
    }

    int erase(ull x) {
        return erase(latest_version(), x);
    }  // 从最新版本删除 x

    int copy_version(int version) {
        int old_root = root[version];
        root.push_back(old_root);
        return versions() - 1;
    }  // 均摊 O(1) 复制版本，不新建 Trie 节点

    int size(int version) const {
        return tree[root[version]].cnt;
    }

    int size() const {
        return size(latest_version());
    }

    int size_between(int left_version, int right_version) const {
        return node_count(root[left_version], root[right_version]);
    }

    int count(int version, ull x) const {
        return count(0, root[version], x);
    }

    int count(ull x) const {
        return count(latest_version(), x);
    }

    int count_between(int left_version, int right_version, ull x) const {
        return count(root[left_version], root[right_version], x);
    }

    ll max_xor(int version, ull x) const {
        return query_xor(0, root[version], x, true);
    }

    ll max_xor(ull x) const {
        return max_xor(latest_version(), x);
    }

    ll max_xor_between(int left_version, int right_version, ull x) const {
        return query_xor(root[left_version], root[right_version], x, true);
    }

    ll min_xor(int version, ull x) const {
        return query_xor(0, root[version], x, false);
    }

    ll min_xor(ull x) const {
        return min_xor(latest_version(), x);
    }

    ll min_xor_between(int left_version, int right_version, ull x) const {
        return query_xor(root[left_version], root[right_version], x, false);
    }

    ll kth_xor(int version, ull x, int k) const {
        return kth_xor(0, root[version], x, k);
    }

    ll kth_xor(ull x, int k) const {
        return kth_xor(latest_version(), x, k);
    }

    ll kth_xor_between(int left_version, int right_version, ull x, int k) const {
        return kth_xor(root[left_version], root[right_version], x, k);
    }

    int count_xor_less(int version, ull x, ull limit) const {
        return count_xor_less(0, root[version], x, limit);
    }

    int count_xor_less(ull x, ull limit) const {
        return count_xor_less(latest_version(), x, limit);
    }

    int count_xor_less_between(int left_version, int right_version, ull x, ull limit) const {
        return count_xor_less(root[left_version], root[right_version], x, limit);
    }

    int versions() const {
        return sz(root);
    }

    int latest_version() const {
        return versions() - 1;
    }

    void reserve_updates(int max_updates) {
        tree.reserve(tree.size() + (size_t)max_updates * (LOG + 2));
        root.reserve(root.size() + (size_t)max_updates);
    }  // max_updates >= 0，为后续增删预留内存
};

/*
用法：

PersistentTrie<30> tree(q);        // 维护 [0, 2^31) 内的整数，q 是预计增删数

int v1 = tree.insert(0, x);        // 从空版本插入 x
int v2 = tree.insert(v1, y);       // 从 v1 继续插入 y
int v3 = tree.insert(0, z);        // 从空版本分出另一支
int v4 = tree.erase(v2, x);        // 从 v2 分叉并删除一个 x
int v5 = tree.copy_version(v3);    // 复制 v3，不产生新 Trie 节点

int num = tree.count(v2, x);       // v2 中 x 的出现次数
ll mx = tree.max_xor(v2, p);       // v2 中最大的 p^y
ll mn = tree.min_xor(v2, p);       // v2 中最小的 p^y
ll kth = tree.kth_xor(v2, p, k);  // v2 中第 k 小的 p^y，k 从 1 开始
int less = tree.count_xor_less(v2, p, limit); // v2 中满足 (p^y)<limit 的 y 的数量

用前缀版本处理数组区间：
vl a(n);
PersistentTrie<30> prefix(a);      // 版本 i 恰好包含 a[0..i-1]
ll range_mx = prefix.max_xor_between(l, r + 1, p); // 查询 0-indexed 区间 [l,r]
ll range_kth = prefix.kth_xor_between(l, r + 1, p, k);

between 系列接口查询 multiset(right_version)-multiset(left_version)，
需保证每个 Trie 前缀的计数差均非负；两个无关分支通常不能直接作差。

所有接口都不检查版本、值域和删除合法性；插入值和查询 x 均需小于 2^(LOG+1)，
count_xor_less 的 limit 可为任意 ull。元素总数、单值重数、节点数和版本数均需能存入 int。
空集合的 max_xor/min_xor 以及非法 k 的 kth_xor 返回 -1。

每次增删时间 O(LOG+1)，且恰好新建 LOG+2 个节点；所有异或/计数查询 O(LOG+1)。
Node 只含两个 32 位儿子下标和一个 32 位计数，在常见平台上为 12 字节。
设增删数为 U、纯复制版本数为 C，节点数恰为 1+U*(LOG+2)，
版本根数为 1+U+C，总空间 O(U*(LOG+2)+C)。
*/
