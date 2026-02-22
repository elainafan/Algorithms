// 自定义 pair 的哈希函数，用于 unordered_map
struct pair_hash {
    inline size_t operator()(const pair<ll, ll>& v) const {
        // 简单的组合哈希策略
        return v.first * 31 + v.second;
    }
};

// 双哈希类
struct StringHash {
    // 第一组参数
    static const ll MOD1 = 1e9 + 7;
    static const ll BASE1 = 131;

    // 第二组参数
    static const ll MOD2 = 1e9 + 9;
    static const ll BASE2 = 13331;

    vector<ll> h1, p1;
    vector<ll> h2, p2;

    StringHash(const string& s) {
        int n = s.size();
        h1.resize(n + 1, 0);
        p1.resize(n + 1, 1);
        h2.resize(n + 1, 0);
        p2.resize(n + 1, 1);

        for (int i = 0; i < n; i++) {
            // 哈希 1
            h1[i + 1] = (h1[i] * BASE1 + s[i]) % MOD1;
            p1[i + 1] = (p1[i] * BASE1) % MOD1;

            // 哈希 2
            h2[i + 1] = (h2[i] * BASE2 + s[i]) % MOD2;
            p2[i + 1] = (p2[i] * BASE2) % MOD2;
        }
    }

    // 获取 s[l...r] 的双哈希值 (闭区间，0-indexed)
    pair<ll, ll> query(int l, int r) {
        ll v1 = (h1[r + 1] - h1[l] * p1[r - l + 1]) % MOD1;
        if (v1 < 0) v1 += MOD1;  // 关键：处理负数

        ll v2 = (h2[r + 1] - h2[l] * p2[r - l + 1]) % MOD2;
        if (v2 < 0) v2 += MOD2;  // 关键：处理负数

        return {v1, v2};
    }
};

// 使用：unordered_map<pair<ll,ll>,int,pair_hash>ma;