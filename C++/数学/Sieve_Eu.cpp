constexpr int MX = 6e6 + 1;
bool pd[MX];
vi primes;
auto init = [] {
    rep(i, 2, MX - 1) {
        if (!pd[i]) primes.push_back(i);
        for (int& p : primes) {
            if (1LL * p * i >= MX) break;
            pd[i * p] = true;
            if (i % p == 0) break;
        }
    }
    return 0;
}();