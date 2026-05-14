// 重链剖分 HLD
// 用法：HLD hld(n); hld.add_edge(x, y); hld.work(root);
// hld.lca(x, y) 查询 LCA；hld.get_path(x, y) 返回若干段 DFS 序区间。
// hld.dfn[x]..hld.dfn[x]+hld.siz[x]-1 是 x 的子树区间。
struct HLD {
    int n, tim = 0;
    vector<vi> g;
    vi fa, dep, siz, son, top, dfn, rnk;

    HLD(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n, {});
        fa.assign(n, -1);
        dep.assign(n, 0);
        siz.assign(n, 0);
        son.assign(n, -1);
        top.assign(n, 0);
        dfn.assign(n, 0);
        rnk.assign(n, 0);
        tim = 0;
    }

    void add_edge(int x, int y) {
        g[x].push_back(y);
        g[y].push_back(x);
    }

    void work(int root = 0) {
        auto dfs1 = [&](auto&& self, int x, int p) -> void {
            fa[x] = p;
            siz[x] = 1;
            for (int y : g[x]) {
                if (y == p) continue;
                dep[y] = dep[x] + 1;
                self(self, y, x);
                siz[x] += siz[y];
                if (son[x] == -1 || siz[y] > siz[son[x]]) son[x] = y;
            }
        };
        auto dfs2 = [&](auto&& self, int x, int tp) -> void {
            top[x] = tp;
            dfn[x] = tim;
            rnk[tim++] = x;
            if (son[x] != -1) self(self, son[x], tp);
            for (int y : g[x]) {
                if (y == fa[x] || y == son[x]) continue;
                self(self, y, y);
            }
        };
        dfs1(dfs1, root, -1);
        dfs2(dfs2, root, root);
    }

    int lca(int x, int y) {
        while (top[x] != top[y]) {
            if (dep[top[x]] < dep[top[y]]) swap(x, y);
            x = fa[top[x]];
        }
        return dep[x] < dep[y] ? x : y;
    }

    int dist(int x, int y) {
        int z = lca(x, y);
        return dep[x] + dep[y] - 2 * dep[z];
    }

    vector<pii> get_path(int x, int y) {
        vector<pii> res;
        while (top[x] != top[y]) {
            if (dep[top[x]] < dep[top[y]]) swap(x, y);
            res.push_back({dfn[top[x]], dfn[x]});
            x = fa[top[x]];
        }
        if (dep[x] > dep[y]) swap(x, y);
        res.push_back({dfn[x], dfn[y]});
        return res;
    }
};
