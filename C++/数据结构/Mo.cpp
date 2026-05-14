// 普通莫队示例：静态区间不同数个数
// 用法：vi ans = mo_distinct(a, qs);
// a 是原数组，qs[i] = {l, r}，下标 0-based 且区间闭合 [l, r]。
struct MoQuery {
    int l, r, id;
};

vi mo_distinct(vi a, vector<pii> qs) {
    int n = sz(a), q = sz(qs);
    vi b = a;
    sort(all(b));
    b.erase(unique(all(b)), b.end());
    for (int& x : a) x = lower_bound(all(b), x) - b.begin();

    int B = max(1, (int)sqrt(n));
    vector<MoQuery> query(q);
    rep(i, 0, q - 1) query[i] = {qs[i].first, qs[i].second, i};
    sort(all(query), [&](const MoQuery& x, const MoQuery& y) {
        int bx = x.l / B, by = y.l / B;
        if (bx != by) return bx < by;
        return bx & 1 ? x.r > y.r : x.r < y.r;
    });

    vi cnt(sz(b)), ans(q);
    int L = 0, R = -1, cur = 0;
    auto add = [&](int p) {
        if (++cnt[a[p]] == 1) cur++;
    };
    auto del = [&](int p) {
        if (--cnt[a[p]] == 0) cur--;
    };

    for (auto [l, r, id] : query) {
        while (L > l) add(--L);
        while (R < r) add(++R);
        while (L < l) del(L++);
        while (R > r) del(R--);
        ans[id] = cur;
    }
    return ans;
}
