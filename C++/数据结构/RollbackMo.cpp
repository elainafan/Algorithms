// 回滚莫队示例：静态区间最高出现次数
// 用法：vi ans = rollback_mo_max_freq(a, qs);
// a 是原数组，qs[i] = {l, r}，下标 0-based 且区间闭合 [l, r]。
struct RollbackQuery {
    int l, r, id;
};

vi rollback_mo_max_freq(vi a, vector<pii> qs) {
    int n = sz(a), q = sz(qs);
    vi b = a;
    sort(all(b));
    b.erase(unique(all(b)), b.end());
    for (int& x : a) x = lower_bound(all(b), x) - b.begin();

    int B = max(1, (int)sqrt(n));
    vector<vector<RollbackQuery>> bucket((n + B - 1) / B);
    rep(i, 0, q - 1) {
        auto [l, r] = qs[i];
        bucket[l / B].push_back({l, r, i});
    }

    vi ans(q), cnt(sz(b));
    rep(id, 0, sz(bucket) - 1) {
        auto& v = bucket[id];
        sort(all(v), [&](const RollbackQuery& x, const RollbackQuery& y) { return x.r < y.r; });

        int br = min(n - 1, (id + 1) * B - 1);
        int R = br, cur = 0;
        fill(all(cnt), 0);

        auto add = [&](int p) {
            cnt[a[p]]++;
            cur = max(cur, cnt[a[p]]);
        };

        for (auto [l, r, qid] : v) {
            if (r <= br) {
                int res = 0;
                rep(i, l, r) {
                    cnt[a[i]]++;
                    res = max(res, cnt[a[i]]);
                }
                rep(i, l, r) cnt[a[i]]--;
                ans[qid] = res;
                continue;
            }

            while (R < r) add(++R);
            int old = cur;
            rep(i, l, br) add(i);
            ans[qid] = cur;
            rep(i, l, br) cnt[a[i]]--;
            cur = old;
        }
    }
    return ans;
}
