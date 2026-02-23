// 无向图欧拉回路/欧拉路径
vi eulerianPathOnUndirectedGraph(int n, int m, vector<vector<pii>> &ma) {
    // ma的第二维是边的编号
    rep(i, 0, n - 1) {
        sort(all(ma[i]), [&](const pii &a, const pii &b) { return a.first < b.first; });
    }
    int st = 0, cnt = 0;
    frep(i, n - 1, 0) {
        if (!ma[i].empty()) {
            if (sz(ma[i]) % 2) {
                st = i;
                cnt++;
            } else if (cnt == 0) {
                st = i;
            }
        }
    }
    // 分别处理欧拉回路和欧拉路径的情况
    if (cnt > 2) return {};
    vi path;
    vector<bool> vis(m, false);
    vi head(n);
    auto dfs = [&](this auto &&dfs, int v) -> void {
        for (int &i = head[v]; i < sz(ma[v]);) {
            auto [to, id] = ma[v][i++];
            if (vis[id]) continue;
            vis[id] = true;
            dfs(to);
        }
        path.push_back(v);
        return;
    };
    dfs(st);
    ranges::reverse(path);
    return path;
}

// 有向图欧拉回路/欧拉路径
vi eulerianPathOnUndirectedGraph(int n, int m, vector<vector<pii>> &ma) {
    // ma的第二维是边的编号
    vi in(n);  // 入度
    rep(i, 0, n - 1) {
        for (auto p : ma[i]) in[p.first]++;
    }
    rep(i, 0, n - 1) {
        sort(all(ma[i]), [&](const pii &a, const pii &b) { return a.first < b.first; });
    }
    int st = -1, end = -1;
    rep(i, 0, n - 1) {
        int out = sz(ma[i]);
        if (out == in[i] + 1) {
            if (st >= 0) return {};
            st = i;
        }
        if (out + 1 == in[i]) {
            if (end >= 0) return {};
            end = i;
        }
    }  // 这里是欧拉路径
    if (st < 0) {
        st = 0;
        rep(i, 0, n - 1) {
            if (!ma[i].empty()) {
                st = i;
                break;
            }
        }
    }
    // 分别处理欧拉回路和欧拉路径的情况
    vi path;
    vi head(n);
    auto dfs = [&](this auto &&dfs, int v) -> void {
        for (int &i = head[v]; i < sz(ma[v]);) {
            auto [to, id] = ma[v][i++];
            dfs(to);
        }
        path.push_back(v);
        return;
    };
    dfs(st);
    ranges::reverse(path);
    return path;
}