// 二维树状数组：单点加、前缀和、矩形区间和，外部下标均为 0-based。
// n 行 m 列，构造 O(nm)，单次 add / pre / query 为 O(log n log m)，空间 O(nm)。
template <typename T = long long>
class Tree2D {
    int n, m;
    vector<vector<T>> tree;

    void build() {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                int nxt = j + (j & -j);
                if (nxt <= m) tree[i][nxt] += tree[i][j];
            }
        }  // 先在每一行构造树状数组
        for (int i = 1; i <= n; i++) {
            int nxt = i + (i & -i);
            if (nxt <= n) {
                for (int j = 1; j <= m; j++) {
                    tree[nxt][j] += tree[i][j];
                }
            }
        }  // 再沿行下标向父节点累加，两个方向分开处理，避免重复计算
    }

public:
    // 初始化 n 行 m 列，所有元素的初始值为 val
    Tree2D(int n, int m, T val = 0)
        : n(n), m(m), tree(n + 1, vector<T>(m + 1, val)) {
        build();
    }

    // 使用给定矩阵在 O(nm) 时间内建树，data 的各行长度需相同
    Tree2D(const vector<vector<T>>& data)
        : n(data.size()), m(n ? data[0].size() : 0), tree(n + 1, vector<T>(m + 1)) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                tree[i][j] = data[i - 1][j - 1];
            }
        }
        build();
    }

    // 单点加：a[x][y] += val
    void add(int x, int y, T val = 1) {
        for (int i = x + 1; i <= n; i += i & -i) {
            for (int j = y + 1; j <= m; j += j & -j) {
                tree[i][j] += val;
            }
        }
    }

    // 前缀和：[0,x] × [0,y]；x 或 y 为 -1 时返回 0
    T pre(int x, int y) const {
        T res = 0;
        for (int i = x + 1; i > 0; i &= i - 1) {
            for (int j = y + 1; j > 0; j &= j - 1) {
                res += tree[i][j];
            }
        }
        return res;
    }

    // 矩形和：左上角 (x1,y1)，右下角 (x2,y2)，四条边均包含
    T query(int x1, int y1, int x2, int y2) const {
        if (x2 < x1 || y2 < y1) return 0;
        return pre(x2, y2) - pre(x1 - 1, y2) - pre(x2, y1 - 1) + pre(x1 - 1, y1 - 1);
    }
};

/*
用法：

Tree2D<ll> tree(n, m);  // 维护 n 行 m 列，初值为 0
// 指定统一初值时写 Tree2D<ll> tree(n, m, val)。
// 从已读入的 vvl a 建树时写 Tree2D<ll> tree(a)。

tree.add(x, y, val);                // a[x][y] 增加 val，省略 val 时增加 1
ll pre = tree.pre(x, y);            // 查询 [0,x] × [0,y] 的元素和
ll sum = tree.query(x1, y1, x2, y2);  // 查询 [x1,x2] × [y1,y2] 的元素和
ll cur = tree.query(x, y, x, y);     // 查询 a[x][y]

x 表示行，y 表示列。修改位置满足 0 <= x < n、0 <= y < m。
矩形使用 0-based 闭区间，空矩形返回 0；pre(-1,y) 和 pre(x,-1) 返回 0。
与一维板子一样，add 是增加值；单点赋值为 val 时，增加 val-tree.query(x,y,x,y)。
*/
