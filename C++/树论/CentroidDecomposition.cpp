// 点分治骨架
// 用法：CentroidDecomposition cd(n); cd.add_edge(x, y); cd.work(root);
// cd.par[x] 是点分树中 x 的父亲；在 build() 里处理经过当前重心的贡献。
struct CentroidDecomposition {
    int n;
    vector<vi> g;
    vi siz, par, vis;

    CentroidDecomposition(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n, {});
        siz.assign(n, 0);
        par.assign(n, -1);
        vis.assign(n, 0);
    }

    void add_edge(int x, int y) {
        g[x].push_back(y);
        g[y].push_back(x);
    }

    int get_size(int x, int p) {
        siz[x] = 1;
        for (int y : g[x]) {
            if (y == p || vis[y]) continue;
            siz[x] += get_size(y, x);
        }
        return siz[x];
    }

    int get_centroid(int x, int p, int tot) {
        for (int y : g[x]) {
            if (y == p || vis[y]) continue;
            if (siz[y] * 2 > tot) return get_centroid(y, x, tot);
        }
        return x;
    }

    void build(int x, int p) {
        int tot = get_size(x, -1);
        int c = get_centroid(x, -1, tot);
        par[c] = p;
        vis[c] = 1;

        // 在这里处理经过重心 c 的路径贡献

        for (int y : g[c]) {
            if (vis[y]) continue;
            build(y, c);
        }
    }

    void work(int root = 0) {
        build(root, -1);
    }
};
