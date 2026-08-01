// 每个 solve() 独立使用

// 小写字母 Trie
void solve() {
    vector<array<int, 26>> son(1);
    vi cnt(1), endcnt(1);

    auto insert = [&](const string& s) -> void {
        int cur = 0;
        cnt[cur]++;
        for (auto c : s) {
            c -= 'a';
            if (!son[cur][c]) {
                son[cur][c] = sz(son);
                son.push_back({}), cnt.push_back(0), endcnt.push_back(0);
            }
            cur = son[cur][c], cnt[cur]++;
        }
        endcnt[cur]++;
    };  // 插入字符串 s
    auto erase = [&](const string& s) -> void {
        int cur = 0;
        cnt[cur]--;
        for (auto c : s) cur = son[cur][c - 'a'], cnt[cur]--;
        endcnt[cur]--;
    };  // 删除一个已存在的字符串 s
    auto find = [&](const string& s) -> int {
        int cur = 0;
        for (auto c : s) {
            c -= 'a';
            if (!son[cur][c]) return -1;
            cur = son[cur][c];
        }
        return cur;
    };  // 返回 s 对应的节点，不存在则返回 -1
    auto cntword = [&](const string& s) -> int {
        int cur = find(s);
        return cur == -1 ? 0 : endcnt[cur];
    };  // 查询字符串 s 的出现次数
    auto cntprefix = [&](const string& s) -> int {
        int cur = find(s);
        return cur == -1 ? 0 : cnt[cur];
    };  // 查询以 s 为前缀的字符串数量
    auto findpre = [&](const string& s) -> string {
        int cur = 0;
        rep(i, 0, sz(s) - 1) {
            int c = s[i] - 'a';
            if (!son[cur][c]) return string();
            cur = son[cur][c];
            if (endcnt[cur]) return s.substr(0, i + 1);
        }
        return string();
    };  // 返回 s 的最短已插入前缀，没有则返回空串

    // insert(s), erase(s), cntword(s), cntprefix(s), findpre(s)
}

// 非字母 Trie：维护 vector<int>
void solve() {
    vector<map<int, int>> son(1);
    vi cnt(1), endcnt(1);

    auto insert = [&](const vi& a) -> void {
        int cur = 0;
        cnt[cur]++;
        for (auto x : a) {
            if (!son[cur].count(x)) {
                son[cur][x] = sz(son);
                son.emplace_back(), cnt.push_back(0), endcnt.push_back(0);
            }
            cur = son[cur][x], cnt[cur]++;
        }
        endcnt[cur]++;
    };  // 插入序列 a
    auto erase = [&](const vi& a) -> void {
        int cur = 0;
        cnt[cur]--;
        for (auto x : a) cur = son[cur][x], cnt[cur]--;
        endcnt[cur]--;
    };  // 删除一个已存在的序列 a
    auto find = [&](const vi& a) -> int {
        int cur = 0;
        for (auto x : a) {
            auto it = son[cur].find(x);
            if (it == son[cur].end()) return -1;
            cur = it->second;
        }
        return cur;
    };  // 返回 a 对应的节点，不存在则返回 -1
    auto cntword = [&](const vi& a) -> int {
        int cur = find(a);
        return cur == -1 ? 0 : endcnt[cur];
    };  // 查询序列 a 的出现次数
    auto cntprefix = [&](const vi& a) -> int {
        int cur = find(a);
        return cur == -1 ? 0 : cnt[cur];
    };  // 查询以 a 为前缀的序列数量
    auto findpre = [&](const vi& a) -> vi {
        int cur = 0;
        rep(i, 0, sz(a) - 1) {
            auto it = son[cur].find(a[i]);
            if (it == son[cur].end()) return vi();
            cur = it->second;
            if (endcnt[cur]) return vi(a.begin(), a.begin() + i + 1);
        }
        return vi();
    };  // 返回 a 的最短已插入前缀，没有则返回空序列

    // insert(a), erase(a), cntword(a), cntprefix(a), findpre(a)
}

// 典题：同时限制前缀和后缀，求最大的单词下标（LeetCode 745）
// 插入所有 suffix + '{' + s，建树 O(sum |s|^2)，查询 O(|pre|+|suf|)
void solve() {
    int n;
    cin >> n;
    vector<array<int, 27>> son(1);
    vi idx(1, -1);

    auto insert = [&](const string& s, int id) -> void {
        string t = s + "{" + s;
        rep(i, 0, sz(s)) {
            int cur = 0;
            rep(j, i, sz(t) - 1) {
                int c = t[j] == '{' ? 26 : t[j] - 'a';
                if (!son[cur][c]) {
                    son[cur][c] = sz(son);
                    son.push_back({}), idx.push_back(-1);
                }
                cur = son[cur][c], idx[cur] = id;
            }
        }
    };  // 插入 s 的所有后缀组合并维护最大下标
    auto query = [&](const string& pre, const string& suf) -> int {
        string t = suf + "{" + pre;
        int cur = 0;
        for (auto c : t) {
            c = c == '{' ? 26 : c - 'a';
            if (!son[cur][c]) return -1;
            cur = son[cur][c];
        }
        return idx[cur];
    };  // 查询同时满足前缀 pre 和后缀 suf 的最大下标

    rep(i, 0, n - 1) {
        string s;
        cin >> s;
        insert(s, i);
    }
    int q;
    cin >> q;
    while (q--) {
        string pre, suf;
        cin >> pre >> suf;
        cout << query(pre, suf) << endl;
    }
}
