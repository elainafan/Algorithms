from typing import List, Tuple


def linear_sieve(mx: int) -> Tuple[List[bool], List[int]]:
    pd = [False] * mx
    primes = []
    for i in range(2, mx):
        if not pd[i]:
            primes.append(i)
        for p in primes:
            if i * p >= mx:
                break
            pd[i * p] = True
            if i % p == 0:
                break
    return pd, primes
