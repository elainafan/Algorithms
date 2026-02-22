// 交集 a&b
// 并集 a|b
// 对称差 a^b
// 相对补 a&(~b)
// 包含 a&b=a or a|b=b

// 集合大小 __builtin_popcount/__builtin_popcountll(s)
// 二进制长度 __lg(s）+1
// 集合最大元素 __lg(s)
// 集合最小元素 __builtin_ctz(s) 需要保证s!=0

// 遍历集合
for (int x = 0; i < n; x++) {
    if ((s >> x) & 1) {
        // operations
    }
}

// 枚举所有集合
for (int x = 0; x < (1 << n); x++) {
    // operations
}

// 枚举非空子集
for (int x = s; x; x = (x - 1) & s) {
    // operations
}

// 枚举所有子集
int x = s;
while (1) {
    if (x == 0) break;
    x = (x - 1) & s;
}

// 枚举超集
for (int x = s; x < (1 << n); x = (x + 1) | s) {
    // operations
}