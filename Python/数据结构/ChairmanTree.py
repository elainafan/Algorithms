from dataclasses import dataclass
from typing import List, Tuple


@dataclass
class PersistentSumNode:
    lo: int = 0  # 左孩子
    ro: int = 0  # 右孩子
    sum: int = 0  # 总值


class PersistentSegmentTree:
    # 下面是维护多个版本的可持久化普通线段树
    # 根据具体情况修改
    # 维护树

    def __init__(self, a: List[int] | None = None, n: int | None = None):
        if a is None and n is None:
            raise ValueError("need a or n")
        self.n = len(a) if a is not None else n
        self.nodes = [PersistentSumNode()]
        self.root = self._build(0, self.n - 1, a)

    def _new(self, node: PersistentSumNode) -> int:
        self.nodes.append(PersistentSumNode(node.lo, node.ro, node.sum))
        return len(self.nodes) - 1

    def _build(self, l: int, r: int, a: List[int] | None) -> int:
        # 建一个空树
        # 根据数组a建树
        cur = len(self.nodes)
        self.nodes.append(PersistentSumNode())
        if l == r:
            self.nodes[cur].sum = 0 if a is None else a[l]
            return cur
        m = (l + r) >> 1
        self.nodes[cur].lo = self._build(l, m, a)
        self.nodes[cur].ro = self._build(m + 1, r, a)
        self.nodes[cur].sum = self.nodes[self.nodes[cur].lo].sum + self.nodes[self.nodes[cur].ro].sum
        return cur

    def update(self, root: int, pos: int, val: int) -> int:
        # 将位置i的值更新为val
        # 调用：node[r+1]=node[l]->update(0,n-1,i,val);
        return self._update(root, 0, self.n - 1, pos, val)

    def _update(self, root: int, l: int, r: int, pos: int, val: int) -> int:
        cur = self._new(self.nodes[root])
        if l == r:
            self.nodes[cur].sum = val
            return cur
        m = (l + r) >> 1
        if pos <= m:
            self.nodes[cur].lo = self._update(self.nodes[cur].lo, l, m, pos, val)
        else:
            # 根据加入的值选择递归左孩子还是右孩子
            self.nodes[cur].ro = self._update(self.nodes[cur].ro, m + 1, r, pos, val)
        self.nodes[cur].sum = self.nodes[self.nodes[cur].lo].sum + self.nodes[self.nodes[cur].ro].sum
        # 更新后操作
        return cur

    def query(self, root: int, ql: int, qr: int) -> int:
        # 查询[ql,qr]的和
        # 调用：ll tem=Node::query(node[r],0,n-1,ql,qr);
        return self._query(root, 0, self.n - 1, ql, qr)

    def _query(self, root: int, l: int, r: int, ql: int, qr: int) -> int:
        if ql <= l and r <= qr:
            return self.nodes[root].sum
        m = (l + r) >> 1
        ans = 0
        if ql <= m:
            ans += self._query(self.nodes[root].lo, l, m, ql, qr)
        if qr > m:
            ans += self._query(self.nodes[root].ro, m + 1, r, ql, qr)
        return ans


@dataclass
class ValueNode:
    lo: int = 0  # 当前节点的左孩子
    ro: int = 0  # 当前节点的右孩子
    cnt: int = 0  # [l,r]中的元素个数
    sum: int = 0  # [l,r]中元素总和


class PersistentValueSegmentTree:
    # 下面是可持久化权值线段树，按道理说这才是真正的主席树
    # 维护当前节点的左右权值
    # 更新操作

    def __init__(self, value_count: int):
        self.n = value_count
        self.nodes = [ValueNode()]
        self.empty_root = self._build(0, self.n - 1)

    def _build(self, l: int, r: int) -> int:
        # 建一棵空树
        cur = len(self.nodes)
        self.nodes.append(ValueNode())
        if l != r:
            m = (l + r) >> 1
            self.nodes[cur].lo = self._build(l, m)
            self.nodes[cur].ro = self._build(m + 1, r)
        return cur

    def _new(self, root: int) -> int:
        x = self.nodes[root]
        self.nodes.append(ValueNode(x.lo, x.ro, x.cnt, x.sum))
        return len(self.nodes) - 1

    def update(self, root: int, pos: int, val: int) -> int:
        # 加入下标为i，权值为val的元素
        # 通常调用为 node[i]=node[i-1]->update(tem,nums[i-1]);
        # 若要在版本v上修改，直接 node[cnt+1]=node[v]->update(tem,nums[i-1]);
        return self._update(root, 0, self.n - 1, pos, val)

    def _update(self, root: int, l: int, r: int, pos: int, val: int) -> int:
        # 先复制上一阶段的树
        cur = self._new(root)
        if l == r:
            self.nodes[cur].cnt += 1
            self.nodes[cur].sum += val
            # 若叶节点则进行操作
            return cur
        m = (l + r) >> 1
        if pos <= m:
            self.nodes[cur].lo = self._update(self.nodes[cur].lo, l, m, pos, val)
        else:
            # 根据加入的值选择递归左孩子还是右孩子
            self.nodes[cur].ro = self._update(self.nodes[cur].ro, m + 1, r, pos, val)
        lo = self.nodes[cur].lo
        ro = self.nodes[cur].ro
        self.nodes[cur].cnt = self.nodes[lo].cnt + self.nodes[ro].cnt
        self.nodes[cur].sum = self.nodes[lo].sum + self.nodes[ro].sum
        # 更新后操作
        return cur

    def kth(self, root: int, old: int, k: int) -> int:
        # 找[l,r]中第k小的数
        # 调用方式为 int tem=node[r]->kth(node[l-1],k);
        return self._kth(root, old, 0, self.n - 1, k)

    def _kth(self, root: int, old: int, l: int, r: int, k: int) -> int:
        if l == r:
            # 叶子则返回
            return l
        m = (l + r) >> 1
        cnt_l = self.nodes[self.nodes[root].lo].cnt - self.nodes[self.nodes[old].lo].cnt  # 左子树中的全部数量
        if k <= cnt_l:
            # 如果小于则到左子树中招
            return self._kth(self.nodes[root].lo, self.nodes[old].lo, l, m, k)
        # 否则到右子树中找
        return self._kth(self.nodes[root].ro, self.nodes[old].ro, m + 1, r, k - cnt_l)

    def query_le(self, root: int, old: int, pos: int) -> Tuple[int, int]:
        # 查询[l,r]中有多少个数<=i，这些数对应的元素和是多少
        # 调用方式为 auto tem=node[r]->query(node[l-1],i);
        return self._query_le(root, old, 0, self.n - 1, pos)

    def _query_le(self, root: int, old: int, l: int, r: int, pos: int) -> Tuple[int, int]:
        if r <= pos:
            # 如果完全包裹
            return self.nodes[root].cnt - self.nodes[old].cnt, self.nodes[root].sum - self.nodes[old].sum
        m = (l + r) >> 1
        #  左子树查询
        cnt, sum = self._query_le(self.nodes[root].lo, self.nodes[old].lo, l, m, pos)
        if pos > m:
            c, s = self._query_le(self.nodes[root].ro, self.nodes[old].ro, m + 1, r, pos)
            cnt += c
            sum += s
            # 右子树查询
        return cnt, sum


# 下面是一道典题，LC3762，主席树+中位数贪心+离散化
