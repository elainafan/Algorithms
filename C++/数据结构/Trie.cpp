// Trie：维护小写字母串
// 适合：单词插入/删除、完整单词计数、前缀计数、查某个串是否有已插入单词作为前缀
struct Trie {
    struct Node {
        int son[26];
        int pass;  // 有多少个字符串经过这个点
        int end;   // 有多少个字符串在这个点结束

        Node() : pass(0), end(0) {
            memset(son, -1, sizeof son);
        }
    };

    vector<Node> tr;

    Trie() {
        tr.push_back(Node());  // 0 是根
    }

    void clear() {
        tr.clear();
        tr.push_back(Node());
    }

    void insert(const string& s) {
        int u = 0;
        tr[u].pass++;
        for (int i = 0; i < (int)s.size(); i++) {
            int c = s[i] - 'a';
            if (tr[u].son[c] == -1) {
                tr[u].son[c] = tr.size();
                tr.push_back(Node());
            }
            u = tr[u].son[c];
            tr[u].pass++;
        }
        tr[u].end++;
    }

    int cntword(const string& s) const {
        int u = 0;
        for (int i = 0; i < (int)s.size(); i++) {
            int c = s[i] - 'a';
            if (tr[u].son[c] == -1) return 0;
            u = tr[u].son[c];
        }
        return tr[u].end;
    }

    int cntprefix(const string& s) const {
        int u = 0;
        for (int i = 0; i < (int)s.size(); i++) {
            int c = s[i] - 'a';
            if (tr[u].son[c] == -1) return 0;
            u = tr[u].son[c];
        }
        return tr[u].pass;
    }

    bool search(const string& s) const {
        return cntword(s) > 0;
    }

    bool startsWith(const string& s) const {
        return cntprefix(s) > 0;
    }

    // 返回 s 的最短已插入前缀；没有则返回空串
    string pre(const string& s) const {
        int u = 0;
        for (int i = 0; i < (int)s.size(); i++) {
            int c = s[i] - 'a';
            if (tr[u].son[c] == -1) return "";
            u = tr[u].son[c];
            if (tr[u].end > 0) return s.substr(0, i + 1);
        }
        return "";
    }

    // 删除一个已经存在的 s；不确定存在时先 if (search(s))
    void erase(const string& s) {
        int u = 0;
        tr[u].pass--;
        for (int i = 0; i < (int)s.size(); i++) {
            int c = s[i] - 'a';
            u = tr[u].son[c];
            tr[u].pass--;
        }
        tr[u].end--;
    }
};

// 用法：
// Trie trie;
// trie.insert("abc");
// trie.insert("abc");
// trie.insert("abd");
// trie.search("abc");      // true
// trie.cntword("abc");     // 2
// trie.cntprefix("ab");    // 3
// trie.startsWith("ac");   // false
// trie.pre("abcd");        // "abc"
// trie.erase("abc");

// 非字母版 Trie：维护 vector<int> / 任意离散 token 序列
// 若字符集很大或不是 'a'..'z'，用这个；否则上面的数组版更快。
struct MapTrie {
    struct Node {
        unordered_map<int, int> son;
        int pass = 0;
        int end = 0;
    };

    vector<Node> tr;

    MapTrie() {
        tr.push_back(Node());
    }

    void clear() {
        tr.clear();
        tr.push_back(Node());
    }

    void insert(const vector<int>& a) {
        int u = 0;
        tr[u].pass++;
        for (int i = 0; i < (int)a.size(); i++) {
            int x = a[i];
            if (!tr[u].son.count(x)) {
                tr[u].son[x] = tr.size();
                tr.push_back(Node());
            }
            u = tr[u].son[x];
            tr[u].pass++;
        }
        tr[u].end++;
    }

    int cntword(const vector<int>& a) const {
        int u = 0;
        for (int i = 0; i < (int)a.size(); i++) {
            int x = a[i];
            unordered_map<int, int>::const_iterator it = tr[u].son.find(x);
            if (it == tr[u].son.end()) return 0;
            u = it->second;
        }
        return tr[u].end;
    }

