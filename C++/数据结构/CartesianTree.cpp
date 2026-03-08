// 笛卡尔树，默认为大根堆
template <class T, typename Compare = std::greater<T>>
struct CartesianTree {
    T inf = std::numeric_limits<T>::max();

    struct Node {
        int idx;               // 原数组下标
        T val;                 // 权值
        int par, siz;          // 父节点索引，子树大小
        std::array<T, 2> son;  // 左儿子，右儿子

        Node(int idx = 0, T val = 0, int par = 0, int siz = 0) : idx(idx), val(val), par(par), siz(siz), son{} {}
    };

    std::vector<Node> t;

    CartesianTree() { init(); }

    void init(Compare comp = Compare()) {
        t.assign(1, {0, 0, 0});
        t[0].son.fill(0);
        if (comp(-inf, inf))
            t[0].val = -inf;
        else
            t[0].val = inf;
    }  // 自动建立虚拟节点

    void add(int idx, T val, int par = 0) { t.emplace_back(idx, val, par); }  // 负责把元素加进末尾，需要使用1-based

    int work(Compare comp = Compare()) {
        for (int i = 1; i < t.size(); i++) {
            int k = i - 1;
            while (comp(t[i].val, t[k].val)) k = t[k].par;
            t[i].son[0] = t[k].son[1];
            t[k].son[1] = i;
            t[i].par = k;
            t[t[i].son[0]].par = i;
        }  // 遍历，砍树枝
        auto dfs = [&](auto &&dfs, int u) -> void {
            if (!u) return;
            t[u].siz = 1;
            dfs(dfs, ls(u));
            dfs(dfs, rs(u));
            t[u].siz += t[ls(u)].siz + t[rs(u)].siz;
        };  // 进行一个dfs
        dfs(dfs, t[0].son[1]);
        return t[0].son[1];
    }

    int Left(int p) { return p - size(ls(p)); }  // 左边最远

    int Right(int p) { return p + size(rs(p)); }  // 右边最远

    int size(int p) { return t[p].siz; }

    int ls(int p) { return t[p].son[0]; }

    int rs(int p) { return t[p].son[1]; }

    int par(int p) { return t[p].par; }
};

// 使用示例
CartesianTree<int, less<int>> ct;

rep(i, 0, n - 1) { ct.add(i + 1, nums[i]); }

int root = ct.work(less<int>());
rep(i, 1, n) {
    int L = ct.Left(i);
    int R = ct.Right(i);
}