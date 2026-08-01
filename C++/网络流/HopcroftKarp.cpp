// 依赖 1.cpp 中的 vi, vvi, all, sz
// 二分图最大匹配，左右部点都 0-based
struct HopcroftKarp {
    int nl, nr, shortest;
    vvi g;
    vi ml, mr, dep, cur;
    bool solved;

    HopcroftKarp(int nl_ = 0, int nr_ = 0) { init(nl_, nr_); }

    auto init(int nl_, int nr_) -> void {
        nl = nl_;
        nr = nr_;
        g.assign(nl, vi());
        ml.assign(nl, -1);
        mr.assign(nr, -1);
        dep.resize(nl);
        cur.resize(nl);
        solved = false;
    }

    auto add(int u, int v) -> void {
        assert(0 <= u && u < nl && 0 <= v && v < nr);
        g[u].push_back(v);
        solved = false;
    }

    auto bfs() -> bool {
        fill(all(dep), -1);
        queue<int> q;
        for (int u = 0; u < nl; u++) {
            if (ml[u] == -1) dep[u] = 0, q.push(u);
        }

        shortest = -1;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (shortest != -1 && dep[u] + 1 > shortest) continue;
            for (int v : g[u]) {
                int w = mr[v];
                if (w == -1) {
                    shortest = dep[u] + 1;
                } else if (shortest == -1 && dep[w] == -1) {
                    dep[w] = dep[u] + 1;
                    q.push(w);
                }
            }
        }
        return shortest != -1;
    }

    auto dfs(int u) -> bool {
        for (int& i = cur[u]; i < sz(g[u]); i++) {
            int v = g[u][i];
            int w = mr[v];
            if (w == -1) {
                if (dep[u] + 1 != shortest) continue;
            } else if (dep[w] != dep[u] + 1 || !dfs(w)) {
                continue;
            }
            ml[u] = v;
            mr[v] = u;
            return true;
        }
        dep[u] = -1;
        return false;
    }

    auto work() -> int {
        fill(all(ml), -1);
        fill(all(mr), -1);
        int res = 0;
        while (bfs()) {
            fill(all(cur), 0);
            for (int u = 0; u < nl; u++) {
                if (ml[u] == -1 && dfs(u)) res++;
            }
        }
        solved = true;
        return res;
    }

    auto min_cover() const -> pair<vi, vi> {
        assert(solved);
        vi vis_left(nl), vis_right(nr);
        queue<int> q;
        for (int u = 0; u < nl; u++) {
            if (ml[u] == -1) vis_left[u] = 1, q.push(u);
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : g[u]) {
                if (ml[u] == v || vis_right[v]) continue;
                vis_right[v] = 1;
                int w = mr[v];
                if (w != -1 && !vis_left[w]) vis_left[w] = 1, q.push(w);
            }
        }

        vi left, right;
        for (int u = 0; u < nl; u++) {
            if (!vis_left[u]) left.push_back(u);
        }
        for (int v = 0; v < nr; v++) {
            if (vis_right[v]) right.push_back(v);
        }
        return {left, right};
    }
};

// 用法：
// HopcroftKarp hk(nl, nr);
// hk.add(u, v);                         // 左部 u 与右部 v 连边
// int ans = hk.work();                  // 最大匹配数
// int mate_right = hk.ml[u];            // 左部 u 匹配的右部点，-1 表示未匹配
// int mate_left = hk.mr[v];             // 右部 v 匹配的左部点，-1 表示未匹配
// auto [left, right] = hk.min_cover();  // 一组最小点覆盖，分别返回左右部点
// min_cover 要求先 work；加边后需重新 work。
// 复杂度 O(E*sqrt(V))，递归深度最多为左部点数。
