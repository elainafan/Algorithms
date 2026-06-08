constexpr int MOD = 1e9 + 7;
constexpr int MX = 1e5 + 1;
ll F[MX];      // 预处理阶乘
ll INV_F[MX];  // 预处理逆元
ll mul(ll x, ll y) { return x * y % MOD; }
ll qpow(ll x, int n) {
    ll res = 1;
    for (; n; n >>= 1) {
        if (n % 2) res = res * x % MOD;
        x = x * x % MOD;
    }
    return res;
}
auto init = [] {
    F[0] = 1;
    for (int i = 1; i < MX; i++) F[i] = F[i - 1] * i % MOD;  // 预处理阶乘
    INV_F[MX - 1] = qpow(F[MX - 1], MOD - 2);
    for (int i = MX - 1; i; i--) {
        INV_F[i - 1] = INV_F[i] * i % MOD;
    }  // 预处理逆元
    return 0;
}();
// 计算C(n,m),即从n个数中取m个数
ll comb(int n, int m) { return m < 0 || m > n ? 0 : F[n] * INV_F[m] % MOD * INV_F[n - m] % MOD; }

constexpr int MX = 31;
ll c[MX][MX];  // 即为C(n,m),从n个数中取m个数
auto init = [] {
    for (int i = 0; i < MX; i++) {
        c[i][0] = c[i][i] = 1;
        for (int j = 1; j < i; j++) {
            c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
        }
    }
    return 0;
}();  // 适用于MX较小的情况

ll comb(int n, int m) {
    if (n < 0 || m < 0 || m > n) return 0;
    return c[n][m];
}

namespace Stirling {
/*
用法：

如果保留 namespace：
   auto s1 = Stirling::first_unsigned(n);
   auto ss1 = Stirling::first_signed(n);
   auto s2 = Stirling::second(n);
   ll p = Stirling::falling_power(x, k);

如果想像平时一样直接调，在板子后面加：
   using namespace Stirling;
   然后直接写 first_unsigned(n), first_signed(n), second(n), falling_power(x, k)。

1. 下降幂
   falling_power(x, k) = x^{\underline{k}} = x(x-1)...(x-k+1) mod MOD
   例：falling_power(n, k) 表示从 n 个不同元素中有顺序选 k 个，即 A(n,k)。

2. 第一类斯特林数
   auto s1 = first_unsigned(n);
   s1[i][j] = c(i,j)，无符号第一类斯特林数，表示 i 个元素分成 j 个非空圆排列的方案数。

   auto ss1 = first_signed(n);
   ss1[i][j] = s(i,j)，带符号第一类斯特林数，满足：
   x^{\underline{i}} = sum_{j=0}^i s(i,j) x^j
   若需要无符号值，则 c(i,j) = (-1)^{i-j} s(i,j)。

3. 第二类斯特林数
   auto s2 = second(n);
   s2[i][j] = S(i,j)，表示 i 个有标号元素分成 j 个非空无标号集合的方案数。
   常见公式：
   x^i = sum_{j=0}^i S(i,j) x^{\underline{j}}

复杂度：
   first_unsigned / first_signed / second 都是 O(n^2) 预处理，O(1) 查询。
   falling_power 是 O(k)。
*/

ll norm(ll x) {
    x %= MOD;
    if (x < 0) x += MOD;
    return x;
}

// 下降幂 x^{\underline{k}} = x(x-1)...(x-k+1)
ll falling_power(ll x, int k) {
    ll res = 1;
    for (int i = 0; i < k; i++) {
        res = res * norm(x - i) % MOD;
    }
    return res;
}

// 无符号第一类斯特林数 c(n,k)：n个元素分成k个圆排列
// c(n,k)=c(n-1,k-1)+(n-1)c(n-1,k)
vector<vector<ll>> first_unsigned(int n) {
    vector<vector<ll>> s(n + 1, vector<ll>(n + 1));
    s[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            s[i][j] = (s[i - 1][j - 1] + (i - 1LL) * s[i - 1][j]) % MOD;
        }
    }
    return s;
}

// 带符号第一类斯特林数 s(n,k)：x^{\underline{n}} = sum s(n,k)x^k
// s(n,k)=s(n-1,k-1)-(n-1)s(n-1,k)
vector<vector<ll>> first_signed(int n) {
    vector<vector<ll>> s(n + 1, vector<ll>(n + 1));
    s[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            s[i][j] = norm(s[i - 1][j - 1] - (i - 1LL) * s[i - 1][j]);
        }
    }
    return s;
}

// 第二类斯特林数 S(n,k)：n个有标号元素分成k个非空无标号集合
// S(n,k)=S(n-1,k-1)+kS(n-1,k)
vector<vector<ll>> second(int n) {
    vector<vector<ll>> s(n + 1, vector<ll>(n + 1));
    s[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            s[i][j] = (s[i - 1][j - 1] + 1LL * j * s[i - 1][j]) % MOD;
        }
    }
    return s;
}
}  // namespace Stirling
