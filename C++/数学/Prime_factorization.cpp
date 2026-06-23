constexpr int MX = 1e5 + 1;
int lpf[MX]; // 存储每个数的最小素因子，复杂度O(NloglogN)
auto init = [] {
    for (int i = 2; i < MX; i++) {
        if (lpf[i] == 0) {
            for (int j = i; j < MX; j += i) {
                if (lpf[j] == 0) lpf[j] = i;
            }
        }
    }
    return 0;
}();
// 质因数分解，返回值为pair<素因子，素因子次幂>，复杂度O(logN)
vector<pair<int, int>> cnt(int x) {
    vector<pair<int, int>> res;
    while (x > 1) {
        int p = lpf[x];
        int e = 1;
        for (x /= p; x % p == 0; x /= p) {
            e++;
        }
        res.emplace_back(p, e);
    }
    return res;
}