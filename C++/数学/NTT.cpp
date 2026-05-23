// NTT求卷积，模数998244353，原根3
// convolution(a, b)返回c，其中c[k]=sum(a[i]*b[j])，i+j=k
constexpr int MOD = 998244353;
constexpr int G = 3;

int qpow(int x, int y) {
    int res = 1;
    while (y) {
        if (y & 1) res = 1LL * res * x % MOD;
        x = 1LL * x * x % MOD;
        y >>= 1;
    }
    return res;
}

void ntt(vi& a, bool inv) {
    int n = sz(a);
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        int wlen = qpow(G, (MOD - 1) / len);
        if (inv) wlen = qpow(wlen, MOD - 2);
        for (int i = 0; i < n; i += len) {
            int w = 1;
            rep(j, 0, len / 2 - 1) {
                int u = a[i + j];
                int v = 1LL * a[i + j + len / 2] * w % MOD;
                a[i + j] = u + v < MOD ? u + v : u + v - MOD;
                a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + MOD;
                w = 1LL * w * wlen % MOD;
            }
        }
    }
    if (inv) {
        int inv_n = qpow(n, MOD - 2);
        for (int& x : a) x = 1LL * x * inv_n % MOD;
    }
}

vi convolution(vi a, vi b) {
    if (a.empty() || b.empty()) return {};
    int need = sz(a) + sz(b) - 1;
    int n = 1;
    while (n < need) n <<= 1;
    a.resize(n);
    b.resize(n);
    ntt(a, false);
    ntt(b, false);
    for (int i = 0; i < n; i++) a[i] = 1LL * a[i] * b[i] % MOD;
    ntt(a, true);
    a.resize(need);
    return a;
}

vi polymul(vi& a, vi& b, int lim) {
    vi c = convolution(a, b);
    if (sz(c) > lim) c.resize(lim);
    return c;
}

vi polypow(vi a, int y, int lim) {
    vi res(1, 1);
    while (y > 0) {
        if (y & 1) res = polymul(res, a, lim);
        y >>= 1;
        if (y) a = polymul(a, a, lim);
    }
    return res;
}

/*
使用示例：

void solve() {
    int n, m;
    cin >> n >> m;
    vi a(n + 1), b(m + 1);
    rep(i, 0, n) cin >> a[i];
    rep(i, 0, m) cin >> b[i];
    vi c = convolution(a, b);
    rep(i, 0, n + m) cout << c[i] << " \n"[i == n + m];
}

输入：
2 1
1 2 3
4 5

输出：
4 13 22 15
*/
