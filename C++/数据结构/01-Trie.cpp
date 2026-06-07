// 01-Trie：维护非负整数
// 常用：max(x ^ y)、min(x ^ y)、删除、重复元素、统计 (x ^ y) < k、第 k 小 x ^ y
// LOG 表示最高位，0 <= x < 2^(LOG+1)。int 常用 30，long long 常用 60。
template<int LOG = 30>
struct BinaryTrie {
    struct Node {
        int son[2];
        int cnt;  // 子树内有多少个数

        Node() : cnt(0) {
            son[0] = son[1] = -1;
        }
    };

    vector<Node> tr;

    BinaryTrie() {
        tr.push_back(Node());  // 0 是根
    }

    void clear() {
        tr.clear();
        tr.push_back(Node());
    }

    void insert(long long x) {
        int u = 0;
        tr[u].cnt++;
        for (int i = LOG; i >= 0; i--) {
            int b = (x >> i) & 1;
            if (tr[u].son[b] == -1) {
                tr[u].son[b] = tr.size();
                tr.push_back(Node());
            }
            u = tr[u].son[b];
            tr[u].cnt++;
        }
    }

    // 删除一个已经存在的 x；不确定存在时先 if (count(x))
    void erase(long long x) {
        int u = 0;
        tr[u].cnt--;
        for (int i = LOG; i >= 0; i--) {
            int b = (x >> i) & 1;
            u = tr[u].son[b];
            tr[u].cnt--;
        }
    }

    int count(long long x) const {
        int u = 0;
        for (int i = LOG; i >= 0; i--) {
            int b = (x >> i) & 1;
            int v = tr[u].son[b];
            if (v == -1 || tr[v].cnt == 0) return 0;
            u = v;
        }
        return tr[u].cnt;
    }

    // 返回 max(x ^ y)
    long long max_xor(long long x) const {
        if (tr[0].cnt == 0) return 0;
        int u = 0;
        long long ans = 0;
        for (int i = LOG; i >= 0; i--) {
            int b = (x >> i) & 1;
            int want = b ^ 1;
            int v = tr[u].son[want];
            if (v != -1 && tr[v].cnt > 0) {
                ans |= 1LL << i;
                u = v;
            } else {
                u = tr[u].son[b];
            }
        }
        return ans;
    }

    // 返回 min(x ^ y)
    long long min_xor(long long x) const {
        if (tr[0].cnt == 0) return 0;
        int u = 0;
        long long ans = 0;
        for (int i = LOG; i >= 0; i--) {
            int b = (x >> i) & 1;
            int v = tr[u].son[b];
            if (v != -1 && tr[v].cnt > 0) {
                u = v;
            } else {
                ans |= 1LL << i;
                u = tr[u].son[b ^ 1];
            }
        }
        return ans;
    }

    // 统计已有 y 中，满足 (x ^ y) < k 的数量
    long long count_xor_less(long long x, long long k) const {
        int u = 0;
        long long ans = 0;
        for (int i = LOG; i >= 0 && u != -1; i--) {
            int xb = (x >> i) & 1;
            int kb = (k >> i) & 1;
            if (kb) {
                int same = tr[u].son[xb];
                if (same != -1) ans += tr[same].cnt;
                u = tr[u].son[xb ^ 1];
            } else {
                u = tr[u].son[xb];
            }
        }
        return ans;
    }

    // 第 k 小的 x ^ y，k 从 1 开始；要求 1 <= k <= tr[0].cnt
    long long kth_xor(long long x, int k) const {
        int u = 0;
        long long ans = 0;
        for (int i = LOG; i >= 0; i--) {
            int b = (x >> i) & 1;
            int same = tr[u].son[b];
            int c = (same == -1 ? 0 : tr[same].cnt);
            if (k <= c) {
                u = same;
            } else {
                k -= c;
                ans |= 1LL << i;
                u = tr[u].son[b ^ 1];
            }
        }
        return ans;
    }
};

// 用法：
// BinaryTrie<30> trie;       // int 非负数
// trie.insert(x);
// if (trie.count(x)) trie.erase(x);
// trie.max_xor(x);           // max(x ^ y)
// trie.min_xor(x);           // min(x ^ y)
// trie.count_xor_less(x, k); // count(y): (x ^ y) < k
// trie.kth_xor(x, k);        // 第 k 小的 x ^ y
// BinaryTrie<60> trie64;     // long long 非负数
