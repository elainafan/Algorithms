// 边双连通分量 Edge-BCC，无向图，点 0-based
// 依赖 1.cpp 中的宏：rep, sz, vi, vvi, pii
// 思路：先找桥，再忽略桥 DFS 染色。
struct EBCC {
    int n, m, tim, tot;
    vector<vector<pii>> g;  // {to, edge_id}
    vector<pii> edges;      // edges[id] = {u, v}
    vi dfn, low, is_bridge, bel;
    vvi comps;              // comps[i] 是第 i 个边双里的点

    EBCC(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        m = tim = tot = 0;
        g.assign(n, vector<pii>());
        edges.clear();
        dfn.assign(n, 0);
        low.assign(n, 0);
        is_bridge.clear();
        bel.assign(n, -1);
        comps.clear();
    }

    int add_edge(int u, int v) {
        edges.push_back({u, v});
        is_bridge.push_back(0);
        g[u].push_back({v, m});
        g[v].push_back({u, m});
        return m++;
    }

    void tarjan(int u, int pe) {
        dfn[u] = low[u] = ++tim;
        for (int i = 0; i < sz(g[u]); i++) {
            int v = g[u][i].first;
            int id = g[u][i].second;
            if (id == pe) continue;
            if (!dfn[v]) {
                tarjan(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) is_bridge[id] = 1;
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    void dfs_comp(int u) {
        bel[u] = tot;
        comps.back().push_back(u);
        for (int i = 0; i < sz(g[u]); i++) {
            int v = g[u][i].first;
            int id = g[u][i].second;
            if (bel[v] != -1 || is_bridge[id]) continue;
            dfs_comp(v);
        }
    }

    vi work() {
        rep(i, 0, n - 1) {
            if (!dfn[i]) tarjan(i, -1);
        }
        rep(i, 0, n - 1) {
            if (bel[i] == -1) {
                comps.push_back(vi());
                dfs_comp(i);
                tot++;
            }
        }
        return bel;
    }

    // 建桥树，要求先 work()
    vvi build_tree() {
        vvi tree(tot);
        rep(id, 0, m - 1) {
            if (!is_bridge[id]) continue;
            int a = bel[edges[id].first];
            int b = bel[edges[id].second];
            tree[a].push_back(b);
            tree[b].push_back(a);
        }
        return tree;
    }
};

// 用法：
// EBCC ebcc(n);
// int id = ebcc.add_edge(u, v); // 无向边，id 是边编号
// vi bel = ebcc.work();         // bel[u] 是 u 所在边双编号
// ebcc.is_bridge[id];           // 这条边是否为桥
// ebcc.comps;                   // 每个边双的点集
// vvi tree = ebcc.build_tree(); // 桥树
// 注意：重边不会被误判成桥，因为每条无向边都有独立 edge_id。
