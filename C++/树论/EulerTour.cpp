// 欧拉序
// 用法：EulerTour et(n); et.add_edge(x, y); et.work(root);
// et.order[tin[x]..tout[x]] 是 x 子树内所有点。
struct EulerTour {
    int n, tim = 0;
    vector<vi> g;
    vi tin, tout, order, fa, dep;

    EulerTour(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n, {});
        tin.assign(n, 0);
        tout.assign(n, 0);
        order.assign(n, 0);
        fa.assign(n, -1);
        dep.assign(n, 0);
        tim = 0;
    }

    void add_edge(int x, int y) {
        g[x].push_back(y);
        g[y].push_back(x);
    }

    void work(int root = 0) {
        auto dfs = [&](auto&& self, int x, int p) -> void {
            fa[x] = p;
            tin[x] = tim;
            order[tim++] = x;
            for (int y : g[x]) {
                if (y == p) continue;
                dep[y] = dep[x] + 1;
                self(self, y, x);
            }
            tout[x] = tim - 1;
        };
        dfs(dfs, root, -1);
    }

    bool is_ancestor(int x, int y) {
        return tin[x] <= tin[y] && tin[y] <= tout[x];
    }
};
