// 无权树直径，0-based，ma 一般是 vvl 邻接表
int ans = 0, L = 0, R = 0;
auto dfs = [&](this auto&& dfs, int x, int pa) -> pii {
    int tem = 0, id = x;  // 从 x 往下走，当前最长链长度和链尾端点
    for (auto& p : ma[x]) {
        if (p == pa) continue;
        auto [dep, to] = dfs(p, x);
        int tem2 = dep + 1;
        // if (s[x] == s[p]) continue;
        if (ans < tem + tem2) {
            ans = tem + tem2;
            L = id, R = to;
        }
        if (tem < tem2) {
            tem = tem2;
            id = to;
        }
    }
    return {tem, id};
};
dfs(0, -1);
// ans 是直径边数，如果是点数需要加一；L, R 是直径对应的两个端点

vi fa(sz(ma), -1), path;
auto find_path = [&](this auto&& find_path, int x, int pa) -> bool {
    if (x == R) return true;
    for (auto& y : ma[x]) {
        if (y == pa) continue;
        fa[y] = x;
        if (find_path(y, x)) return true;
    }
    return false;
};
find_path(L, -1);
for (int x = R; x != -1; x = fa[x]) path.push_back(x);
reverse(all(path));
// path 是从 L 到 R 的整条直径路径
