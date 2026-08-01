M = 2 * 10**8
is_prime = bytearray()
primes = []


def init() -> None:
    global is_prime

    is_prime = bytearray(b"\x01") * M
    is_prime[0] = is_prime[1] = False
    primes.clear()
    for i in range(2, M):
        if is_prime[i]:
            primes.append(i)
            for j in range(i * i, M, i):
                is_prime[j] = False


# Python 中避免导入文件时立即分配约 200 MB，使用前手动调用 init()
# 用法：init()
