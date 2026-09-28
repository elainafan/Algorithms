// AC 自动机 + Fail 树：默认处理小写字母。
// 节点 0 是根，所有模式串必须在 build() 前插入。
// Fail 树使用显式栈迭代 DFS，不占用递归调用栈。

struct FailTree {
    vvl ma;
    vi dfn, siz, fdfn;

    FailTree() {}

    FailTree(const vi& fail) { init(fail); }

    void init(const vi& fail) {
        int n = sz(fail);
        ma.assign(n, {});
        rep(i, 1, n - 1) ma[fail[i]].push_back(i);

        dfn.assign(n, 0);
        siz.assign(n, 1);
        fdfn.clear();
        fdfn.reserve(n);
        if (!n) return;

        vi st;
        st.reserve(n);
        st.push_back(0);
        while (sz(st)) {
            int u = st.back();
            st.pop_back();
            dfn[u] = sz(fdfn);
            fdfn.push_back(u);
            frep(i, sz(ma[u]) - 1, 0) st.push_back((int)ma[u][i]);
        }

        frep(i, n - 1, 1) siz[fail[fdfn[i]]] += siz[fdfn[i]];
    }

    int size() { return sz(ma); }

    pii subtree(int u) { return {dfn[u], dfn[u] + siz[u] - 1}; }  // 返回子树对应的 DFS 序闭区间 [l, r]
};

struct ACAM {
    vector<array<int, 26>> son;
    vi fail;

    // n 是所有模式串长度总和的估计值，只用于预留空间。
    ACAM(int n = 0) {
        son.reserve(n + 1);
        fail.reserve(n + 1);
        son.push_back({});
        fail.push_back(0);
    }

    int insert(const string& s) {
        int u = 0;
        rep(i, 0, sz(s) - 1) {
            int c = s[i] - 'a';
            if (!son[u][c]) {
                son[u][c] = sz(son);
                son.push_back({});
                fail.push_back(0);
            }
            u = son[u][c];
        }
        return u;
    }  // 插入模式串，返回其终点节点

    void build() {
        vi que;
        que.reserve(sz(son));
        rep(c, 0, 25) {
            if (son[0][c]) que.push_back(son[0][c]);
        }

        int hd = 0;
        while (hd < sz(que)) {
            int u = que[hd++];
            rep(c, 0, 25) {
                int v = son[u][c];
                if (v) {
                    fail[v] = son[fail[u]][c];
                    que.push_back(v);
                } else {
                    son[u][c] = son[fail[u]][c];
                }
            }
        }
    }  // 建 fail 指针并补全转移，只调用一次

    int size() { return sz(son); }

    int go(int u, char c) { return son[u][c - 'a']; }
};

/*
用法：

int n;
cin >> n;
ACAM ac;
vi pos(n);
rep(i, 0, n - 1) {
    string s;
    cin >> s;
    pos[i] = ac.insert(s);
}
ac.build();

FailTree ft(ac.fail);

string text;
cin >> text;
vl cnt(ac.size());
int u = 0;
rep(i, 0, sz(text) - 1) {
    u = ac.go(u, text[i]);
    cnt[u]++;
}

// 沿 fail 指针向父亲汇总，cnt[pos[i]] 就是第 i 个模式串的出现次数。
frep(i, sz(ft.fdfn) - 1, 1) {
    u = ft.fdfn[i];
    cnt[ac.fail[u]] += cnt[u];
}
rep(i, 0, n - 1) cout << cnt[pos[i]] << endl;

// Fail 树子树区间为闭区间 [l, r]。
pii range = ft.subtree(pos[0]);
int l = range.first, r = range.second;
// 以 dfn 为下标的树状数组 / 线段树中查询 [l, r]，
// 就是在 Fail 树上查询 pos[0] 的整棵子树。

所有模式串必须在 build() 前插入，build() 只需调用一次。
字符范围默认是 'a' 到 'z'，模式串默认非空，节点编号和下标均不检查。
插入总复杂度 O(所有模式串长度总和)，build() 为 O(26 * 节点数)，
扫描文本为 O(|text|)，Fail 树建树和 DFS 序为 O(节点数)。
*/
