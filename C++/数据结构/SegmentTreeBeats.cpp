// 线段树 Beats：区间加 / 区间 chmin / 区间 chmax / 查询 sum,min,max
// 下标 0-index，区间都是闭区间 [l,r]。
//
// 用法：
// vector<Info> a(n);
// rep(i, 0, n - 1) a[i] = Info(x);
// SegmentTreeBeats seg(a);
// seg.add(l, r, x);     // a[i] += x
// seg.chmin(l, r, x);   // a[i] = min(a[i], x)
// seg.chmax(l, r, x);   // a[i] = max(a[i], x)
// seg.assign(p, x);     // 单点赋值 a[p] = x
// auto res = seg.query(l, r);
// cout << res.sum << ' ' << res.mn << ' ' << res.mx << endl;
// 也可以直接 seg.sum(l,r), seg.min_val(l,r), seg.max_val(l,r)。
//
// mn/mx 是最小/最大值，se_mn/se_mx 是严格次小/严格次大值。
// Beats 适合同时有区间取 min/max 和区间和查询的题，复杂度按均摊 log 级别记。

struct Info {
    static constexpr ll INF = (1LL << 60);
    ll sum = 0;
    ll mx = -INF, se_mx = -INF;
    ll mn = INF, se_mn = INF;
    int cnt_mx = 0, cnt_mn = 0;
    ll add = 0;  // 只有区间加需要普通懒标记

    Info(ll x = 0)
        : sum(x), mx(x), se_mx(-INF), mn(x), se_mn(INF), cnt_mx(1), cnt_mn(1), add(0) {}
};

Info operator+(const Info& a, const Info& b) {
    Info c;
    c.sum = a.sum + b.sum;

    // 合并最大值、严格次大值、最大值个数
    if (a.mx == b.mx) {
        c.mx = a.mx;
        c.cnt_mx = a.cnt_mx + b.cnt_mx;
        c.se_mx = max(a.se_mx, b.se_mx);
    } else if (a.mx > b.mx) {
        c.mx = a.mx;
        c.cnt_mx = a.cnt_mx;
        c.se_mx = max(a.se_mx, b.mx);
    } else {
        c.mx = b.mx;
        c.cnt_mx = b.cnt_mx;
        c.se_mx = max(a.mx, b.se_mx);
    }

    // 合并最小值、严格次小值、最小值个数
    if (a.mn == b.mn) {
        c.mn = a.mn;
        c.cnt_mn = a.cnt_mn + b.cnt_mn;
        c.se_mn = min(a.se_mn, b.se_mn);
    } else if (a.mn < b.mn) {
        c.mn = a.mn;
        c.cnt_mn = a.cnt_mn;
        c.se_mn = min(a.se_mn, b.mn);
    } else {
        c.mn = b.mn;
        c.cnt_mn = b.cnt_mn;
        c.se_mn = min(a.mn, b.se_mn);
    }

    c.add = 0;
    return c;
}

class SegmentTreeBeats {
    int n;
    vector<Info> info;

    void maintain(int node) {
        info[node] = info[node << 1] + info[node << 1 | 1];
    }  // 用左右儿子维护当前节点

    void build(const vector<Info>& a, int node, int l, int r) {
        if (l == r) {
            info[node] = a[l];
            return;
        }
        int m = (l + r) >> 1;
        build(a, node << 1, l, m);
        build(a, node << 1 | 1, m + 1, r);
        maintain(node);
    }  // 建树，复杂度O(n)

    void apply_add(int node, int l, int r, ll x) {
        info[node].sum += x * (r - l + 1);
        info[node].mx += x;
        info[node].mn += x;
        if (info[node].se_mx != -Info::INF) info[node].se_mx += x;
        if (info[node].se_mn != Info::INF) info[node].se_mn += x;
        info[node].add += x;
    }  // 整段加x

    void apply_chmin(int node, ll x) {
        if (info[node].mx <= x) return;
        ll old = info[node].mx;
        info[node].sum += (x - old) * info[node].cnt_mx;
        if (info[node].mn == old) info[node].mn = x;
        if (info[node].se_mn == old) info[node].se_mn = x;
        info[node].mx = x;
    }  // 只把最大值压到x，要求se_mx < x < mx

