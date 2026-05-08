from typing import List, Tuple


def eratosthenes(mx: int) -> Tuple[List[bool], List[int]]:
    is_prime = [True] * mx
    if mx > 0:
        is_prime[0] = False
    if mx > 1:
        is_prime[1] = False

    primes = []
    for i in range(2, mx):
        if is_prime[i]:
            primes.append(i)
            if i * i < mx:
                for j in range(i * i, mx, i):
                    is_prime[j] = False
    # 函数指针在创建时自动调用
    return is_prime, primes
