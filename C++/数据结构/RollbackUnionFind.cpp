// 可撤销并查集：支持按合并调用逐次撤销，以及回滚到当前历史链上的快照。
// 元素为 0-indexed；按大小合并但不路径压缩，保证历史可以恢复。

class RollbackUnionFind {
    vector<int> fa;
    vector<int> siz;
    vector<int> his;  // 成功合并记录被挂根，失败合并记录 -1
    int cc;

public:
    // n >= 0；max_operations >= 0，是预计的 merge 调用次数（包含失败合并）。
    RollbackUnionFind(int n, int max_operations = 0)
        : fa(n), siz(n, 1), cc(n) {
        iota(fa.begin(), fa.end(), 0);
        his.reserve((size_t)max_operations);
    }

    int get(int x) const {
        while (fa[x] != x) x = fa[x];
        return x;
    }  // 不能路径压缩，否则无法回滚

    bool is_same(int x, int y) const {
        return get(x) == get(y);
    }

    int get_size(int x) const {
        return siz[get(x)];
    }

    int components() const {
        return cc;
    }

    int snapshot() const {
        return sz(his);
    }  // 返回当前历史深度，可交给 rollback

    bool merge(int x, int y) {
        x = get(x);
        y = get(y);
        if (x == y) {
            his.push_back(-1);
            return false;
        }

        if (siz[x] > siz[y]) swap(x, y);
        his.push_back(x);
        fa[x] = y;
        siz[y] += siz[x];
        cc--;
        return true;
    }  // 失败合并也占一条历史，使每次 merge 都恰好对应一次 undo

    bool undo() {
        if (his.empty()) return false;
        int x = his.back();
        his.pop_back();
        if (x == -1) return true;

        // 后续操作已按 LIFO 撤销，此时 fa[x] 就是合并时的新根。
        int y = fa[x];
        siz[y] -= siz[x];
        fa[x] = x;
        cc++;
        return true;
    }  // 消费最后一条 merge 历史；true 不代表连通结构一定改变

    bool rollback(int t) {
        if (t < 0 || t > snapshot()) return false;
        while (snapshot() > t) undo();
        return true;
    }  // 深度越界时保持状态不变并返回 false

    void reserve_operations(int max_operations) {
        his.reserve(his.size() + (size_t)max_operations);
    }  // max_operations >= 0，为后续 merge 调用预留内存
};

/*
用法：

RollbackUnionFind uf(n, q);       // n 个独立集合；为至多 q 次 merge 预留历史空间
uf.reserve_operations(extra_q);   // 需要时再为后续 extra_q 次 merge 追加预留

int snap = uf.snapshot();         // 保存当前状态
bool joined = uf.merge(x, y);     // 是否真的合并；失败调用也会写入一条空历史
uf.merge(y, z);

int leader = uf.get(x);           // x 当前所在集合的代表元素
bool same = uf.is_same(x, z);     // x,z 当前是否连通
int size = uf.get_size(x);        // x 当前所在集合的大小
int cc = uf.components();         // 当前连通块数

bool undone = uf.undo();          // 撤销最后一次 merge 调用
bool restored = uf.rollback(snap); // 撤销到 snap；也适合 DFS 进入/退出前后成对使用

常用于分治处理离线动态图连通性，以及搜索、回溯中的临时合并。
失败 merge 也对应一次 undo；此时 undo 仍返回 true，没有历史时才返回 false。
snapshot 只是当前历史深度。rollback(t) 只会沿当前历史链向前撤销，不能恢复已经
撤销并被新分支替代的状态，也无法识别来自旧分支的同深度快照；旧快照不得跨分支复用。
0 <= t <= snapshot() 时成功，否则返回 false 且不修改状态。

所有涉及元素下标的接口均不做边界检查：需保证 n >= 0、0 <= x,y < n、
max_operations >= 0。历史深度和预计操作数需能存入 int。

初始化 O(n)；按大小合并保证树高 O(log n)，因此 get/is_same/get_size/merge
最坏 O(log n)；snapshot/components O(1)，undo O(1)，rollback O(撤销次数)。
不做路径压缩，否则查询会修改 fa，无法只用合并历史恢复状态。

fa、siz 各为每个元素一个 int；his 每次 merge 只存一个 int（失败时存 -1）。
设元素数为 n、预留或曾达到的最大历史容量为 M，总空间 O(n+M)。
*/