    void apply_chmax(int node, ll x) {
        if (info[node].mn >= x) return;
        ll old = info[node].mn;
        info[node].sum += (x - old) * info[node].cnt_mn;
        if (info[node].mx == old) info[node].mx = x;
        if (info[node].se_mx == old) info[node].se_mx = x;
        info[node].mn = x;
    }  // 只把最小值抬到x，要求mn < x < se_mn

    void pushdown(int node, int l, int r) {
        if (l == r) {
            info[node].add = 0;
            return;
        }
        int m = (l + r) >> 1;
        if (info[node].add != 0) {
            apply_add(node << 1, l, m, info[node].add);
            apply_add(node << 1 | 1, m + 1, r, info[node].add);
            info[node].add = 0;
        }
        if (info[node << 1].mx > info[node].mx) apply_chmin(node << 1, info[node].mx);
        if (info[node << 1 | 1].mx > info[node].mx) apply_chmin(node << 1 | 1, info[node].mx);
        if (info[node << 1].mn < info[node].mn) apply_chmax(node << 1, info[node].mn);
        if (info[node << 1 | 1].mn < info[node].mn) apply_chmax(node << 1 | 1, info[node].mn);
    }  // 下传加法，并让儿子的min/max不越过当前节点

    void add(int node, int l, int r, int ql, int qr, ll x) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply_add(node, l, r, x);
            return;
        }
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        add(node << 1, l, m, ql, qr, x);
        add(node << 1 | 1, m + 1, r, ql, qr, x);
        maintain(node);
    }  // 区间加[ql,qr]

    void chmin(int node, int l, int r, int ql, int qr, ll x) {
        if (qr < l || r < ql || info[node].mx <= x) return;
        if (ql <= l && r <= qr && info[node].se_mx < x) {
            apply_chmin(node, x);
            return;
        }
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        chmin(node << 1, l, m, ql, qr, x);
        chmin(node << 1 | 1, m + 1, r, ql, qr, x);
        maintain(node);
    }  // 区间取min：[ql,qr]内a[i]=min(a[i],x)

    void chmax(int node, int l, int r, int ql, int qr, ll x) {
        if (qr < l || r < ql || info[node].mn >= x) return;
        if (ql <= l && r <= qr && info[node].se_mn > x) {
            apply_chmax(node, x);
            return;
        }
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        chmax(node << 1, l, m, ql, qr, x);
        chmax(node << 1 | 1, m + 1, r, ql, qr, x);
        maintain(node);
    }  // 区间取max：[ql,qr]内a[i]=max(a[i],x)

    void assign(int node, int l, int r, int p, ll x) {
        if (l == r) {
            info[node] = Info(x);
            return;
        }
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        if (p <= m)
            assign(node << 1, l, m, p, x);
        else
            assign(node << 1 | 1, m + 1, r, p, x);
        maintain(node);
    }  // 单点赋值

    Info query(int node, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return info[node];
        pushdown(node, l, r);
        int m = (l + r) >> 1;
        if (qr <= m) return query(node << 1, l, m, ql, qr);
        if (ql > m) return query(node << 1 | 1, m + 1, r, ql, qr);
        return query(node << 1, l, m, ql, qr) + query(node << 1 | 1, m + 1, r, ql, qr);
    }  // 区间查询[ql,qr]

public:
    SegmentTreeBeats(int n, Info init_val = Info()) : SegmentTreeBeats(vector<Info>(n, init_val)) {}

    SegmentTreeBeats(const vector<Info>& a)
        : n(sz(a)), info(2 << bit_width((unsigned)sz(a) - 1)) {
        build(a, 1, 0, n - 1);
    }  // 维护下标为[0,n-1]的数组

    void add(int ql, int qr, ll x) { add(1, 0, n - 1, ql, qr, x); }  // 区间加

    void chmin(int ql, int qr, ll x) { chmin(1, 0, n - 1, ql, qr, x); }  // 区间取min

    void chmax(int ql, int qr, ll x) { chmax(1, 0, n - 1, ql, qr, x); }  // 区间取max

    void assign(int p, ll x) { assign(1, 0, n - 1, p, x); }  // 单点赋值

    Info query(int ql, int qr) { return query(1, 0, n - 1, ql, qr); }  // 区间查询

    Info get(int p) { return query(p, p); }  // 单点查询

    ll sum(int ql, int qr) { return query(ql, qr).sum; }  // 区间和

    ll min_val(int ql, int qr) { return query(ql, qr).mn; }  // 区间最小值

    ll max_val(int ql, int qr) { return query(ql, qr).mx; }  // 区间最大值
};
