// sa[i]：排名第 i 的后缀起点
// rk[i]：后缀 s[i...] 的排名
// height[i]：lcp(sa[i - 1], sa[i])，height[0] = 0
struct SA {
    string s;
    int n;
    vi sa, rk, height;
    vvi st;

    SA(string s) : s(move(s)), n(sz(this->s)), sa(n), rk(n), height(n) {
        if (!n) return;
        build_sa(), build_height(), build_st();
    }  // 构造 sa、rk、height 和 LCP 的 ST 表

    auto lcp(int x, int y) const -> int {
        if (x == y) return n - x;
        x = rk[x], y = rk[y];
        if (x > y) swap(x, y);
        int k = __lg(y - x);
        return min(st[k][x + 1], st[k][y - (1 << k) + 1]);
    }  // 查询后缀 s[x...] 与 s[y...] 的 LCP

    auto cmp(int l1, int r1, int l2, int r2) const -> int {
        int n1 = r1 - l1 + 1, n2 = r2 - l2 + 1, len = min(n1, n2);
        int p = min(lcp(l1, l2), len);
        if (p == len) return n1 == n2 ? 0 : (n1 < n2 ? -1 : 1);
        return s[l1 + p] < s[l2 + p] ? -1 : 1;
    }  // 比较两个闭区间子串，返回 -1、0、1

    auto match(const string& t) const -> pii {
        int l = 0, r = n;
        while (l < r) {
            int mid = (l + r) / 2;
            if (s.compare(sa[mid], sz(t), t) < 0)
                l = mid + 1;
            else
                r = mid;
        }
        int L = l;
        r = n;
        while (l < r) {
            int mid = (l + r) / 2;
            if (s.compare(sa[mid], sz(t), t) <= 0)
                l = mid + 1;
            else
                r = mid;
        }
        return {L, l};
    }  // 返回以 t 为前缀的后缀排名区间 [l,r)

    auto distinct() const -> ll {
        return 1LL * n * (n + 1) / 2 - accumulate(all(height), 0LL);
    }  // 查询不同子串数量

    auto kth_sub(ll k) const -> pii {
        rep(i, 0, n - 1) {
            int cnt = n - sa[i] - height[i];
            if (k > cnt)
                k -= cnt;
            else
                return {sa[i], sa[i] + height[i] + k - 1};
        }
        return {-1, -1};
    }  // 查询字典序第 k 小的不同子串，返回闭区间 [l,r]

    auto repeat(int k) const -> int {
        if (k <= 1) return n;
        if (k > n) return 0;
        deque<int> q;
        int ans = 0;
        rep(i, 1, n - 1) {
            while (!q.empty() && height[q.back()] >= height[i]) q.pop_back();
            q.push_back(i);
            while (q.front() < i - k + 2) q.pop_front();
            if (i >= k - 1) ans = max(ans, height[q.front()]);
        }
        return ans;
    }  // 查询至少出现 k 次的最长子串长度

    auto lcs(int m) const -> pii {
        int ans = 0, pos = -1;
        rep(i, 1, n - 1) {
            int x = sa[i - 1], y = sa[i];
            if (x == m || y == m || (x < m) == (y < m)) continue;
            if (height[i] > ans) ans = height[i], pos = x;
        }
        return {ans, pos};
    }  // 当 s=a+'#'+b 且 m=|a| 时，返回最长公共子串的 {长度,起点}

private:
    auto build_sa() -> void {
        int m = 256;
        vi old(n), id(n), cnt(max(n, m));
        rep(i, 0, n - 1) rk[i] = (unsigned char)s[i], cnt[rk[i]]++;
        rep(i, 1, m - 1) cnt[i] += cnt[i - 1];
        frep(i, n - 1, 0) sa[--cnt[rk[i]]] = i;

        for (int w = 1, p = 0; p < n; w <<= 1) {
            p = 0;
            rep(i, max(0, n - w), n - 1) id[p++] = i;
            rep(i, 0, n - 1) {
                if (sa[i] >= w) id[p++] = sa[i] - w;
            }
            fill(all(cnt), 0);
            rep(i, 0, n - 1) cnt[rk[id[i]]]++;
            rep(i, 1, m - 1) cnt[i] += cnt[i - 1];
            frep(i, n - 1, 0) sa[--cnt[rk[id[i]]]] = id[i];

            old = rk, p = 1, rk[sa[0]] = 0;
            rep(i, 1, n - 1) {
                int x = sa[i], y = sa[i - 1];
                rk[x] = old[x] == old[y] &&
                                (x + w < n ? old[x + w] : -1) == (y + w < n ? old[y + w] : -1)
                            ? p - 1
                            : p++;
            }
            if (p == n) break;
            m = p;
        }
    }  // O(nlogn) 构造后缀数组 sa 和排名 rk

    auto build_height() -> void {
        int h = 0;
        rep(i, 0, n - 1) {
            if (!rk[i]) {
                h = 0;
                continue;
            }
            int j = sa[rk[i] - 1];
            while (i + h < n && j + h < n && s[i + h] == s[j + h]) h++;
            height[rk[i]] = h;
            if (h) h--;
        }
    }  // O(n) 构造相邻后缀的 LCP 数组 height

    auto build_st() -> void {
        int lg = bit_width((unsigned)n);
        st.assign(lg, vi(n));
        st[0] = height;
        rep(j, 1, lg - 1) {
            rep(i, 0, n - (1 << j)) {
                st[j][i] = min(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]);
            }
        }
    }  // O(nlogn) 构造 height 的 ST 表
};

// SA suf(s);
// suf.sa / suf.rk / suf.height
// suf.lcp(x, y);                 后缀 s[x...] 与 s[y...] 的 LCP
// suf.cmp(l1, r1, l2, r2);      比较两个闭区间子串
// auto [l, r] = suf.match(t);    t 的出现次数为 r-l
// suf.distinct();                不同子串数量
// auto [l, r] = suf.kth_sub(k);  第 k 小不同子串为 s[l...r]
// suf.repeat(k);                 至少出现 k 次的最长子串长度
// SA suf(a + '#' + b);
// suf.lcs(sz(a));                两个字符串的最长公共子串
