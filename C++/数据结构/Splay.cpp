// Splay维护序列，0-based，内部有左右两个哨兵。

struct Splay {
    static constexpr ll INF = (1LL << 60);

    struct Info {
        int siz = 0;
        ll sum = 0, mn = INF, mx = -INF;
    };

    struct Node {
        int ch[2] = {0, 0}, fa = 0;
        int siz = 0;
        ll val = 0, sum = 0, mn = INF, mx = -INF;
        ll add = 0, cover_val = 0;
        bool rev = false, cover = false;
    };

    vector<Node> tr;
    int root = 0;

    Splay() : tr(1) { build(vl()); }
    Splay(const vl& a) : tr(1) { build(a); }

    int size(int p) const {
        return p ? tr[p].siz : 0;
    }

    int size() const {
        return size(root) - 2;
    }

    int new_node(ll x, int fa = 0) {
        tr.push_back(Node());
        int p = sz(tr) - 1;
        tr[p].fa = fa;
        tr[p].siz = 1;
        tr[p].val = tr[p].sum = tr[p].mn = tr[p].mx = x;
        return p;
    }  // 新建值为x的节点

    void maintain(int p) {
        if (!p) return;
        tr[p].siz = size(tr[p].ch[0]) + size(tr[p].ch[1]) + 1;
        tr[p].sum = tr[tr[p].ch[0]].sum + tr[p].val + tr[tr[p].ch[1]].sum;
        tr[p].mn = min({tr[tr[p].ch[0]].mn, tr[p].val, tr[tr[p].ch[1]].mn});
        tr[p].mx = max({tr[tr[p].ch[0]].mx, tr[p].val, tr[tr[p].ch[1]].mx});
    }  // 维护子树信息

    void apply_rev(int p) {
        if (!p) return;
        swap(tr[p].ch[0], tr[p].ch[1]);
        tr[p].rev ^= 1;
    }  // 区间翻转标记

    void apply_add(int p, ll x) {
        if (!p) return;
        tr[p].val += x;
        tr[p].sum += x * tr[p].siz;
        tr[p].mn += x;
        tr[p].mx += x;
        if (tr[p].cover)
            tr[p].cover_val += x;
        else
            tr[p].add += x;
    }  // 区间加标记

    void apply_assign(int p, ll x) {
        if (!p) return;
        tr[p].val = x;
        tr[p].sum = x * tr[p].siz;
        tr[p].mn = tr[p].mx = x;
        tr[p].add = 0;
        tr[p].cover = true;
        tr[p].cover_val = x;
    }  // 区间赋值标记

    void pushdown(int p) {
        if (!p) return;
        if (tr[p].cover) {
            apply_assign(tr[p].ch[0], tr[p].cover_val);
            apply_assign(tr[p].ch[1], tr[p].cover_val);
            tr[p].cover = false;
        }
        if (tr[p].add) {
            apply_add(tr[p].ch[0], tr[p].add);
            apply_add(tr[p].ch[1], tr[p].add);
            tr[p].add = 0;
        }
        if (tr[p].rev) {
            apply_rev(tr[p].ch[0]);
            apply_rev(tr[p].ch[1]);
            tr[p].rev = false;
        }
    }  // 下传懒标记

    int build_tree(const vl& a, int l, int r, int fa = 0) {
        if (l > r) return 0;
        int m = (l + r) >> 1;
        int p = new_node(a[m], fa);
        tr[p].ch[0] = build_tree(a, l, m - 1, p);
        tr[p].ch[1] = build_tree(a, m + 1, r, p);
        maintain(p);
        return p;
    }  // 建一棵子树

    void build(const vl& a) {
        tr.assign(1, Node());
        vl b;
        b.push_back(0);  // 左哨兵
        for (ll x : a) b.push_back(x);
        b.push_back(0);  // 右哨兵
        root = build_tree(b, 0, sz(b) - 1);
    }  // 重新建树

    int dir(int x) const {
        return tr[tr[x].fa].ch[1] == x;
    }

    void push_path(int x) {
        static vector<int> st;
        st.clear();
        for (int p = x; p; p = tr[p].fa) st.push_back(p);
        while (!st.empty()) {
            pushdown(st.back());
            st.pop_back();
        }
    }  // 旋转前下传根到x的标记

    void rotate(int x) {
        int y = tr[x].fa, z = tr[y].fa;
        int k = dir(x), w = tr[x].ch[k ^ 1];
        if (z) tr[z].ch[dir(y)] = x;
        tr[x].fa = z;
        tr[x].ch[k ^ 1] = y;
        tr[y].fa = x;
        tr[y].ch[k] = w;
        if (w) tr[w].fa = y;
        maintain(y);
        maintain(x);
    }  // 单旋

