/*
参考：https://codeforces.com/blog/entry/81661
点分树：维护到最近标记点的距离，点编号为 0-based。
无权树 add_edge(x, y)，有权树 add_edge(x, y, w)。
边权非负，边权和距离为 ll，任意简单路径的长度需在 ll 范围内。

文章中 best[c] 表示：点分树以 c 为根的子树内，已标记点到 c 的最小原树距离。
标记 x 时，沿 x 的点分树祖先 c 更新 best[c]；查询时取 min(best[c]+dist(x,c))。
本板用 st[c] 保存这些距离，最小值就是 best[c]，因此还支持取消标记。

对 x 和最近标记点 y，它们在点分树上的 LCA 位于原树路径 x-y 上，该层候选恰好达到最短距离。
其他祖先对应的绕行距离不会更短，所以枚举祖先取最小值即可；这里用到了边权非负。
dist 始终是原树距离。本板建树时预存 anc / dis，更新查询时直接枚举，省去 LCA 求距离。

建树 O(n log n)，标记 / 取消标记 O(log^2 n)，查询 O(log n)，空间 O(n log n)。
*/

struct CentroidTree {
    int n;
    vector<vector<pll>> g;
    vi siz, par, vis, on;
    vector<vi> tree, anc;
    vvl dis;
    vector<multiset<ll>> st;

    CentroidTree(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n, {});
        siz.assign(n, 0);
        par.assign(n, -1);
        vis.assign(n, 0);
        on.assign(n, 0);
        tree.assign(n, {});
        anc.assign(n, {});
        dis.assign(n, {});
        st.assign(n, {});
    }

    void add_edge(int x, int y, ll w = 1) {
        g[x].push_back({y, w});
        g[y].push_back({x, w});
    }

    int get_size(int x, int p) {
        siz[x] = 1;
        for (auto [y, w] : g[x]) {
            if (y == p || vis[y]) continue;
            siz[x] += get_size(y, x);
        }
        return siz[x];
    }

    int get_centroid(int x, int p, int tot) {
        for (auto [y, w] : g[x]) {
            if (y == p || vis[y]) continue;
            if (siz[y] > tot / 2) return get_centroid(y, x, tot);
        }
        return x;
    }

    void collect(int x, int p, ll d, int c) {
        anc[x].push_back(c);
        dis[x].push_back(d);
        for (auto [y, w] : g[x]) {
            if (y == p || vis[y]) continue;
            collect(y, x, d + w, c);
        }
    }  // 记录x到每层点分树祖先c的距离

    void build(int x, int p) {
        int tot = get_size(x, -1);
        int c = get_centroid(x, -1, tot);
        par[c] = p;
        if (p != -1) tree[p].push_back(c);
        vis[c] = 1;
        collect(c, -1, 0, c);
        for (auto [y, w] : g[c]) {
            if (vis[y]) continue;
            build(y, c);
        }
    }

    void work(int root = 0) {
        siz.assign(n, 0);
        par.assign(n, -1);
        vis.assign(n, 0);
        on.assign(n, 0);
        tree.assign(n, {});
        anc.assign(n, {});
        dis.assign(n, {});
        st.assign(n, {});
        if (n) build(root, -1);
    }  // 建点分树；可以重复调用，会清空已有标记；root 是原树遍历起点

    void add(int x) {
        if (on[x]) return;
        on[x] = 1;
        rep(i, 0, sz(anc[x]) - 1) {
            st[anc[x][i]].insert(dis[x][i]);
        }
    }  // 沿点分树祖先维护到标记点的原树距离，重复标记无影响

    void del(int x) {
        if (!on[x]) return;
        on[x] = 0;
        rep(i, 0, sz(anc[x]) - 1) {
            int c = anc[x][i];
            auto it = st[c].find(dis[x][i]);
            if (it != st[c].end()) st[c].erase(it);
        }
    }  // 取消标记x

    void toggle(int x) {
        if (on[x])
            del(x);
        else
            add(x);
    }  // 翻转x的标记状态

    ll query(int x) const {
        ll ans = -1;
        rep(i, 0, sz(anc[x]) - 1) {
            int c = anc[x][i];
            if (st[c].empty()) continue;
            ll d = *st[c].begin();
            if (d > LLONG_MAX - dis[x][i]) continue;  // 经 c 绕行的候选距离可能超出 ll
            ll cur = d + dis[x][i];
            if (ans == -1 || cur < ans) ans = cur;
        }
        return ans;
    }  // 查询x到最近标记点的距离，没有标记点返回-1
};

/*
用法：

CentroidTree ct(n);
ct.add_edge(x, y);        // 无权边
ct.add_edge(x, y, w);     // 非负权边，w 为 ll
ct.work(root);           // root 是原树遍历起点，点分树的根由重心划分确定

ct.add(x);                // 标记x
ct.del(x);                // 取消标记x
ct.toggle(x);             // 翻转x的标记
ct.query(x);              // 到最近标记点的距离，没有返回-1

ct.par[x] 是点分树父亲，ct.tree[x] 是点分树儿子。
ct.anc[x] 按点分树根到 x 的顺序保存祖先（包含 x），ct.dis[x][i] 是对应的原树距离。
初始没有标记点；若题目指定初始标记点 s，建树后调用 ct.add(s)。

若题目只有标记和查询，可以把 st 改为 best 数组：
    初始 best[c] = -1；标记 x 时，沿 anc[x] 用 dis[x][i] 更新 best[c] 的最小值。
    查询仍枚举同一条祖先链，用 best[c]+dis[x][i] 更新答案，跳过 best[c] = -1 的位置。
    此时标记和查询均为 O(log n)；需要取消标记时使用上面的 multiset 版本。
*/
