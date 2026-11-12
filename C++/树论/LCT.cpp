// Link-Cut Tree：维护动态森林，支持连边、断边、单点赋值和路径点权和。
// 点编号为 [0,n-1]，内部用 -1 表示空节点，点权和路径和均为 ll。
// 初始化 O(n)，各操作均摊 O(log n)，空间 O(n)；query 包含路径两端点。
struct LCT {
    struct Node {
        int ch[2] = {-1, -1}, fa = -1;
        ll val = 0, sum = 0;
        bool rev = false;
    };

    vector<Node> tr;
    vi st;

    LCT(int n) : tr(n) {}
    LCT(const vl& a) : LCT(sz(a)) {
        rep(i, 0, sz(a) - 1) tr[i].val = tr[i].sum = a[i];
    }  // 初始所有点互不连通

    bool is_root(int x) const {
        int p = tr[x].fa;
        return p == -1 || (tr[p].ch[0] != x && tr[p].ch[1] != x);
    }  // 判断辅助 Splay 的根；有 fa 但不是 fa 的儿子时，fa 是虚父亲

    int dir(int x) const {
        return tr[tr[x].fa].ch[1] == x;
    }

    void maintain(int x) {
        tr[x].sum = tr[x].val;
        for (int y : tr[x].ch) {
            if (y != -1) tr[x].sum += tr[y].sum;
        }
    }  // 只合并辅助树儿子，不包含虚子树

    void apply_rev(int x) {
        if (x == -1) return;
        swap(tr[x].ch[0], tr[x].ch[1]);
        tr[x].rev ^= 1;
    }

    void pushdown(int x) {
        if (!tr[x].rev) return;
        apply_rev(tr[x].ch[0]);
        apply_rev(tr[x].ch[1]);
        tr[x].rev = false;
    }

    void push_path(int x) {
        st.clear();
        st.emplace_back(x);
        while (!is_root(x)) {
            x = tr[x].fa;
            st.emplace_back(x);
        }
        while (!st.empty()) {
            pushdown(st.back());
            st.pop_back();
        }
    }  // 旋转前从辅助树根向下传标记，不能跨过虚父亲

    void rotate(int x) {
        int y = tr[x].fa, z = tr[y].fa;
        int k = dir(x), w = tr[x].ch[k ^ 1];
        if (!is_root(y)) tr[z].ch[dir(y)] = x;
        tr[x].fa = z;
        tr[x].ch[k ^ 1] = y;
        tr[y].fa = x;
        tr[y].ch[k] = w;
        if (w != -1) tr[w].fa = y;
        maintain(y);
        maintain(x);
    }

    void splay(int x) {
        push_path(x);
        while (!is_root(x)) {
            int y = tr[x].fa;
            if (!is_root(y)) {
                if (dir(x) == dir(y))
                    rotate(y);
                else
                    rotate(x);
            }
            rotate(x);
        }
    }

    void access(int x) {
        for (int y = -1, p = x; p != -1; y = p, p = tr[p].fa) {
            splay(p);
            tr[p].ch[1] = y;
            if (y != -1) tr[y].fa = p;
            maintain(p);
        }
        splay(x);
    }  // 打通原树根到 x 的路径；旧右儿子的 fa 要保留，实边转为虚边

    void makeroot(int x) {
        access(x);
        apply_rev(x);
    }  // 将 x 设为所在原树的根

    int findroot(int x) {
        access(x);
        pushdown(x);
        while (tr[x].ch[0] != -1) {
            x = tr[x].ch[0];
            pushdown(x);
        }
        splay(x);
        return x;
    }  // 返回当前原树根，编号仍为 0-based

    bool connected(int x, int y) {
        if (x == y) return true;
        int a = findroot(x), b = findroot(y);
        return a == b;
    }

    void split(int x, int y) {
        makeroot(x);
        access(y);
    }  // 若 x、y 连通，将路径 x-y 提取为以 y 为根的辅助树

    bool link(int x, int y) {
        makeroot(x);
        if (findroot(y) == x) return false;
        tr[x].fa = y;
        return true;
    }  // 加边 (x,y)；已连通则不加边，返回 false

    bool cut(int x, int y) {
        split(x, y);
        if (tr[y].ch[0] != x || tr[x].ch[1] != -1) return false;
        tr[y].ch[0] = tr[x].fa = -1;
        maintain(y);
        return true;
    }  // 只删除直接相连的边 (x,y)；无此边返回 false，不会断开整条路径

    void update(int x, ll val) {
        access(x);
        tr[x].val = val;
        maintain(x);
    }  // 将点 x 的权值赋为 val

    ll get(int x) const {
        return tr[x].val;
    }

    ll query(int x, int y) {
        split(x, y);
        return tr[y].sum;
    }  // 查询路径 x-y 的点权和，要求两点连通
};

/*
用法：

LCT lct(a);                    // 用 vl a 初始化点权，各点初始互不连通
// 全部点权初始化为 0 时写 LCT lct(n)。
lct.link(x, y);                // 加边 (x,y)，成功返回 true，形成环则返回 false
lct.cut(x, y);                 // 删除边 (x,y)，成功返回 true，无此边返回 false
bool same = lct.connected(x, y);
lct.update(x, val);            // 单点赋值
ll val = lct.get(x);           // 查询点权
ll ans = lct.query(x, y);      // 查询路径点权和，先保证 x、y 连通
lct.makeroot(x);               // 换根；连边、断边、路径查询也可能改变原树根
int root = lct.findroot(x);     // 查询当前根

改为路径异或时，将 maintain 中的 += 改成 ^=，sum 就表示路径异或值。
维护最小值 / 最大值时，改成 min / max 合并即可；本板不维护原树子树信息。
维护边权时，把每条边 (x,y) 拆成 x-e-y，新点 e 的点权为边权，原点权设为 0。
预先用 LCT lct(n+m) 分配原点和边点，删除该边时分别 cut(x,e)、cut(e,y)。
*/
