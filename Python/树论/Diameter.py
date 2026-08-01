# 无权树直径，0-based，ma 一般是邻接表
def tree_diameter(ma: list[list[int]]) -> tuple[int, int, int]:
    ans = 0
    L = 0
    R = 0

    def dfs(x: int, pa: int) -> tuple[int, int]:
        nonlocal ans, L, R
        tem = 0
        id = x  # 从 x 往下走，当前最长链长度和链尾端点
        for p in ma[x]:
            if p == pa:
                continue
            dep, to = dfs(p, x)
            tem2 = dep + 1
            # if s[x] == s[p]:
            #     continue
            if ans < tem + tem2:
                ans = tem + tem2
                L, R = id, to
            if tem < tem2:
                tem = tem2
                id = to
        return tem, id

    dfs(0, -1)
    return ans, L, R


def diameter_path(ma: list[list[int]], L: int, R: int) -> list[int]:
    fa = [-1] * len(ma)
    path = []

    def find_path(x: int, pa: int) -> bool:
        if x == R:
            return True
        for y in ma[x]:
            if y == pa:
                continue
            fa[y] = x
            if find_path(y, x):
                return True
        return False

    find_path(L, -1)
    x = R
    while x != -1:
        path.append(x)
        x = fa[x]
    path.reverse()
    return path


# 用法：
# ans, L, R = tree_diameter(ma)
# path = diameter_path(ma, L, R)
# ans 是直径边数，如果要点数则加一；L、R 是两个端点，path 是 L 到 R 的路径
