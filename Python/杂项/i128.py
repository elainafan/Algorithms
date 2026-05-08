# 重载输入运算符以支持__int128类型
def read_int(s: str) -> int:
    return int(s)


# 重载输出运算符以支持__int128类型
def write_int(x: int) -> str:
    return str(x)


# Python int is arbitrary precision, so it covers C++ __int128 use cases.