    void splay(int x, int goal = 0) {
        push_path(x);
        while (tr[x].fa != goal) {
            int y = tr[x].fa, z = tr[y].fa;
            if (z != goal) {
                if (dir(x) == dir(y))
                    rotate(y);
                else
                    rotate(x);
            }
            rotate(x);
        }
        if (!goal) root = x;
    }  // 把x旋到goal下面，goal=0表示旋到根

    int kth_node(int k) {
        int p = root;
        while (p) {
            pushdown(p);
            if (k <= size(tr[p].ch[0])) {
                p = tr[p].ch[0];
            } else if (k == size(tr[p].ch[0]) + 1) {
                return p;
            } else {
                k -= size(tr[p].ch[0]) + 1;
                p = tr[p].ch[1];
            }
        }
        return 0;
    }  // 查询整棵树第k个节点，包含哨兵

    int range(int l, int r) {
        int x = kth_node(l + 1), y = kth_node(r + 3);
        splay(x);
        splay(y, x);
        return tr[y].ch[0];
    }  // 把[l,r]旋到右儿子的左子树

    void maintain_range() {
        int y = tr[root].ch[1];
        maintain(y);
        maintain(root);
    }  // 维护两个哨兵节点

    ll kth(int pos) {
        int p = kth_node(pos + 2);
        splay(p);
        return tr[p].val;
    }  // 查询a[pos]，保证0<=pos<size()

    void insert_tree(int pos, int mid) {
        int x = kth_node(pos + 1), y = kth_node(pos + 2);
        splay(x);
        splay(y, x);
        tr[y].ch[0] = mid;
        if (mid) tr[mid].fa = y;
        maintain(y);
        maintain(x);
    }  // 拼接一棵子树

    void insert(int pos, ll x) {
        insert_tree(pos, new_node(x));
    }  // 插到下标pos前面

    void insert(int pos, const vl& a) {
        insert_tree(pos, build_tree(a, 0, sz(a) - 1));
    }  // 插入一段序列

    int cut(int l, int r) {
        int mid = range(l, r);
        int y = tr[root].ch[1];
        tr[y].ch[0] = 0;
        if (mid) tr[mid].fa = 0;
        maintain(y);
        maintain(root);
        return mid;
    }  // 截取区间[l,r]

    void erase(int l, int r) {
        cut(l, r);
    }  // 删除区间[l,r]

    void reverse(int l, int r) {
        int mid = range(l, r);
        apply_rev(mid);
        maintain_range();
    }  // 翻转区间[l,r]

    void add(int l, int r, ll x) {
        int mid = range(l, r);
        apply_add(mid, x);
        maintain_range();
    }  // 区间加x

    void assign(int l, int r, ll x) {
        int mid = range(l, r);
        apply_assign(mid, x);
        maintain_range();
    }  // 区间赋值为x

    Info query(int l, int r) {
        int mid = range(l, r);
        return {size(mid), tr[mid].sum, tr[mid].mn, tr[mid].mx};
    }  // 查询区间信息

    void dfs(int p, vl& res) {
        if (!p) return;
        pushdown(p);
        dfs(tr[p].ch[0], res);
        res.push_back(tr[p].val);
        dfs(tr[p].ch[1], res);
    }

    vl to_vector() {
        vl res;
        dfs(root, res);
        if (!res.empty()) res.erase(res.begin());
        if (!res.empty()) res.pop_back();
        return res;
    }  // 调试用，导出整段序列
};

/*
用法：

下标均为0-indexed，区间为闭区间[l,r]。
插入位置pos表示插到下标pos前面，pos=size()表示插到末尾。

Splay sp(a);
sp.kth(pos);              // 查询a[pos]
sp.insert(pos, x);        // 插入单个数
sp.insert(pos, b);        // 插入一段序列
sp.erase(l, r);           // 删除区间[l,r]
sp.reverse(l, r);         // 翻转区间[l,r]
sp.add(l, r, x);          // 区间加x
sp.assign(l, r, x);       // 区间赋值为x
sp.query(l, r);           // 返回区间Info: siz,sum,mn,mx
int mid = sp.cut(l, r);   // 截取区间[l,r]，原树中删除这一段，返回这一段的根
sp.insert_tree(pos, mid); // 把mid插到下标pos前面
*/
