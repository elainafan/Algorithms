using ull = unsigned long long;

ull splitmix64(ull x) {
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
}

struct custom_hash {
    static const ull FIXED_RANDOM;

    size_t operator()(ull x) const {
        return splitmix64(x + FIXED_RANDOM);
    }
};

const ull custom_hash::FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();

struct StringHash {
    static ull base() {
        static ull b = splitmix64(chrono::steady_clock::now().time_since_epoch().count()) | 1;
        return b;
    }

    vector<ull> h, p;

    StringHash(const string& s) {
        int n = s.size();
        h.assign(n + 1, 0);
        p.assign(n + 1, 1);
        for (int i = 0; i < n; i++) {
            h[i + 1] = h[i] * base() + (unsigned char)s[i] + 1;
            p[i + 1] = p[i] * base();
        }
    }

    // 获取 s[l...r] 的哈希值，闭区间，0-indexed
    ull query(int l, int r) const {
        return h[r + 1] - h[l] * p[r - l + 1];
    }
};

// 使用：StringHash hs(s); hs.query(l, r);
// 使用：unordered_map<ull, int, custom_hash> ma;
