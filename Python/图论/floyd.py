from typing import List  # noqa: UP035


def floyd(n: int, edges: List[List[int]]) -> List[List[int]]:  # noqa: UP006
    dp = [[((1 << 63) - 1) // 2] * n for _ in range(n)]
    for i in range(n):
        dp[i][i] = 0
    for p in edges:
        dp[p[0]][p[1]] = min(dp[p[0]][p[1]], p[2])
        dp[p[1]][p[0]] = min(dp[p[1]][p[0]], p[2])
    for k in range(n):
        for i in range(n):
            if dp[i][k] == ((1 << 63) - 1) // 2:
                continue
            for j in range(n):
                dp[i][j] = min(dp[i][j], dp[i][k] + dp[k][j])
    return dp
