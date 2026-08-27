// 可持久化并查集：支持从任意历史版本合并，以及查询代表元素、连通性和集合大小。
// 元素为 0-indexed，版本 0 是 n 个独立集合。

class PersistentUnionFind {
    // 内部节点的 ch 存儿子；叶子的 ch[0] 复用为 d[x]。
    // d[x] < 0 表示 x 是根且集合大小为 -d[x]，否则 d[x] 是父亲。
    struct Node {
        int ch[2];
    };

    struct State {
        int root, cc;
    };

    int lg;
    vector<Node> tree;
    vector<State> states;

    int new_node(int x, int y) {
        tree.push_back({x, y});
        return sz(tree) - 1;
    }

    int value(int root, int p) const {
        int node = root;
        for (int i = lg - 1; i >= 0; i--) {
            if (!node) return -1;
            node = tree[node].ch[p >> i & 1];
        }
        return node ? tree[node].ch[0] : -1;
    }  // 空子树中的所有位置默认为 -1

    int find_root(int root, int x, int& root_val) const {
        root_val = value(root, x);
        while (root_val >= 0) {
            x = root_val;
            root_val = value(root, x);
        }
        return x;
    }  // 不路径压缩，顺便返回根的负大小

    int update_one(int old, int bit, int p, int val) {
        if (bit < 0) return new_node(val, 0);
        int child[2] = {tree[old].ch[0], tree[old].ch[1]};
        int b = p >> bit & 1;
        child[b] = update_one(child[b], bit - 1, p, val);
        return new_node(child[0], child[1]);
    }

    int update_two(int old, int bit, int p1, int v1, int p2, int v2) {
        if (bit < 0) return new_node(v1, 0);  // 防御分支；成功合并时 p1 != p2
        int child[2] = {tree[old].ch[0], tree[old].ch[1]};
        int b1 = p1 >> bit & 1;
        int b2 = p2 >> bit & 1;
        if (b1 == b2) {
            child[b1] = update_two(child[b1], bit - 1, p1, v1, p2, v2);
        } else {
            child[b1] = update_one(child[b1], bit - 1, p1, v1);
            child[b2] = update_one(child[b2], bit - 1, p2, v2);
        }
        return new_node(child[0], child[1]);
    }  // 一次复制两个位置的路径并，避免重复复制公共前缀

    int nodes_per_merge() const {
        return 2 * lg + 1;
    }

public:
    // n > 0；max_operations >= 0，是预计会新建版本的操作数。
    PersistentUnionFind(int n, int max_operations = 0)
        : lg(bit_width((unsigned)n - 1)) {
        tree.reserve(1 + (size_t)max_operations * nodes_per_merge());
        states.reserve((size_t)max_operations + 1);
        tree.push_back({});
        states.push_back({0, n});
    }

    // 从 version 版本分叉并合并 x,y，返回新版本；已连通时仍会新建等价版本。
    int merge(int version, int x, int y) {
        const State old = states[version];
        int vx, vy;
        x = find_root(old.root, x, vx);
        y = find_root(old.root, y, vy);
        if (x == y) return copy_version(version);

        // 负数绝对值越大，集合越大；保证把较小集合 x 挂到较大集合 y。
        if (vx < vy) {
            swap(x, y);
            swap(vx, vy);
        }
        int new_root = update_two(old.root, lg - 1,
                                  x, y,           // d[x] = y
                                  y, vx + vy);    // d[y] = -size(x∪y)
        states.push_back({new_root, old.cc - 1});
        return versions() - 1;
    }

    int merge(int x, int y) {
        return merge(latest_version(), x, y);
    }  // 从最新版本合并

    int copy_version(int version) {
        State old = states[version];
        states.push_back(old);
        return versions() - 1;
    }  // 均摊 O(1) 复制版本，不新建持久化数组节点

    int get(int version, int x) const {
        int root_val;
        return find_root(states[version].root, x, root_val);
    }  // 查询 version 版本中 x 的代表元素

    int get(int x) const {
        return get(latest_version(), x);
    }

    bool is_same(int version, int x, int y) const {
        return get(version, x) == get(version, y);
    }

    bool is_same(int x, int y) const {
        return is_same(latest_version(), x, y);
    }

    int get_size(int version, int x) const {
        int root_val;
        find_root(states[version].root, x, root_val);
        return -root_val;
    }

    int get_size(int x) const {
        return get_size(latest_version(), x);
    }

    int components(int version) const {
        return states[version].cc;
    }

    int components() const {
        return components(latest_version());
    }

    int versions() const {
        return sz(states);
    }

    int latest_version() const {
        return versions() - 1;
    }

    void reserve_operations(int max_operations) {
        tree.reserve(tree.size() + (size_t)max_operations * nodes_per_merge());
        states.reserve(states.size() + (size_t)max_operations);
    }  // 为后续新版本按“全部成功合并”的最坏情况预留内存
};

/*
用法：

PersistentUnionFind uf(n, q);     // 版本 0 中每个元素自成一个集合

int v1 = uf.merge(0, x, y);       // 从初始版本合并 x,y
int v2 = uf.merge(v1, y, z);      // 从 v1 继续合并
int v3 = uf.merge(0, a, b);       // 从初始版本分出另一支
int v4 = uf.merge(c, d);          // 从当前最新版本 v3 合并
int v5 = uf.copy_version(v2);     // 复制 v2，不产生新持久化数组节点

int leader = uf.get(v2, x);       // v2 中 x 的代表元素
bool same = uf.is_same(v2, x, z); // v2 中 x,z 是否连通
int size = uf.get_size(v2, x);    // v2 中 x 所在集合的大小
int cc = uf.components(v2);       // v2 中的连通块数

不传 version 的 merge/get/is_same/get_size/components 默认操作最新版本。
merge 无论是否真正合并都会新建一个版本，便于令操作编号和版本编号对齐。

所有接口均不做边界检查：需保证 n > 0、0 <= x,y < n、
0 <= version < uf.versions()、max_operations >= 0。

初始化 O(1)；按大小合并保证并查集树高 O(log n)，持久化数组单点读取 O(log n)，
因此 get/is_same/get_size/merge 最坏 O(log^2 n)，components/copy_version 均摊 O(1)。
不做路径压缩，否则查询也会修改历史版本。

每次成功合并最多新建 2*bit_width(n-1)+1 个节点，失败合并和纯复制不新建节点。
Node 只有两个 int，在常见平台上为 8 字节；每个版本的 State 也为 8 字节。
设成功合并数为 U、总新版本数为 V，总空间 O(U log n + V)。
*/
