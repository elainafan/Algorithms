from typing import List


def tree_diameter(edges: List[List[int]]) -> int:
    n = len(edges) + 1
    ma = [[] for _ in range(n)]
    for x, y in edges:
        ma[x].append(y)
        ma[y].append(x)

    ans = 0

    def dfs(x: int, fa: int) -> int:
        nonlocal ans
        best = 0
        for p in ma[x]:
            if p == fa:
                continue
            tem2 = dfs(p, x) + 1
            # if (s[x] == s[p]) continue;
            ans = max(ans, best + tem2)
            best = max(best, tem2)
        return best

    dfs(0, -1)
    return ans  # 这里求的是边的个数，如果是点的个数需要加一
