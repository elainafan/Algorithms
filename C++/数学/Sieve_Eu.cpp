constexpr int MX = 1e8 + 1;
bool pd[MX];
vi primes;
auto init = [] {
    rep(i, 2, MX) {
        if (!pd[i]) primes.push_back(i);
        for (int& p : primes) {
            if (p * i > MX) break;
            pd[i * p] = true;
            if (i % p == 0) break;
        }
    }
    return 0;
}();