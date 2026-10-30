// 随机异或哈希：按每个位置所属的区间状态集合分组，位置和闭区间均为 1-based。
// 每个状态独立取一个 64 位随机值；相同 [l,r] 的不同状态也分别取值。
// xor_hash 为 O(n+m)，xor_group 期望 O(n)，额外空间 O(n)。
// 哈希相等时认为状态集合相同，存在极小碰撞概率。
using ull = unsigned long long;

vector<ull> xor_hash(int n, const vector<pll>& seg) {
    vector<ull> h(n + 2);
    for (auto [l, r] : seg) {
        ull v = rng();
        v = (v << 32) | rng();
        h[l] ^= v;
        h[r + 1] ^= v;  // 进入区间时加入，离开时消去
    }
    rep(i, 1, n) h[i] ^= h[i - 1];
    h.pop_back();
    return h;
}  // h[x] 是所有覆盖 x 的状态随机值的异或和，h[0] 不使用

pair<vi, int> xor_group(const vector<ull>& h) {
    int n = sz(h) - 1, cnt = 0;
    unordered_map<ull, int> ma;
    ma.reserve(n);
    vi id(n + 1);
    rep(i, 1, n) {
        auto [it, ok] = ma.emplace(h[i], cnt);
        if (ok) cnt++;
        id[i] = it->second;
    }
    return {move(id), cnt};
}  // 返回 {各位置的类别编号, 类别总数}，类别编号从 0 开始

/*
用法：

vector<pll> seg;                    // 保存 m 个状态，每个状态是闭区间 [l,r]
seg.emplace_back(l, r);             // 1 <= l <= r <= n
auto h = xor_hash(n, seg);
bool same = h[x] == h[y];           // 判断 x、y 是否属于完全相同的状态集合
auto [id, cnt] = xor_group(h);      // id[x] 为 x 的类别，类别范围 [0,cnt-1]

vi siz(cnt);
rep(i, 1, n) siz[id[i]]++;          // 统计各类别包含的位置数

哈希仅在同一次 xor_hash 的结果中比较；没有任何状态覆盖的位置也归为一类。
*/
