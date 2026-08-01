MX = 10**5 + 1
phi = [0] * MX


def _init() -> None:
    def Phi(n: int) -> int:
        res = n
        i = 2
        while i * i <= n:
            if n % i == 0:
                res = res // i * (i - 1)
                while n % i == 0:
                    n //= i
            i += 1
        if n > 1:
            res = res // n * (n - 1)
        return res

    for i in range(1, MX):
        phi[i] = Phi(i)


_init()


# 用法：
# value = phi[x]  # 已预处理 1 <= x < MX
