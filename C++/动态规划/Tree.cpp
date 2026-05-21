vector<int> lastMarkedNodes(vector<vector<int>>& edges) {
    int n = edges.size() + 1;
    vector<vector<int>> ma(n);
    for (auto& p : edges) {
        ma[p[0]].push_back(p[1]);
        ma[p[1]].push_back(p[0]);
    }
    vector<tuple<int, int, int, int>> tem(n);  // 最长，最长编号，次长，次长编号
    auto dfs = [&](this auto&& dfs, int x, int pa) -> pair<int, int> {
        int m1 = 0, m2 = 0;
        int t1 = x, t2 = x;
        for (auto& p : ma[x]) {
            if (p == pa) continue;
            auto [a, b] = dfs(p, x);
            a++;
            if (a > m1) {
                m2 = m1, t2 = t1;
                m1 = a, t1 = b;
            } else if (a > m2) {
                m2 = a;
                t2 = b;
            }
        }
        tem[x] = make_tuple(m1, t1, m2, t2);
        return {m1, t1};
    };
    dfs(0, -1);
    vector<int> res(n);    // 代表从x往上走，不经过子树的最大长度
    vector<int> up_id(n);  // 条件同上，节点编号
    vector<int> res2(n);   // 最终节点
    auto dfs2 = [&](this auto&& dfs2, int x, int pa) -> void {
        if (pa == -1) {
            res2[x] = get<1>(tem[x]);
        } else {
            int tem2 = get<0>(tem[pa]), tem3 = get<0>(tem[pa]) + 1, id = get<1>(tem[pa]);
            if (get<0>(tem[x]) + 1 == tem2 && get<1>(tem[x]) == get<1>(tem[pa])) {
                tem3 = get<2>(tem[pa]) + 1;
                id = get<3>(tem[pa]);
            }
            if (res[pa] + 1 > tem3) id = up_id[pa];
            up_id[x] = id;
            res[x] = max(tem3, res[pa] + 1);
            if (get<0>(tem[x]) > res[x]) id = get<1>(tem[x]);
            res2[x] = id;
        }
        for (int& p : ma[x]) {
            if (p == pa) continue;
            dfs2(p, x);
        }
    };
    dfs2(0, -1);
    return res2;
}
// 换根DP求每个节点为根的子树最大深度及其对应叶子

// CF337D 给定多个标记点，求到所有标记点距离都不超过k的点数
void solve() {
    int n, x, y, m, k;
    cin >> n >> m >> k;
    vi tem(n);
    rep(i, 0, m - 1) {
        cin >> x;
        tem[x - 1] = 1;
    }
    vvi ma(n);
    rep(i, 1, n - 1) {
        cin >> x >> y;
        ma[x - 1].push_back(y - 1);
        ma[y - 1].push_back(x - 1);
    }
    const int NEG = -1e9;
    vi down(n, NEG), up(n, NEG);
    vi mx1(n, NEG), mx2(n, NEG), id(n, -1);
    auto upd = [&](int x, int val, int id2) -> void {
        if (val > mx1[x]) {
            mx2[x] = mx1[x];
            mx1[x] = val;
            id[x] = id2;
        } else if (val > mx2[x]) {
            mx2[x] = val;
        }
    };
    auto dfs1 = [&](this auto&& dfs1, int x, int pa) -> void {
        if (tem[x]) {
            upd(x, 0, -1);
        }
        for (int& p : ma[x]) {
            if (p == pa) continue;
            dfs1(p, x);
            if (down[p] != NEG) upd(x, down[p] + 1, p);
        }
        down[x] = mx1[x];
    };
    dfs1(0, -1);
    auto dfs2 = [&](this auto&& dfs2, int x, int pa) -> void {
        for (int& p : ma[x]) {
            if (p == pa) continue;
            if (up[x] != NEG) up[p] = max(up[p], up[x] + 1);
            int tem2 = (id[x] == p ? mx2[x] : mx1[x]);
            if (tem2 != NEG) up[p] = max(up[p], tem2 + 1);
            dfs2(p, x);
        }
    };
    dfs2(0, -1);
    int ans = 0;
    rep(i, 0, n - 1) { ans += (max(down[i], up[i]) <= k); }
    cout << ans << endl;
    return;
}