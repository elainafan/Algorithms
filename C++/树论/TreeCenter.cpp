// 无权树的中心，0-based。
// 返回使到其他点的最大距离最小的所有点，答案有一个或两个点。
vi tree_centers(const vvi& ma) {
    int n = sz(ma);
    if (n <= 2) {
        vi center(n);
        iota(all(center), 0);
        return center;
    }

    vi deg(n);
    queue<int> q;
    rep(x, 0, n - 1) {
        deg[x] = sz(ma[x]);
        if (deg[x] == 1) q.push(x);
    }

    int remain = n;
    while (remain > 2) {
        int cnt = q.size();
        remain -= cnt;
        while (cnt--) {
            int x = q.front();
            q.pop();
            for (int y : ma[x]) {
                if (--deg[y] == 1) q.push(y);
            }
        }
    }

    vi center;
    while (!q.empty()) {
        center.push_back(q.front());
        q.pop();
    }
    sort(all(center));
    return center;
}

/*
用法：

vvi ma(n);
// 加入 n - 1 条无向边
vi center = tree_centers(ma);

// center.size() 为 1 或 2。
// 树的中心也就是任意一条直径的中点。
*/
