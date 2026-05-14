// 虚树，依赖 HLD 的 dfn/siz/dep/lca
// 用法：HLD hld(n); hld.work(root); VirtualTree vt(n, hld);
// vi nodes = vt.build(key);  // key 是关键点，返回虚树点集，nodes[0] 是虚树根
// vt.g[u]                   // u 的虚树儿子，pair 是 {儿子, 原树距离}
// vt.used                   // 当前虚树点集，主要内部清空用
struct VirtualTree {
    int n;
    HLD& hld;
    vector<vector<pii>> g;
    vi used;

    VirtualTree(int n, HLD& hld) : n(n), hld(hld), g(n) {}

    bool is_ancestor(int x, int y) {
        return hld.dfn[x] <= hld.dfn[y] && hld.dfn[y] < hld.dfn[x] + hld.siz[x];
    }

    vi build(vi key) {
        for (int x : used) g[x].clear();
        used.clear();
        if (key.empty()) return {};

        sort(all(key), [&](int x, int y) { return hld.dfn[x] < hld.dfn[y]; });
        key.erase(unique(all(key)), key.end());
        int m = sz(key);
        rep(i, 0, m - 2) key.push_back(hld.lca(key[i], key[i + 1]));
        sort(all(key), [&](int x, int y) { return hld.dfn[x] < hld.dfn[y]; });
        key.erase(unique(all(key)), key.end());

        vi st;
        for (int x : key) {
            while (!st.empty() && !is_ancestor(st.back(), x)) st.pop_back();
            if (!st.empty()) {
                g[st.back()].push_back({x, hld.dep[x] - hld.dep[st.back()]});
            }
            st.push_back(x);
        }
        used = key;
        return used;
    }
};
