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