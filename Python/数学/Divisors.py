MX = 2 * 10**5 + 5
divisors = [[] for _ in range(MX)]

for i in range(1, MX):
    for j in range(i, MX, i):
        divisors[j].append(i)


# 用法：
# ds = divisors[x]  # x 的全部正因数，已从小到大排列
# 预处理复杂度 O(MX log MX)
