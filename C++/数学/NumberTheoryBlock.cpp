// 数论分块 / 整除分块
// 核心：固定左端点 l，令 v = n / l，则所有满足 n / i = v 的 i 构成连续区间 [l, r]，
// 其中 r = n / v。总块数 O(sqrt(n))。

ll sum1(ll l, ll r) {
    return (ll)(((__int128)l + r) * (r - l + 1) / 2);
}

// 枚举所有 [l, r]，其中 floor(n / i) = v。
template <class F>
void each_floor_block(ll n, F work) {
    for (ll l = 1, r; l <= n; l = r + 1) {
        ll v = n / l;
        r = n / v;
        work(l, r, v);
    }
}

// sum_{i=1}^n floor(n / i)
ll floor_sum(ll n) {
    ll res = 0;
    each_floor_block(n, [&](ll l, ll r, ll v) {
        res += v * (r - l + 1);
    });
    return res;
}

// sum_{i=1}^n i * floor(n / i)
ll weighted_floor_sum(ll n) {
    ll res = 0;
    each_floor_block(n, [&](ll l, ll r, ll v) {
        res += v * sum1(l, r);
    });
    return res;
}

// 枚举所有 [l, r]，其中 floor(n / i) = vn 且 floor(m / i) = vm。
// 常用于 sum f(n / i, m / i) 之类的问题。
template <class F>
void each_floor_block(ll n, ll m, F work) {
    ll lim = min(n, m);
    for (ll l = 1, r; l <= lim; l = r + 1) {
        ll vn = n / l, vm = m / l;
        r = min(n / vn, m / vm);
        work(l, r, vn, vm);
    }
}

/*
使用示例：

1. 计算 sum_{i=1}^n floor(n / i)

ll ans = floor_sum(n);

2. 计算 sum_{i=1}^n i * floor(n / i)

ll ans = weighted_floor_sum(n);

3. 自定义贡献，例如 sum_{i=1}^n (n / i)^2

ll ans = 0;
each_floor_block(n, [&](ll l, ll r, ll v) {
    ans += v * v * (r - l + 1);
});

4. 双变量分块，例如枚举 floor(n / i) 和 floor(m / i)

each_floor_block(n, m, [&](ll l, ll r, ll vn, ll vm) {
    // i in [l, r] 时，n / i = vn，m / i = vm
});
*/
