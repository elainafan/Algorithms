// 点分树，维护动态点集到最近标记点的距离，0-based。
// 无权树直接 add_edge(x, y)，有权树 add_edge(x, y, w)。

struct CentroidTree {
    static constexpr ll INF = (1LL << 60);

    int n;
    vector<vector<pii>> g;
    vi siz, par, vis, on;
    vector<vi> tree, anc;
    vector<vl> dis;
    vector<mset<ll>> st;

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

    void add_edge(int x, int y, int w = 1) {
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
            if (siz[y] * 2 > tot) return get_centroid(y, x, tot);
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
    }  // 建点分树

    void add(int x) {
        if (on[x]) return;
        on[x] = 1;
        for (int i = 0; i < sz(anc[x]); i++) {
            st[anc[x][i]].insert(dis[x][i]);
        }
    }  // 标记x

    void del(int x) {
        if (!on[x]) return;
        on[x] = 0;
        for (int i = 0; i < sz(anc[x]); i++) {
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
        ll ans = INF;
        for (int i = 0; i < sz(anc[x]); i++) {
            int c = anc[x][i];
            if (!st[c].empty()) ans = min(ans, *st[c].begin() + dis[x][i]);
        }
        return ans == INF ? -1 : ans;
    }  // 查询x到最近标记点的距离，没有标记点返回-1
};

/*
用法：

CentroidTree ct(n);
ct.add_edge(x, y);        // 无权边
ct.add_edge(x, y, w);     // 有权边
ct.work(root);

ct.add(x);                // 标记x
ct.del(x);                // 取消标记x
ct.toggle(x);             // 翻转x的标记
ct.query(x);              // 到最近标记点的距离，没有返回-1

ct.par[x] 是点分树父亲，ct.tree[x] 是点分树儿子。
ct.anc[x][i] 是 x 的第 i 层点分树祖先，ct.dis[x][i] 是对应距离。
*/
