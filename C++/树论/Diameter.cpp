int ans = 0;
auto dfs = [&](this auto&& dfs, int x, int pa) -> int {
    int tem = 0;
    for (int& p : ma[x]) {
        if (p == pa) continue;
        int tem2 = dfs(p, x) + 1;
        // if (s[x] == s[p]) continue;
        ans = max(ans, tem + tem2);
        tem = max(tem, tem2);
    }
    return tem;
};
dfs(0, -1);
return ans; // 这里求的是边的个数，如果是点的个数需要加一