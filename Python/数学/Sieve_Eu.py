MX = 6 * 10**6 + 1
pd = bytearray(MX)
primes = []

for i in range(2, MX):
    if not pd[i]:
        primes.append(i)
    for p in primes:
        if p * i >= MX:
            break
        pd[i * p] = True
        if i % p == 0:
            break
