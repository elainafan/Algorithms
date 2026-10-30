/*
Kruskal 重构树：无向图，0-based，边为 {x,y,w}，边权 ll，支持不连通图。
对不同且连通的原图点，val[lca(x,y)] 为：
Min=true：升序建树，路径最大边权的最小值；Min=false：降序建树，路径最小边权的最大值。
get(x,k) 返回边权 <= k（升序）或 >= k（降序）时的连通块节点，siz 为原图点数。
构造 O(m log m+n log n)，查询 O(log n)，空间 O(m+n log n)。
*/
template <bool Min = true>
struct KruskalTree {
    int n, tot, lg;
    vector<vi> ma, pa;  // ma[x] 是重构树儿子，pa[x][i] 是 2^i 级祖先
    vi root, rt, dep, siz;  // root 保存各棵树的根，rt[x] 是 x 所在树的根
    vl val;

    KruskalTree(int n, vector<trl> edges)
        : n(n), tot(n), ma(2 * n), rt(2 * n), dep(2 * n), siz(2 * n), val(2 * n) {
        vi fa(2 * n);
        iota(all(fa), 0);
        rep(i, 0, n - 1) siz[i] = 1;
        auto get = [&](this auto&& get, int x) -> int {
            if (fa[x] != x) fa[x] = get(fa[x]);
            return fa[x];
        };  // 只用于建树的并查集，与重构树的父亲 pa 分开

        sort(all(edges), [](const trl& x, const trl& y) {
            return Min ? std::get<2>(x) < std::get<2>(y)
                       : std::get<2>(x) > std::get<2>(y);
        });
        for (auto [u, v, w] : edges) {
            int x = get(u), y = get(v);
            if (x == y) continue;
            val[tot] = w;
            siz[tot] = siz[x] + siz[y];
            ma[tot] = {x, y};
            fa[x] = fa[y] = tot;
            tot++;
        }

        lg = bit_width(unsigned(tot));
        pa.assign(tot, vi(lg, -1));
        // 父亲的编号总比儿子大，倒序处理即可先求出父亲的深度和倍增表。
        frep(x, tot - 1, 0) {
            int p = pa[x][0];
            if (p == -1) {
                root.emplace_back(x);
                rt[x] = x;
            } else {
                rt[x] = rt[p];
                dep[x] = dep[p] + 1;
            }
            rep(i, 1, lg - 1) {
                int p = pa[x][i - 1];
                if (p != -1) pa[x][i] = pa[p][i - 1];
            }
            for (int y : ma[x]) pa[y][0] = x;
        }
    }

    int lca(int x, int y) const {
        if (rt[x] != rt[y]) return -1;
        if (dep[x] < dep[y]) swap(x, y);
        int d = dep[x] - dep[y];
        rep(i, 0, lg - 1) {
            if (d >> i & 1) x = pa[x][i];
        }
        if (x == y) return x;
        frep(i, lg - 1, 0) {
            if (pa[x][i] != pa[y][i]) {
                x = pa[x][i], y = pa[y][i];
            }
        }
        return pa[x][0];
    }  // 返回重构树上的 LCA，不连通返回 -1

    int get(int x, ll k) const {
        frep(i, lg - 1, 0) {
            int p = pa[x][i];
            if (p != -1 && (Min ? val[p] <= k : val[p] >= k)) x = p;
        }
        return x;
    }  // x 为原点，返回阈值 k 下所属连通块；单点连通块返回 x 本身
};

/*
用法：

vector<trl> edges;                  // 每条无向边保存为 {x,y,w}
edges.emplace_back(x, y, w);        // 原点编号从 0 开始
KruskalTree<> tree(n, edges);       // 升序建树，查询最小瓶颈和边权 <= k 的连通块
// 降序建树时写 KruskalTree<false> tree(n, edges)，对应最大瓶颈和边权 >= k 的连通块。

int p = tree.lca(x, y);             // p=-1 表示不连通
// 若 x != y 且 p != -1，ll ans = tree.val[p] 即两点之间的瓶颈答案。
// x==y 时 p=x，叶子的 val 不用于瓶颈查询，空路径答案按题意处理。

int c = tree.get(x, k);             // 阈值 k 下，x 所在连通块对应的重构树节点
int cnt = tree.siz[c];              // 该连通块包含的原点数
bool same = tree.get(x, k) == tree.get(y, k);  // 阈值 k 下两点是否连通

tree.root 保存所有重构树的根；原图连通时只有一个根，编号为 tree.tot-1。
tree.ma[c] 保存 c 的儿子，tree.siz[c] 是其子树中的原点数，可用于后续树上统计。
*/
