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
    // 通常调用为 node[i]=node[i-1]->add(tem,nums[i-1]);
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
    // 调用方式为 int tem=node[r]->kth(node[l-1],k);
    int kth(Node* old, int k) {
        if (l == r) return l;                        // 叶子则返回
        int cnt_l = lo->cnt - old->lo->cnt;          // 左子树中的全部数量
        if (k <= cnt_l) return lo->kth(old->lo, k);  // 如果小于则到左子树中招
        return ro->kth(old->ro, k - cnt_l);          // 否则到右子树中找
    }

    // 查询[l,r]中有多少个数<=i，这些数对应的元素和是多少
    // 调用方式为 auto tem=node[r]->query(node[l-1],i);
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

// 下面是一道典题，LC3762，主席树+中位数贪心+离散化

#include <bits/stdc++.h>
#define ll long long
#define ull unsigned long long
#define lowbit(x) (x & (-x))
using namespace std;
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
    // 调用方式为 int tem=node[r]->kth(node[l-1],k);
    int kth(Node* old, int k) {
        if (l == r) return l;                        // 叶子则返回
        int cnt_l = lo->cnt - old->lo->cnt;          // 左子树中的全部数量
        if (k <= cnt_l) return lo->kth(old->lo, k);  // 如果小于则到左子树中招
        return ro->kth(old->ro, k - cnt_l);          // 否则到右子树中找
    }

    // 查询[l,r]中有多少个数<=i，这些数对应的元素和是多少
    // 调用方式为 auto tem=node[r]->query(node[l-1],i);
    pair<int, ll> query(Node* old, int i) {
        if (r <= i) return {cnt - old->cnt, sum - old->sum};  // 如果完全包裹
        auto [cnt, sum] = lo->query(old->lo, i);              //  左子树查询
        int mid = (l + r) >> 1;
        if (i > mid) {
            auto [c, s] = ro->query(old->ro, i);
            cnt += c;
            sum += s;
        }  // 右子树查询
        return {cnt, sum};
    }
};
class Solution {
public:
    vector<long long> minOperations(vector<int>& nums, int k, vector<vector<int>>& queries) {
        int n = nums.size();
        vector<ll> res;
        vector<int> pre(n + 1, 0);
        for (int i = 2; i <= n; i++) {
            pre[i] = pre[i - 1] + (!(nums[i - 1] % k == nums[i - 2] % k));
        }
        auto sorted = nums;
        ranges::sort(sorted);
        sorted.erase(unique(sorted.begin(), sorted.end()), sorted.end());
        int m = sorted.size();
        vector<Node*> node(n + 1);
        node[0] = Node::build(0, m - 1);
        for (int i = 1; i <= n; i++) {
            auto x = ranges::lower_bound(sorted, nums[i - 1]);
            int tem = x - sorted.begin();
            node[i] = node[i - 1]->add(tem, nums[i - 1]);
        }
        for (auto p : queries) {
            int l = p[0], r = p[1];
            if (pre[r + 1] != pre[l + 1]) {
                res.push_back(-1);
                continue;
            }
            int tem = node[r + 1]->kth(node[l], (r - l) / 2 + 1);
            int tem2 = sorted[tem];
            auto [x, y] = node[r + 1]->query(node[l], tem);
            ll ans = 0;
            ll tot = node[r + 1]->sum - node[l]->sum;
            ans += 1LL * x * tem2 - y;
            ans += tot - y - 1LL * (r - l + 1 - x) * tem2;
            res.push_back(ans / k);
        }
        return res;
    }
};