// 依赖 Dinic.cpp 和 1.cpp 中的 ll, vi, vl, sz
// 先复制 Dinic，再复制本文件
// 上下界可行流 / 上下界最大流与最小流，点 0-based
struct LowerFlow {
    struct Edge {
        int from, to;
        ll low, high;
    };

    int n;
    vector<Edge> edges;

    LowerFlow(int n_ = 0) { init(n_); }

    auto init(int n_) -> void {
        n = n_;
        edges.clear();
    }

    auto add(int u, int v, ll low, ll high) -> int {
        assert(0 <= low && low <= high);
        edges.push_back({u, v, low, high});
        return sz(edges) - 1;
    }

    auto feasible() const -> optional<vl> {
        Dinic mf(n + 2);
        vi id, extra;
        ll need = build(mf, id, extra);
        if (mf.flow(n, n + 1, need) != need) return nullopt;
        return recover(mf, id);
    }

    auto feasible_st(int s, int t) const -> optional<pair<ll, vl>> {
        if (s == t) return nullopt;
        Dinic mf(n + 2);
        vi id, extra;
        auto back = prepare_st(mf, id, extra, s, t);
        if (!back) return nullopt;
        return pair<ll, vl>{mf.get(*back), recover(mf, id)};
    }

    auto max_flow(int s, int t) const -> optional<pair<ll, vl>> {
        if (s == t) return nullopt;
        Dinic mf(n + 2);
        vi id, extra;
        auto back = prepare_st(mf, id, extra, s, t);
        if (!back) return nullopt;

        ll base = mf.get(*back);
        disable_auxiliary(mf, extra, *back);
        ll ans = base + mf.flow(s, t);
        return pair<ll, vl>{ans, recover(mf, id)};
    }

    auto min_flow(int s, int t) const -> optional<pair<ll, vl>> {
        if (s == t) return nullopt;
        Dinic mf(n + 2);
        vi id, extra;
        auto back = prepare_st(mf, id, extra, s, t);
        if (!back) return nullopt;

        ll base = mf.get(*back);
        disable_auxiliary(mf, extra, *back);
        ll ans = base - mf.flow(t, s, base);
        return pair<ll, vl>{ans, recover(mf, id)};
    }

private:
    auto build(Dinic& mf, vi& id, vi& extra) const -> ll {
        vl d(n);
        id.resize(sz(edges));
        for (int i = 0; i < sz(edges); i++) {
            const auto& e = edges[i];
            id[i] = mf.add(e.from, e.to, e.high - e.low);
            d[e.from] -= e.low;
            d[e.to] += e.low;
        }

        ll need = 0;
        for (int i = 0; i < n; i++) {
            if (d[i] > 0) {
                extra.push_back(mf.add(n, i, d[i]));
                need += d[i];
            } else if (d[i] < 0) {
                extra.push_back(mf.add(i, n + 1, -d[i]));
            }
        }
        return need;
    }

    auto prepare_st(Dinic& mf, vi& id, vi& extra, int s, int t) const
        -> optional<int> {
        ll need = build(mf, id, extra);
        int back = mf.add(t, s, Dinic::INF);
        if (mf.flow(n, n + 1, need) != need) return nullopt;
        return back;
    }

    auto disable_auxiliary(Dinic& mf, const vi& extra, int back) const -> void {
        for (int e : extra) mf.edges[e].cap = mf.edges[e ^ 1].cap = 0;
        mf.edges[back].cap = mf.edges[back ^ 1].cap = 0;
    }

    auto recover(const Dinic& mf, const vi& id) const -> vl {
        vl res(sz(edges));
        for (int i = 0; i < sz(edges); i++) {
            res[i] = edges[i].low + mf.get(id[i]);
        }
        return res;
    }
};

// 用法：
// LowerFlow mf(n);
// int id = mf.add(u, v, low, high); // 边流量限制为 [low,high]
// auto a = mf.feasible();           // 无源汇可行流，返回 optional<vl>
// if (a) cout << (*a)[id] << endl;  // 第 id 条原边的一组可行流量
// auto p = mf.feasible_st(s, t);     // 任意一组非负 s-t 可行流
// auto b = mf.max_flow(s, t);       // 上下界 s-t 最大流
// auto c = mf.min_flow(s, t);       // 上下界 s-t 最小非负流
// if (b) {
//     auto [flow, edge_flow] = *b;  // 最大流值和每条原边的流量
// }
// 有源汇接口要求 s!=t；s==t 时返回 nullopt。
// 参数合法时，nullopt 表示不存在可行流。
// 有源汇接口都按流值非负的 s-t 流定义。
// 每个求解函数都会重新建图，可独立调用。
// 容量、平衡量和答案均需在 ll 范围内。
