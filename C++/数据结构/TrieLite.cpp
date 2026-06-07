// Trie 短版：赛时方便改
// 改法：
// 1. 改 SIG 和 id()，适配字符集。
// 2. 在 Node 里加字段，比如 idx / mn / mx / vector<int> ids。
// 3. 在 insert() 经过节点或结尾时维护字段。
// 4. 需要查询时先 walk() 到节点，再读 tr[u] 的字段。
template<int SIG = 26>
struct TrieLite {
    struct Node {
        int ch[SIG];
        int pass, end;

        Node() : pass(0), end(0) {
            memset(ch, -1, sizeof ch);
        }
    };

    vector<Node> tr;

    TrieLite() {
        tr.push_back(Node()); // 0 是根
    }

    int id(char c) {
        return c - 'a'; // 大写改成 c - 'A'，01 串改成 c - '0'
    }

    int newnode() {
        tr.push_back(Node());
        return (int)tr.size() - 1;
    }

    void insert(const string& s) {
        int u = 0;
        tr[u].pass++;
        for (int i = 0; i < (int)s.size(); i++) {
            int c = id(s[i]);
            if (tr[u].ch[c] == -1) {
                int v = newnode();
                tr[u].ch[c] = v;
            }
            u = tr[u].ch[c];
            tr[u].pass++;
            // 经过节点时要维护的信息写这里
        }
        tr[u].end++;
        // 单词结尾要维护的信息写这里
    }

    int walk(const string& s) {
        int u = 0;
        for (int i = 0; i < (int)s.size(); i++) {
            int c = id(s[i]);
            if (tr[u].ch[c] == -1) return -1;
            u = tr[u].ch[c];
        }
        return u;
    }

    int cntword(const string& s) {
        int u = walk(s);
        return u == -1 ? 0 : tr[u].end;
    }

    int cntprefix(const string& s) {
        int u = walk(s);
        return u == -1 ? 0 : tr[u].pass;
    }
};

// 用法：
// TrieLite<26> trie;
// trie.insert("abc");
// int u = trie.walk("ab");
// if (u != -1) cout << trie.tr[u].pass << '\n';

// 非字母短版：字符集很大 / token 是 int 时用
struct MapTrieLite {
    struct Node {
        map<int, int> ch; // 想更快可换 unordered_map<int,int>
        int pass = 0;
        int end = 0;
    };

    vector<Node> tr;

    MapTrieLite() {
        tr.push_back(Node());
    }

    int newnode() {
        tr.push_back(Node());
        return (int)tr.size() - 1;
    }

    void insert(const vector<int>& a) {
        int u = 0;
        tr[u].pass++;
        for (int i = 0; i < (int)a.size(); i++) {
            int x = a[i];
            if (!tr[u].ch.count(x)) {
                int v = newnode();
                tr[u].ch[x] = v;
            }
            u = tr[u].ch[x];
            tr[u].pass++;
        }
        tr[u].end++;
    }

    int walk(const vector<int>& a) {
        int u = 0;
        for (int i = 0; i < (int)a.size(); i++) {
            int x = a[i];
            if (!tr[u].ch.count(x)) return -1;
            u = tr[u].ch[x];
        }
        return u;
    }
};

// 01-Trie 短版：只留最常用的插入和最大异或
// 需要删除就给每个点 cnt--；需要 min_xor 就优先走同位。
template<int LOG = 30>
struct BinaryTrieLite {
    struct Node {
        int ch[2];
        int cnt;

        Node() : cnt(0) {
            ch[0] = ch[1] = -1;
        }
    };

    vector<Node> tr;

    BinaryTrieLite() {
        tr.push_back(Node());
    }

    int newnode() {
        tr.push_back(Node());
        return (int)tr.size() - 1;
    }

    void insert(long long x) {
        int u = 0;
        tr[u].cnt++;
        for (int i = LOG; i >= 0; i--) {
            int b = (x >> i) & 1;
            if (tr[u].ch[b] == -1) {
                int v = newnode();
                tr[u].ch[b] = v;
            }
            u = tr[u].ch[b];
            tr[u].cnt++;
        }
    }

    long long max_xor(long long x) {
        if (tr[0].cnt == 0) return 0;
        int u = 0;
        long long ans = 0;
        for (int i = LOG; i >= 0; i--) {
            int b = (x >> i) & 1;
            int v = tr[u].ch[b ^ 1];
            if (v != -1 && tr[v].cnt > 0) {
                ans |= 1LL << i;
                u = v;
            } else {
                u = tr[u].ch[b];
            }
        }
        return ans;
    }
};

// 用法：
// BinaryTrieLite<30> trie;
// trie.insert(x);
// cout << trie.max_xor(x) << '\n';
