void solve() {
    int n;
    cin >> n;
    int B = sqrt(n);
    vector<ll> a(n);
    vector<ll> sum((n + B - 1) / B);
    vector<ll> tag((n + B - 1) / B);
    rep(i, 0, n - 1) cin >> a[i];
    auto build = [&]() { rep(i, 0, n - 1) sum[i / B] += a[i]; };

    // 单点加法
    auto add = [&](int pos, int val) {
        a[pos] += val;
        sum[pos / B] += val;
    };

    // 区间加法
    auto add = [&](int l, int r, int val) {
        int idl = l / B, idr = r / B;
        if (idl == idr) {
            rep(i, l, r) {
                a[i] += val;
                sum[idl] += val;
            }
        } else {
            rep(i, l, (idl + 1) * B - 1) {
                a[i] += val;
                sum[idl] += val;
            }
            rep(i, idl + 1, idr - 1) {
                tag[i] += val;
                sum[i] += 1LL * val * B;
            }
            rep(i, idr * B, r) {
                a[i] += val;
                sum[idr] += val;
            }
        }
    };

    // 区间查询
    auto query = [&](int l, int r) -> ll {
        ll res = 0;
        int idl = l / B, idr = r / B;
        if (idl == idr) {
            rep(i, l, r) res += a[i];
        } else {
            rep(i, l, (idl + 1) * B - 1) res += a[i];
            rep(i, idl + 1, idr - 1) res += sum[i];
            rep(i, idr * B, r) res += a[i];
        }
        return res;
    };

    // 带标记区间查询
    auto query = [&](int l, int r) {
        ll res = 0;
        int idl = l / B, idr = r / B;
        if (idl == idr) {
            rep(i, l, r) res += a[i] + tag[idl];
        } else {
            rep(i, l, (idl + 1) * B - 1) res += a[i] + tag[idl];
            rep(i, idl + 1, idr - 1) res += sum[i];
            rep(i, idr * B, r) res += a[i] + tag[idr];
        }
        return res;
    };
}