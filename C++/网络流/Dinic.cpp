// 依赖 1.cpp 中的宏：ll, vi, vvi, all, sz
// 有向图，点 0-based，容量为 ll
struct Dinic {
    static constexpr ll INF = numeric_limits<ll>::max();

    struct Edge {
        int to;
        ll cap;
    };

    int n;
    vector<Edge> edges;
    vvi g;
    vi dep, cur;

    Dinic(int n_ = 0) { init(n_); }

    auto init(int n_) -> void {
        n = n_;
        edges.clear();
        g.assign(n, vi());
        dep.resize(n);
        cur.resize(n);
    }

    auto add(int u, int v, ll cap) -> int {
        assert(cap >= 0);
        int id = sz(edges);
        g[u].push_back(id);
        edges.push_back({v, cap});
        g[v].push_back(id ^ 1);
        edges.push_back({u, 0});
        return id;
    }

    auto bfs(int s, int t) -> bool {
        fill(all(dep), -1);
        queue<int> q;
        dep[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int id : g[u]) {
                const auto& e = edges[id];
                if (e.cap == 0 || dep[e.to] != -1) continue;
                dep[e.to] = dep[u] + 1;
                q.push(e.to);
            }
        }
        return dep[t] != -1;
    }

    auto dfs(int u, int t, ll lim) -> ll {
        if (u == t) return lim;
        ll res = 0;
        for (int& i = cur[u]; i < sz(g[u]) && res < lim; i++) {
            int id = g[u][i];
            auto& e = edges[id];
            if (e.cap == 0 || dep[e.to] != dep[u] + 1) continue;
            ll f = dfs(e.to, t, min(lim - res, e.cap));
            if (f == 0) continue;
            e.cap -= f;
            edges[id ^ 1].cap += f;
            res += f;
        }
        return res;
    }

    auto flow(int s, int t, ll lim = INF) -> ll {
        if (s == t || lim <= 0) return 0;
        ll res = 0;
        while (res < lim && bfs(s, t)) {
            fill(all(cur), 0);
            while (res < lim) {
                ll f = dfs(s, t, lim - res);
                if (f == 0) break;
                res += f;
            }
        }
        return res;
    }

    auto get(int id) const -> ll { return edges[id ^ 1].cap; }

    auto cut(int s) const -> vi {
        vi vis(n);
        queue<int> q;
        vis[s] = 1;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int id : g[u]) {
                const auto& e = edges[id];
                if (e.cap == 0 || vis[e.to]) continue;
                vis[e.to] = 1;
                q.push(e.to);
            }
        }
        return vis;
    }
};

// 用法：
// Dinic mf(n);
// int id = mf.add(u, v, cap);   // 加有向边 u -> v，返回正向边编号
// ll ans = mf.flow(s, t);       // 最大流
// ll got = mf.flow(s, t, k);    // 最多再流 k，got < k 表示流不满
// ll f = mf.get(id);            // 边 id 当前已流的流量
// vi side = mf.cut(s);          // 跑完最大流后，side[u]=1 是最小割的源点侧
// 无向容量边写 mf.add(u, v, c), mf.add(v, u, c)。
// edges[id] 是正向残量边，edges[id^1] 是反向边；flow 会修改残量网络。
// 一般图复杂度 O(V^2 E)。
