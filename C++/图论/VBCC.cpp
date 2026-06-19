// 点双连通分量 Vertex-BCC，无向图，点 0-based
// 依赖 1.cpp 中的宏：rep, sz, all, vi, vvi, pii
// 一个割点会属于多个点双，所以这里用 belong[u] 存 u 属于哪些点双。
struct VBCC {
    int n, m, tim, tot;
    vector<vector<pii>> g;  // {to, edge_id}
    vector<pii> edges;      // edges[id] = {u, v}
    vi dfn, low, stk, cut;
    vvi comps, belong;      // comps[i] 是第 i 个点双里的点

    VBCC(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        m = tim = tot = 0;
        g.assign(n, vector<pii>());
        edges.clear();
        dfn.assign(n, 0);
        low.assign(n, 0);
        stk.clear();
        cut.assign(n, 0);
        comps.clear();
        belong.assign(n, vi());
    }

    int add_edge(int u, int v) {
        edges.push_back({u, v});
        g[u].push_back({v, m});
        g[v].push_back({u, m});
        return m++;
    }

    void add_comp(int stop_id) {
        vi comp;
        while (true) {
            int id = stk.back();
            stk.pop_back();
            comp.push_back(edges[id].first);
            comp.push_back(edges[id].second);
            if (id == stop_id) break;
        }
        sort(all(comp));
        comp.erase(unique(all(comp)), comp.end());
        for (int i = 0; i < sz(comp); i++) belong[comp[i]].push_back(tot);
        comps.push_back(comp);
        tot++;
    }

    void tarjan(int u, int pe) {
        dfn[u] = low[u] = ++tim;
        int child = 0;
        for (int i = 0; i < sz(g[u]); i++) {
            int v = g[u][i].first;
            int id = g[u][i].second;
            if (id == pe) continue;
            if (!dfn[v]) {
                stk.push_back(id);
                child++;
                tarjan(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u]) {
                    if (pe != -1 || child > 1) cut[u] = 1;
                    add_comp(id);
                }
            } else if (dfn[v] < dfn[u]) {
                // 只把返祖边压一次
                stk.push_back(id);
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    void work() {
        rep(i, 0, n - 1) {
            if (dfn[i]) continue;
            if (g[i].empty()) {
                dfn[i] = low[i] = ++tim;
                belong[i].push_back(tot);
                comps.push_back(vi(1, i));
                tot++;
            } else {
                tarjan(i, -1);
            }
        }
    }
};

// 用法：
// VBCC vbcc(n);
// vbcc.add_edge(u, v); // 无向边
// vbcc.work();
// vbcc.cut[u];         // u 是否为割点
// vbcc.comps;          // 每个点双的点集
// vbcc.belong[u];      // u 属于哪些点双，割点会出现多次
// 注意：孤立点会单独成为一个点双。
