constexpr int M = 2e8;
bool is_prime[M];
vector<int> primes;
auto init = [] {
    ranges::fill(is_prime, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i < M; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            for (ll j = 1LL * i * i; j < M; j += i) {
                is_prime[j] = false;
            }
        }
    }
    return 0;
}();
// 函数指针在创建时自动调用