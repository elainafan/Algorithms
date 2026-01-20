class Node {
    int l, r;  // 维护当前节点的左右权值
    Node* lo;  // 当前节点的左孩子
    Node* ro;  // 当前节点的右孩子
    int cnt;   // [l,r]中的元素个数
    void maintain() {
        cnt = lo->cnt + ro->cnt;
        sum = lo->sum + ro->sum;
    }  // 更新操作

public:
    ll sum;  // [l,r]中元素总和

    Node(int l, int r, Node* lo = nullptr, Node* ro = nullptr, ll cnt = 0, ll sum = 0) : l(l), r(r), lo(lo), ro(ro), cnt(cnt), sum(sum) {}

    // 建一棵空树
    static Node* build(int l, int r) {
        Node* o = new Node(l, r);
        if (l == r) return o;
        int mid = (l + r) >> 1;
        o->lo = build(l, mid);
        o->ro = build(mid + 1, r);
        return o;
    }

    // 加入下标为i，权值为val的元素
    Node* add(int i, int val) {
        Node* o = new Node(l, r, lo, ro, cnt, sum);  // 先复制上一阶段的树
        if (l == r) {
            o->cnt++;
            o->sum += val;
            return o;
        }  // 若叶节点则进行操作
        int mid = (l + r) >> 1;
        if (i <= mid)
            o->lo = o->lo->add(i, val);
        else
            o->ro = o->ro->add(i, val);  // 根据加入的值选择递归左孩子还是右孩子
        o->maintain();                   // 更新后操作
        return o;
    }

    // 找[l,r]中第k小的数
    // 调用方式为 int tem=node[r].kth(node[l-1],k);
    int kth(Node* old, int k) {
        if (l == r) return l;                        // 叶子则返回
        int cnt_l = lo->cnt - old->lo->cnt;          // 左子树中的全部数量
        if (k <= cnt_l) return lo->kth(old->lo, k);  // 如果小于则到左子树中招
        return ro->kth(old->ro, k - cnt_l);          // 否则到右子树中找
    }

    // 查询[l,r]中有多少个数<=i，这些数对应的元素和是多少
    // 调用方式为 auto tem=node[r].query(node[l-1],i);
    pair<int, ll> query(Node* old, int i) {
        if (r <= i) return {cnt - old->cnt, sum - old->sum}; // 如果完全包裹
        auto [cnt, sum] = lo->query(old->lo, i); //  左子树查询
        int mid = (l + r) >> 1; 
        if (i > mid) {
            auto [c, s] = ro->query(old->ro, i);
            cnt += c;
            sum += s;
        } // 右子树查询
        return {cnt, sum};
    }
};