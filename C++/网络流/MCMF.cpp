// 依赖 1.cpp 中的宏：ll, i128, vi, vvi, pll, all, sz
// 有向图，点 0-based；势能 + Dijkstra 求最小费用流
struct MCMF {
    static constexpr i128 INF = (i128)1 << 120;

    struct Edge {
        int to;
        ll cap, cost;
    };

    int n;
    vector<Edge> edges;
    vvi g;
    vi pre;
    vector<i128> dis, h;

    MCMF(int n_ = 0) { init(n_); }

    auto init(int n_) -> void {
        n = n_;
        edges.clear();
        g.assign(n, vi());
        pre.resize(n);
        dis.resize(n);
        h.resize(n);
    }

    auto add(int u, int v, ll cap, ll cost) -> int {
        assert(cap >= 0 && cost != numeric_limits<ll>::min());
        int id = sz(edges);
        g[u].push_back(id);
        edges.push_back({v, cap, cost});
        g[v].push_back(id ^ 1);
        edges.push_back({u, 0, -cost});
        return id;
    }

    auto init_h(int s) -> bool {
        fill(all(h), 0);
        bool neg = false;
        for (const auto& e : edges) {
            if (e.cap > 0 && e.cost < 0) neg = true;
        }
        if (!neg) return true;

        fill(all(h), INF);
        vi inq(n), len(n);
        queue<int> q;
        h[s] = 0;
        q.push(s);
        inq[s] = 1;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            inq[u] = 0;
            for (int id : g[u]) {
                const auto& e = edges[id];
                i128 nd = h[u] + e.cost;
                if (e.cap == 0 || h[e.to] <= nd) continue;
                h[e.to] = nd;
                pre[e.to] = id;
                len[e.to] = len[u] + 1;
                if (len[e.to] >= n) return false;
                if (!inq[e.to]) q.push(e.to), inq[e.to] = 1;
            }
        }
        for (int i = 0; i < n; i++) {
            if (h[i] == INF) h[i] = 0;
        }
        return true;
    }

    auto dijkstra(int s, int t) -> bool {
        fill(all(dis), INF);
        fill(all(pre), -1);
        priority_queue<pair<i128, int>, vector<pair<i128, int>>, greater<>> q;
        dis[s] = 0;
        q.push({0, s});
        while (!q.empty()) {
            auto [d, u] = q.top();
            q.pop();
            if (d != dis[u]) continue;
            for (int id : g[u]) {
                const auto& e = edges[id];
                if (e.cap == 0) continue;
                i128 nd = d + e.cost + h[u] - h[e.to];
                if (nd >= dis[e.to]) continue;
                dis[e.to] = nd;
                pre[e.to] = id;
                q.push({nd, e.to});
            }
        }
        if (pre[t] == -1) return false;
        for (int i = 0; i < n; i++) {
            if (dis[i] != INF) h[i] += dis[i];
        }
        return true;
    }

    auto flow(int s, int t, ll lim = numeric_limits<ll>::max()) -> pll {
        if (s == t || lim <= 0) return {0, 0};
        if (!init_h(s)) {
            assert(false && "reachable negative-cost cycle");
            return {0, 0};
        }
        ll res = 0;
        i128 cost = 0;
        while (res < lim && dijkstra(s, t)) {
            ll f = lim - res;
            for (int v = t; v != s; v = edges[pre[v] ^ 1].to) {
                f = min(f, edges[pre[v]].cap);
            }
            i128 path_cost = 0;
            for (int v = t; v != s; v = edges[pre[v] ^ 1].to) {
                int id = pre[v];
                path_cost += edges[id].cost;
                edges[id].cap -= f;
                edges[id ^ 1].cap += f;
            }
            res += f;
            cost += (i128)f * path_cost;
        }
        assert(cost >= numeric_limits<ll>::min() && cost <= numeric_limits<ll>::max());
        return {res, (ll)cost};
    }

    auto get(int id) const -> ll { return edges[id ^ 1].cap; }
};

// 用法：
// MCMF mf(n);
// int id = mf.add(u, v, cap, cost); // 有向边 u -> v
// auto [f, c] = mf.flow(s, t);      // 最小费用最大流，返回 {流量,费用}
// auto [f, c] = mf.flow(s, t, k);   // 最小费用流 k 单位，f<k 表示流不满
// ll edge_flow = mf.get(id);        // 边 id 当前已流的流量
// 支持负费用边，但要求 s 可达的正容量残量图中没有负费用环。
// 违反该前提时，调试构建会断言失败；关闭断言后 flow 返回 {0,0}。
// 本实现只增广 s-t 路径，不会主动加入与 s 不连通的负费用环流。
// 标准定义下若存在这类负环，需要负环消圈或其它环流算法。
// 最大费用流：原费用不能是 LLONG_MIN，传 -cost，总费用用 i128 取反。
// 最短路和势能用 i128，返回的总费用需在 ll 范围内。
// flow 会修改残量网络；重复调用返回继续增广的流量和费用。
// 初始 SPFA 最坏 O(VE)，之后每轮增广的 Dijkstra 为 O(ElogV)。
