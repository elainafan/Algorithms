// 强连通分量 Tarjan，有向图，点 0-based
// 依赖 1.cpp 中的宏：rep, sz, all, vi, vvi
struct SCC {
    int n, tim, tot;
    vvi g;
    vi dfn, low, stk, ins, bel;
    vvi comps;  // comps[i] 是第 i 个强连通分量里的点

    SCC(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        tim = tot = 0;
        g.assign(n, vi());
        dfn.assign(n, 0);
        low.assign(n, 0);
        ins.assign(n, 0);
        bel.assign(n, -1);
        stk.clear();
        comps.clear();
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
    }

    void dfs(int u) {
        dfn[u] = low[u] = ++tim;
        stk.push_back(u);
        ins[u] = 1;
        for (int i = 0; i < sz(g[u]); i++) {
            int v = g[u][i];
            if (!dfn[v]) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (ins[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (dfn[u] == low[u]) {
            vi comp;
            while (true) {
                int x = stk.back();
                stk.pop_back();
                ins[x] = 0;
                bel[x] = tot;
                comp.push_back(x);
                if (x == u) break;
            }
            comps.push_back(comp);
            tot++;
        }
    }

    vi work() {
        rep(i, 0, n - 1) {
            if (!dfn[i]) dfs(i);
        }
        return bel;
    }

    // 缩点建 DAG，要求先 work()
    vvi build_dag() {
        vvi dag(tot);
        rep(u, 0, n - 1) {
            for (int i = 0; i < sz(g[u]); i++) {
                int v = g[u][i];
                if (bel[u] != bel[v]) dag[bel[u]].push_back(bel[v]);
            }
        }
        rep(i, 0, tot - 1) {
            sort(all(dag[i]));
            dag[i].erase(unique(all(dag[i])), dag[i].end());
        }
        return dag;
    }
};

// 用法：
// SCC scc(n);
// scc.add_edge(u, v);     // 有向边 u -> v
// vi bel = scc.work();    // bel[u] 是 u 所在 SCC 编号
// scc.comps;              // 每个 SCC 的点集
// vvi dag = scc.build_dag();
// 注意：Tarjan 得到的 SCC 编号通常是反拓扑序，别默认编号大小有语义。
