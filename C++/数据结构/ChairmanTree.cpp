using ll = long long;

struct ChairmanTree {
    struct Node {
        int ls = 0, rs = 0, cnt = 0;
        ll sum = 0;
    };

    vector<Node> tr;
    vector<int> root;
    vector<int> vals;

    ChairmanTree(const vector<int>& a) {
        vals = a;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());

        int n = a.size();
        tr.reserve((n + 1) * 20);
        tr.push_back(Node());
        root.assign(n + 1, 0);
        root[0] = build(1, vals.size());
        for (int i = 1; i <= n; i++) {
            int pos = lower_bound(vals.begin(), vals.end(), a[i - 1]) - vals.begin() + 1;
            root[i] = add(root[i - 1], 1, vals.size(), pos, a[i - 1]);
        }
    }

    int build(int l, int r) {
        int o = new_node();
        if (l == r) return o;
        int m = (l + r) >> 1;
        tr[o].ls = build(l, m);
        tr[o].rs = build(m + 1, r);
        return o;
    }

    int add(int old, int l, int r, int p, int val) {
        int o = copy_node(old);
        tr[o].cnt++;
        tr[o].sum += val;
        if (l == r) return o;
        int m = (l + r) >> 1;
        if (p <= m) tr[o].ls = add(tr[old].ls, l, m, p, val);
        else tr[o].rs = add(tr[old].rs, m + 1, r, p, val);
        return o;
    }

    int kth(int l, int r, int k) const {
        return vals[kth(root[l - 1], root[r], 1, vals.size(), k) - 1];
    }

    pair<int, ll> leq(int l, int r, int x) const {
        int pos = upper_bound(vals.begin(), vals.end(), x) - vals.begin();
        if (!pos) return {0, 0};
        return query(root[l - 1], root[r], 1, vals.size(), pos);
    }

private:
    int new_node() {
        tr.push_back(Node());
        return tr.size() - 1;
    }

    int copy_node(int old) {
        tr.push_back(tr[old]);
        return tr.size() - 1;
    }

    int kth(int old, int now, int l, int r, int k) const {
        if (l == r) return l;
        int left_cnt = tr[tr[now].ls].cnt - tr[tr[old].ls].cnt;
        int m = (l + r) >> 1;
        if (k <= left_cnt) return kth(tr[old].ls, tr[now].ls, l, m, k);
        return kth(tr[old].rs, tr[now].rs, m + 1, r, k - left_cnt);
    }

    pair<int, ll> query(int old, int now, int l, int r, int qr) const {
        if (r <= qr) return {tr[now].cnt - tr[old].cnt, tr[now].sum - tr[old].sum};
        int m = (l + r) >> 1;
        auto res = query(tr[old].ls, tr[now].ls, l, m, qr);
        if (qr > m) {
            auto t = query(tr[old].rs, tr[now].rs, m + 1, r, qr);
            res.first += t.first;
            res.second += t.second;
        }
        return res;
    }
};

// 使用：ChairmanTree ct(a);
// ct.kth(l, r, k)：查询 1-indexed 区间 [l, r] 第 k 小的原值
// ct.leq(l, r, x)：查询区间 [l, r] 内 <= x 的数量和总和
//
// vector<int> a(n);              // a 是 0-indexed 原数组
// ChairmanTree ct(a);
// int kth_val = ct.kth(l, r, k); // l, r, k 都按 1-indexed 传
// auto [cnt, sum] = ct.leq(l, r, limit);
