class RollbackUnionFind {
    vector<int> fa;
    vector<int> siz;  // 集合大小
    vector<pair<int, int>> his;  // 每次成功合并记录(被挂的根, 新根)，失败合并记录(-1, -1)

public:
    int cc;  // 连通块个数
    RollbackUnionFind(int n) : fa(n), siz(n, 1), cc(n) { iota(fa.begin(), fa.end(), 0); }

    int get(int x) {  // 不能路径压缩，否则无法回滚
        while (fa[x] != x) x = fa[x];
        return x;
    }

    bool is_same(int x, int y) { return get(x) == get(y); }

    int snapshot() { return his.size(); }  // 记录当前历史栈大小，之后可rollback回来

    bool merge(int from, int to) {
        int x = get(from), y = get(to);
        if (x == y) {
            // 记录一次空操作，使undo次数和merge调用次数一一对应
            his.emplace_back(-1, -1);
            return false;
        }
        // 按大小合并，保证树高为O(logn)
        if (siz[x] > siz[y]) swap(x, y);
        his.emplace_back(x, y);
        fa[x] = y;
        siz[y] += siz[x];
        cc--;
        return true;
    }

    void undo() {
        auto [x, y] = his.back();
        his.pop_back();
        if (x == -1) return;
        // 撤销x挂到y下面这一步
        siz[y] -= siz[x];
        fa[x] = x;
        cc++;
    }

    void rollback(int t) {
        while ((int)his.size() > t) undo();
    }

    int get_size(int x) { return siz[get(x)]; }  // 查询x所在集合大小
};

/*
使用示例：

void solve() {
    RollbackUnionFind uf(5);
    int t = uf.snapshot();

    uf.merge(0, 1);
    uf.merge(1, 2);
    cout << uf.is_same(0, 2) << endl;  // 1
    cout << uf.get_size(0) << endl;    // 3
    cout << uf.cc << endl;             // 3

    uf.rollback(t);
    cout << uf.is_same(0, 2) << endl;  // 0
    cout << uf.cc << endl;             // 5
}

常见场景：
1. 分治处理离线动态图连通性，每进入一段区间前snapshot，退出时rollback。
2. 搜索/回溯中临时合并集合，递归返回后撤销。
*/
