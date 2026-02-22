constexpr int MOD = 1e9 + 7;
using matrix = vector<vector<ll>>;
matrix mul(matrix& a, matrix& b) {
    int n = a.size(), m = b[0].size();
    matrix c = matrix(n, vector<ll>(m));
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < a[i].size(); k++) {
            if (a[i][k] == 0) continue;
            for (int j = 0; j < m; j++) {
                c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % MOD;
            }
        }
    }
    return c;
}
matrix pow_mul(matrix a, int n, matrix& f0) {
    matrix res = f0;
    while (n) {
        if (n & 1) res = mul(a, res);
        a = mul(a, a);
        n >>= 1;
    }
    return res;
}