# 交集 a & b
# 并集 a | b
# 对称差 a ^ b
# 相对补 a & (~b)
# 包含 a & b == a or a | b == b

# 集合大小 bin(s).count("1")
# 二进制长度 s.bit_length()
# 集合最大元素 s.bit_length() - 1
# 集合最小元素 (s & -s).bit_length() - 1，需要保证 s != 0

# 遍历集合
# for x in range(n):
#     if s >> x & 1:
#         # operations
#         pass

# 枚举所有集合
# for x in range(1 << n):
#     # operations
#     pass

# 枚举非空子集
# x = s
# while x:
#     # operations
#     x = (x - 1) & s

# 枚举所有子集
# x = s
# while True:
#     # operations
#     if x == 0:
#         break
#     x = (x - 1) & s

# 枚举超集
# x = s
# while x < 1 << n:
#     # operations
#     x = (x + 1) | s
