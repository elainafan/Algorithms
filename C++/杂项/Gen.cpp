#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int rnd(int l, int r) {
    return uniform_int_distribution<int>(l, r)(rng);
}

int main() {
    // Example for array problems. Change this part to match the input format.
    int n = rnd(1, 8);  // Keep data small enough for brute.cpp.
    cout << n << '\n';
    for (int i = 0; i < n; i++) {
        cout << rnd(-10, 10) << " \n"[i == n - 1];
    }
    return 0;
}
