// 每个 solve() 独立使用

// 小写字母 Trie
void solve() {
    vector<array<int, 26>> son(1);
    vi endcnt(1);
    auto insert = [&](const string& s) -> void {
        int cur = 0;
        for (auto c : s) {
            c -= 'a';
            if (!son[cur][c]) son[cur][c] = sz(son), son.push_back({}), endcnt.push_back(0);
            cur = son[cur][c];
        }
        endcnt[cur]++;
    };  // 插入字符串 s
    auto query = [&](const string& s) -> int {
        int cur = 0;
        for (auto c : s) {
            c -= 'a';
            if (!son[cur][c]) return 0;
            cur = son[cur][c];
        }
        return endcnt[cur];
    };  // 查询字符串 s 的出现次数
}

// 非字母 Trie
void solve() {
    vector<map<int, int>> son(1);
    vi endcnt(1);
    auto insert = [&](const vi& a) -> void {
        int cur = 0;
        for (auto x : a) {
            if (!son[cur].count(x)) son[cur][x] = sz(son), son.emplace_back(), endcnt.push_back(0);
            cur = son[cur][x];
        }
        endcnt[cur]++;
    };  // 插入序列 a
    auto query = [&](const vi& a) -> int {
        int cur = 0;
        for (auto x : a) {
            if (!son[cur].count(x)) return 0;
            cur = son[cur][x];
        }
        return endcnt[cur];
    };  // 查询序列 a 的出现次数
}

// 01-Trie：插入、查询最大异或
void solve() {
    const int LOG = 30;
    vector<array<int, 2>> son(1);
    auto insert = [&](ll x) -> void {
        int cur = 0;
        frep(i, LOG, 0) {
            int b = x >> i & 1;
            if (!son[cur][b]) son[cur][b] = sz(son), son.push_back({});
            cur = son[cur][b];
        }
    };  // 插入整数 x
    auto query = [&](ll x) -> ll {
        int cur = 0;
        ll ans = 0;
        frep(i, LOG, 0) {
            int b = x >> i & 1;
            if (son[cur][b ^ 1]) ans |= 1LL << i, b ^= 1;
            cur = son[cur][b];
        }
        return ans;
    };  // 查询 max(x^y)
}
