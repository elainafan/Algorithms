// 数论分块
// floor_sum(n) 返回 sum_{i=1}^n floor(n / i)。
// divide_block(n) 展示分块循环：在 [l, r] 内 floor(n / i) 相同。
ll floor_sum(ll n) {
    ll res = 0;
    for (ll l = 1, r; l <= n; l = r + 1) {
        ll v = n / l;
        r = n / v;
        res += v * (r - l + 1);
    }
    return res;
}

void divide_block(ll n) {
    for (ll l = 1, r; l <= n; l = r + 1) {
        ll v = n / l;
        r = n / v;
        // 此时 i in [l, r] 的 n / i 都等于 v
    }
}
