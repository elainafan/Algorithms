// FFT求卷积，适合普通整数系数；如果需要取模且模数友好，优先用NTT。
// convolution_ll(a, b)返回c，其中c[k]=sum(a[i]*b[j])，i+j=k
using cd = complex<double>;
const double PI = acos(-1.0);

void fft(vector<cd>& a, bool inv) {
    int n = sz(a);
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (inv ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            rep(j, 0, len / 2 - 1) {
                cd u = a[i + j];
                cd v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (inv) {
        for (cd& x : a) x /= n;
    }
}

vl convolution_ll(vl a, vl b) {
    if (a.empty() || b.empty()) return {};
    int need = sz(a) + sz(b) - 1;
    int n = 1;
    while (n < need) n <<= 1;
    vector<cd> fa(all(a)), fb(all(b));
    fa.resize(n);
    fb.resize(n);
    fft(fa, false);
    fft(fb, false);
    rep(i, 0, n - 1) fa[i] *= fb[i];
    fft(fa, true);
    vl res(need);
    rep(i, 0, need - 1) res[i] = llround(fa[i].real());
    return res;
}

/*
使用示例：

void solve() {
    int n, m;
    cin >> n >> m;
    vl a(n + 1), b(m + 1);
    rep(i, 0, n) cin >> a[i];
    rep(i, 0, m) cin >> b[i];
    vl c = convolution_ll(a, b);
    rep(i, 0, n + m) cout << c[i] << " \n"[i == n + m];
}

输入：
2 1
1 2 3
4 5

输出：
4 13 22 15
*/
