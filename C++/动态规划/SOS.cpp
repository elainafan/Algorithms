// 设 w 为 a[i] 的二进制最大长度
// 返回一个长为 2^w 的数组 f，其中 f[S] 表示 a 中是 S 的子集的元素个数（把二进制数视作集合）
// 时间复杂度 O(n + U log U)，其中 U = max(a)
vl sos_dp(vl& a) {
    ll maxx = *max_element(all(a));
    int n = bit_width((uint64_t)maxx);  // 二进制长度上限

    vl dp(1 << n);
    for (auto& p : a) dp[p]++;

    rep(i, 0, n - 1) {
        rep(j, 0, (1 << n) - 1) {
            if (j >> i & 1) dp[j] += dp[j ^ (1 << i)];
        }
    }

    return dp;
}

// 设 w 为 a[i] 的二进制最大长度
// 返回一个长为 2^w 的数组 f，其中 f[S] 表示 a 中是 S 的超集的元素个数（把二进制数视作集合）
// 时间复杂度 O(n + U log U)，其中 U = max(a)
vl sos_dp(vl& a) {
    ll maxx = *max_element(all(a));
    int n = bit_width((uint64_t)maxx);  // 二进制长度上限

    vl dp(1 << n);
    for (auto& p : a) dp[p]++;

    rep(i, 0, n - 1) {
        rep(j, 0, (1 << n) - 1) {
            if (j >> i & 1) continue;
            dp[j] += dp[j | (1 << i)];
        }
    }

    return dp;
}