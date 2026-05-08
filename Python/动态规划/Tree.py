from typing import List, Tuple


def farthest_leaf_from_each_root(edges: List[List[int]]) -> List[int]:
    # 换根DP求每个节点为根的子树最大深度及其对应叶子
    n = len(edges) + 1
    ma = [[] for _ in range(n)]
    for x, y in edges:
        ma[x].append(y)
        ma[y].append(x)

    tem: List[Tuple[int, int, int, int]] = [(0, i, 0, i) for i in range(n)]  # 最长，最长编号，次长，次长编号

    def dfs(x: int, fa: int) -> Tuple[int, int]:
        m1, t1 = 0, x
        m2, t2 = 0, x
        for p in ma[x]:
            if p == fa:
                continue
            a, b = dfs(p, x)
            a += 1
            if a > m1:
                m2, t2 = m1, t1
                m1, t1 = a, b
            elif a > m2:
                m2 = a
                t2 = b
        tem[x] = (m1, t1, m2, t2)
        return m1, t1

    dfs(0, -1)
    res = [0] * n  # 代表从x往上走，不经过子树的最大长度
    up_id = list(range(n))  # 条件同上，节点编号
    res2 = [0] * n  # 最终节点

    def reroot(x: int, fa: int) -> None:
        down_len, down_id, second_len, second_id = tem[x]
        res2[x] = down_id if down_len >= res[x] else up_id[x]

        for p in ma[x]:
            if p == fa:
                continue
            use_len, use_id = down_len, down_id
            child_len, child_id, _, _ = tem[p]
            if child_len + 1 == down_len and child_id == down_id:
                use_len, use_id = second_len, second_id

            tem3 = use_len + 1
            id = use_id
            if res[x] + 1 > tem3:
                tem3 = res[x] + 1
                id = up_id[x]
            up_id[p] = id
            res[p] = tem3
            reroot(p, x)

    reroot(0, -1)
    return res2
