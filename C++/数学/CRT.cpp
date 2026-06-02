/*
CRT / exCRT

求解同余方程组：
    x = r[i] (mod m[i])

用法：
    vector<ll> r = {2, 3, 2};
    vector<ll> m = {3, 5, 7};

    auto [x, mod] = crt(r, m);    // 要求 m 两两互质
    auto [x, mod] = excrt(r, m);  // m 不要求互质

    auto [ok, x0, y0, g] = linear_diophantine(a, b, c);  // ax + by = c
    auto [x0, step] = linear_congruence(a, b, m);        // ax = b (mod m)

返回值：
    若有解，返回 {x, mod}，表示所有解为 x + k * mod，且 0 <= x < mod。
    若无解，返回 {-1, -1}。

    linear_diophantine：
        若 ok = true，返回一组解 (x0, y0)，g = gcd(a, b)。
        所有整数解：
            x = x0 + k * (b / g)
            y = y0 - k * (a / g)

    linear_congruence：
        若有解，返回 {x0, step}，表示所有解为 x = x0 + k * step。
        其中 0 <= x0 < step，step = m / gcd(a, m)。
        若无解，返回 {-1, -1}。

注意：
    1. m[i] 必须为正数。
    2. 若最终 lcm 超过 long long，需要自己改成 __int128 或高精度。
    3. crt 实际上调用 excrt，只是语义上提醒模数互质时可用。
*/

ll exgcd(ll a, ll b, ll& x, ll& y) {
    if (!b) {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll g = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - a / b * y1;
    return g;
}

ll norm_mod(ll x, ll mod) {
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

struct Diophantine {
    bool ok;
    ll x, y, g;
};

// 解 ax + by = c。
Diophantine linear_diophantine(ll a, ll b, ll c) {
    if (a == 0 && b == 0) return {c == 0, 0, 0, 0};

    ll x, y;
    ll g = exgcd(abs(a), abs(b), x, y);
    if (c % g != 0) return {false, 0, 0, g};

    x = (ll)((__int128)x * (c / g));
    y = (ll)((__int128)y * (c / g));
    if (a < 0) x = -x;
    if (b < 0) y = -y;
    return {true, x, y, g};
}

// 解 ax = b (mod mod)。
pair<ll, ll> linear_congruence(ll a, ll b, ll mod) {
    ll x, y;
    ll g = exgcd(abs(a), mod, x, y);
    if (b % g != 0) return {-1, -1};

    ll step = mod / g;
    x = (ll)((__int128)x * (b / g) % step);
    if (a < 0) x = -x;
    return {norm_mod(x, step), step};
}

// 求 a 在 mod 下的逆元，要求 gcd(a, mod) = 1。
ll inv_mod(ll a, ll mod) {
    ll x, y;
    exgcd(a, mod, x, y);
    return norm_mod(x, mod);
}

// 扩展 CRT：模数不一定互质。
pair<ll, ll> excrt(const vector<ll>& r, const vector<ll>& m) {
    ll ans = norm_mod(r[0], m[0]);
    ll mod = m[0];
    for (int i = 1; i < (int)r.size(); i++) {
        ll b = norm_mod(r[i] - ans, m[i]);
        ll g = gcd(mod, m[i]);
        if (b % g != 0) return {-1, -1};

        ll p = mod / g;
        ll q = m[i] / g;
        ll t = (ll)((__int128)(b / g) * inv_mod(p % q, q) % q);

        ll lcm = (ll)((__int128)mod / g * m[i]);
        ans = (ans + (ll)((__int128)mod * t % lcm)) % lcm;
        mod = lcm;
    }
    return {ans, mod};
}

// 普通 CRT：模数两两互质。
pair<ll, ll> crt(const vector<ll>& r, const vector<ll>& m) {
    return excrt(r, m);
}