    int cntprefix(const vector<int>& a) const {
        int u = 0;
        for (int i = 0; i < (int)a.size(); i++) {
            int x = a[i];
            unordered_map<int, int>::const_iterator it = tr[u].son.find(x);
            if (it == tr[u].son.end()) return 0;
            u = it->second;
        }
        return tr[u].pass;
    }

    bool search(const vector<int>& a) const {
        return cntword(a) > 0;
    }

    bool startsWith(const vector<int>& a) const {
        return cntprefix(a) > 0;
    }

    vector<int> pre(const vector<int>& a) const {
        int u = 0;
        for (int i = 0; i < (int)a.size(); i++) {
            int x = a[i];
            unordered_map<int, int>::const_iterator it = tr[u].son.find(x);
            if (it == tr[u].son.end()) return vector<int>();
            u = it->second;
            if (tr[u].end > 0) return vector<int>(a.begin(), a.begin() + i + 1);
        }
        return vector<int>();
    }

    void erase(const vector<int>& a) {
        int u = 0;
        tr[u].pass--;
        for (int i = 0; i < (int)a.size(); i++) {
            int x = a[i];
            u = tr[u].son[x];
            tr[u].pass--;
        }
        tr[u].end--;
    }
};

// 用法：
// MapTrie trie;
// trie.insert(vector<int>{1, 3, 1});
// trie.cntprefix(vector<int>{1, 3});

// 01-Trie：维护非负整数，常用来求最大/最小异或、支持删除和重复元素。
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
        tr.push_back(Node());
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

    // 返回能和 x 异或出的最大值，即 max(x ^ y)
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

    // 返回能和 x 异或出的最小值，即 min(x ^ y)
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

    // 第 k 小的 x^y，k 从 1 开始；要求 1 <= k <= tr[0].cnt
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
// BinaryTrie<30> trie;     // int 非负数
// trie.insert(x);
// trie.erase(x);
// trie.count(x);
// trie.max_xor(x);         // max(x ^ y)
// trie.min_xor(x);         // min(x ^ y)
// trie.count_xor_less(x, k); // count(y): (x ^ y) < k
// trie.kth_xor(x, k);      // 第 k 小的 x ^ y
// BinaryTrie<60> trie64;   // long long 非负数

// Trie变式，同时检索前缀和后缀

const int UND = '#';
struct Node {
    unordered_map<int, Node*> son;
    int idx = -1;
};
class Trie {
    Node* root = new Node();
    void destory(Node* node) {
        if (!node) return;
        for (auto [_, son] : node->son) destory(son);
        delete node;
    }

public:
    // 析构函数
    ~Trie() { destory(root); }

    void insert(const string& word, int idx) {
        Node* cur = root;
        int n = sz(word);
        rep(i, 0, n - 1) {
            Node* tem = cur;
            rep(j, i, n - 1) {
                int c = word[j] - 'a';
                int tem2 = (c << 7) | UND;
                if (!tem->son.count(tem2)) tem->son[tem2] = new Node();
                tem = tem->son[tem2];
                tem->idx = max(tem->idx, idx);
            }
            tem = cur;
            rep(j, i, n - 1) {
                int c = word[n - 1 - j] - 'a';
                int tem2 = (UND << 7) | c;
                if (!tem->son.count(tem2)) tem->son[tem2] = new Node();
                tem = tem->son[tem2];
                tem->idx = max(tem->idx, idx);
            }
            tem = cur;
            int c1 = word[i] - 'a';
            int c2 = word[n - 1 - i] - 'a';
            int tem2 = (c1 << 7) | c2;
            if (!cur->son.count(tem2)) cur->son[tem2] = new Node();
            cur = cur->son[tem2];
            cur->idx = max(cur->idx, idx);
        }
    }

    int find(const string& pre, const string& suf) {
        int m1 = sz(pre), m2 = sz(suf);
        int m = max(m1, m2);
        Node* cur = root;
        rep(i, 0, m - 1) {
            int c1 = (i <= m1 - 1) ? pre[i] - 'a' : UND;
            int c2 = (i <= m2 - 1) ? suf[m2 - 1 - i] - 'a' : UND;
            int tem2 = (c1 << 7) | c2;
            if (!cur->son.count(tem2)) return -1;
            cur = cur->son[tem2];
        }
        return cur->idx;
    }
};
