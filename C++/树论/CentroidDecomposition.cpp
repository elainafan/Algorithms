/*
点分治：统计无权树上距离等于 k 的不同顶点无序点对。
放在 solve() 内，沿用 1.cpp 的宏；需要 C++23 的 this auto&& 递归 lambda。
前置：n >= 1，k >= 0，vvl ma(n) 已建好，点编号为 0-based。
1.cpp 的 rep 包含右端点，读入 n-1 条边时使用 rep(i, 1, n-1)。
时间 O(n log n)，空间 O(n)。
*/
vl siz(n), vis(n), cnt(n);
ll ans = 0;

auto getsiz = [&](this auto&& getsiz, int x, int pa) -> void {
    ll tem = 1;
    for (auto& p : ma[x]) {
        if (p == pa || vis[p]) continue;
        getsiz(p, x);
        tem += siz[p];
    }
    siz[x] = tem;
    return;
};  // 当前未删除连通块的子树大小

auto getcen = [&](this auto&& getcen, int x, int pa, int tot) -> ll {
    for (auto& p : ma[x]) {
        if (p == pa || vis[p]) continue;
        if (siz[p] > tot / 2) return getcen(p, x, tot);
    }
    return x;
};  // 沿大小超过 tot/2 的子树向下走，找到当前连通块的重心

auto getdis = [&](this auto&& getdis, int x, int fa, int d, vl& dis) -> void {
    if (d > k) return;
    dis.emplace_back(d);
    for (auto& p : ma[x]) {
        if (p == fa || vis[p]) continue;
        getdis(p, x, d + 1, dis);
    }
};  // 收集当前分支内各点到重心的距离，只保留 d <= k

auto dfs = [&](this auto&& dfs, int x) -> void {
    getsiz(x, -1);
    int c = getcen(x, -1, siz[x]);
    vis[c] = 1;

    vi use;
    cnt[0] = 1;  // 重心本身，负责统计以重心为一个端点的路径
    use.emplace_back(0);

    for (int y : ma[c]) {
        if (vis[y]) continue;
        vi dis;
        getdis(y, c, 1, dis);

        for (int d : dis) ans += cnt[k - d];  // 先查询：只与之前的分支配对
        for (int d : dis) {
            if (!cnt[d]) use.emplace_back(d);
            ++cnt[d];
        }  // 再合并：避免把同一分支内的点错误地配对
    }

    for (int d : use) cnt[d] = 0;  // 只清空本层用过的深度，递归前清理，避免各层互相影响
    for (int y : ma[c]) {
        if (!vis[y]) dfs(y);
    }
};

if (k < n) dfs(0);  // 无权树的最大距离为 n-1，k >= n 时 ans 保持为 0
// ans 为所求点对数；若题目允许两个端点相同，k=0 时额外加 n。
