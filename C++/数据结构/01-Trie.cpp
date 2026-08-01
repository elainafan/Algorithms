// 01-Trie：所有数满足 0 <= x < 2^(LOG + 1)
void solve() {
    const int LOG = 30;  // long long 改成 60
    vector<array<int, 2>> son(1);
    vi cnt(1);

    auto insert = [&](ll x) -> void {
        int cur = 0;
        cnt[cur]++;
        frep(i, LOG, 0) {
            int b = x >> i & 1;
            if (!son[cur][b]) {
                son[cur][b] = sz(son);
                son.push_back({}), cnt.push_back(0);
            }
            cur = son[cur][b], cnt[cur]++;
        }
    };  // 插入整数 x
    auto erase = [&](ll x) -> void {
        int cur = 0;
        cnt[cur]--;
        frep(i, LOG, 0) {
            int b = x >> i & 1;
            cur = son[cur][b], cnt[cur]--;
        }
    };  // 删除一个已存在的整数 x
    auto count = [&](ll x) -> int {
        int cur = 0;
        frep(i, LOG, 0) {
            int b = x >> i & 1;
            if (!son[cur][b] || !cnt[son[cur][b]]) return 0;
            cur = son[cur][b];
        }
        return cnt[cur];
    };  // 查询整数 x 的出现次数
    auto query = [&](ll x, bool mx = true) -> ll {
        if (!cnt[0]) return -1;
        int cur = 0;
        ll ans = 0;
        frep(i, LOG, 0) {
            int b = x >> i & 1, to = b ^ mx;
            if (!son[cur][to] || !cnt[son[cur][to]]) to ^= 1;
            ans |= 1LL * (b ^ to) << i;
            cur = son[cur][to];
        }
        return ans;
    };  // mx=true 求最大异或，mx=false 求最小异或，空树返回 -1
    auto cntless = [&](ll x, ll k) -> ll {
        if (k <= 0) return 0;
        if (k >= (1LL << (LOG + 1))) return cnt[0];
        int cur = 0;
        ll ans = 0;
        frep(i, LOG, 0) {
            int xb = x >> i & 1, kb = k >> i & 1;
            if (kb) {
                if (son[cur][xb]) ans += cnt[son[cur][xb]];
                cur = son[cur][xb ^ 1];
            } else {
                cur = son[cur][xb];
            }
            if (!cur) break;
        }
        return ans;
    };  // 统计满足 (x^y)<k 的 y 的数量
    auto kth = [&](ll x, int k) -> ll {
        int cur = 0;
        ll ans = 0;
        frep(i, LOG, 0) {
            int b = x >> i & 1, to = son[cur][b];
            int siz = to ? cnt[to] : 0;
            if (k > siz) k -= siz, ans |= 1LL << i, to = son[cur][b ^ 1];
            cur = to;
        }
        return ans;
    };  // 查询第 k 小的 x^y，要求 1<=k<=cnt[0]

    // insert(x), erase(x), count(x), query(x), query(x,0), cntless(x,k), kth(x,k)
}
