# Python 算法竞赛语法清单

这份清单面向“用 Python 打算法竞赛/刷 LeetCode/写模板”的场景。重点不是系统学 Python，而是把常用语法糖、库函数、坑点，以及它们和 C++ 的对应关系整理在一起。

## 常用 import 模板

```python
from collections import defaultdict, deque, Counter
from functools import cache
from sortedcontainers import SortedSet, SortedDict, SortedList
from typing import List, Tuple, Set
from math import inf
from heapq import heappop, heappush
from bisect import bisect_left, bisect_right
import math

MOD = 10**9 + 7
```

说明：

- `collections`：常用容器，类似 C++ 的 `queue / deque / map默认值 / multiset计数`
- `functools.cache`：记忆化搜索，类似 C++ 里手写 `map/数组 memo`
- `sortedcontainers`：第三方有序容器，类似 C++ 的 `set / map / multiset`
- `typing`：类型提示，LeetCode 常用，不影响运行逻辑
- `math.inf`：无穷大，类似 C++ 的 `INF`
- `heapq`：堆，类似 C++ 的 `priority_queue`
- `bisect`：二分，类似 C++ 的 `lower_bound / upper_bound`
- `math`：数学函数库

注意：

- `sortedcontainers` 不是 Python 标准库，LeetCode 通常有，普通 OJ 不一定有。
- 如果 OJ 没有 `sortedcontainers`，需要换成堆、树状数组、线段树、离散化，或者手写有序结构。

## 输入输出

### 普通输入

```python
n = int(input())
a = list(map(int, input().split()))
x, y, z = map(int, input().split())
```

对应 C++：

```cpp
int n;
cin >> n;
vector<int> a(n);
for (int& x : a) cin >> x;
```

区别：

- Python 的 `input()` 一次读一行。
- `map(int, input().split())` 是惰性迭代器，通常外面套 `list()`。

### 快速输入

```python
import sys
input = sys.stdin.readline

n = int(input())
a = list(map(int, input().split()))
```

如果数据特别大：

```python
import sys
data = list(map(int, sys.stdin.buffer.read().split()))
```

这种写法类似 C++ 快读。常见用法：

```python
it = iter(data)
n = next(it)
m = next(it)
edges = [(next(it), next(it), next(it)) for _ in range(m)]
```

### 输出

```python
print(ans)
print(*a)
print("\n".join(map(str, res)))
```

对应 C++：

```cpp
cout << ans << '\n';
for (int x : a) cout << x << ' ';
```

区别：

- 大量输出时，不要循环里频繁 `print`，优先收集到 `res` 后 `join`。
- `print(*a)` 会用空格展开列表。

## 基础类型

### int

Python 的 `int` 是任意精度整数，类似 C++ 的 `long long` + `__int128` + 大整数。

```python
x = 10**100
```

对应 C++：

```cpp
__int128 x; // 但 C++ 仍有范围上限
```

区别：

- Python 不会整数溢出。
- 但大整数运算比机器整数慢。

### float 和 inf

```python
from math import inf

dis = [inf] * n
```

对应 C++：

```cpp
const long long INF = 4e18;
vector<long long> dis(n, INF);
```

注意：

- 如果全程是整数，通常也可以用 `10**30` 当 `INF`。
- `math.inf` 是浮点无穷大，和整数比较没问题，但最终答案如果必须是整数，注意不要混出浮点。

### bool

```python
flag = True
flag = False
```

对应 C++：

```cpp
bool flag = true;
```

Python 中：

```python
int(True) == 1
int(False) == 0
```

所以可以写：

```python
ans += condition
```

类似 C++：

```cpp
ans += condition;
```

### None

`None` 类似 C++ 的 `nullptr` 或“无值”。

```python
parent = None
```

判断必须用：

```python
if parent is None:
    ...
```

不要写：

```python
if parent == None:
    ...
```

## 运算符

### 除法

```python
a / b   # 真除法，结果是 float
a // b  # 向下取整除法
a % b   # 取模
```

对应 C++：

```cpp
a / b
a % b
```

区别：

- Python 的 `//` 是向负无穷取整。
- C++ 整数除法是向 0 取整。

例子：

```python
-3 // 2 == -2
-3 % 2 == 1
```

C++ 中：

```cpp
-3 / 2 == -1
-3 % 2 == -1
```

如果要模拟 C++ 向 0 除法：

```python
int(a / b)
```

### 快速幂和模幂

```python
pow(x, y)
pow(x, y, MOD)
```

对应 C++：

```cpp
qpow(x, y)
qpow(x, y, MOD)
```

Python 的三参数 `pow(x, y, MOD)` 是内置快速幂，通常不用自己写。

求逆元：

```python
inv = pow(x, MOD - 2, MOD)
```

### 位运算

```python
x & y
x | y
x ^ y
~x
x << k
x >> k
x.bit_count()
x.bit_length()
```

对应 C++：

```cpp
x & y
x | y
x ^ y
~x
x << k
x >> k
__builtin_popcount(x)
bit_width(x)
```

常用：

```python
lowbit = x & -x
i = lowbit.bit_length() - 1
```

对应 C++：

```cpp
int lowbit = x & -x;
int i = __builtin_ctz(x);
```

## 条件、循环、缩进

Python 用缩进表示代码块，没有 `{}`。

```python
if x < y:
    x, y = y, x
```

对应 C++：

```cpp
if (x < y) {
    swap(x, y);
}
```

### range

```python
for i in range(n):          # 0 到 n - 1
for i in range(1, n + 1):   # 1 到 n
for i in range(n - 1, -1, -1):  # n - 1 到 0
```

对应 C++：

```cpp
for (int i = 0; i < n; i++)
for (int i = 1; i <= n; i++)
for (int i = n - 1; i >= 0; i--)
```

### while

```python
while x:
    x &= x - 1
```

和 C++ 类似。

### 链式比较

```python
if l <= x <= r:
    ...
```

对应 C++：

```cpp
if (l <= x && x <= r) {
    ...
}
```

## 列表 list

Python 的 `list` 类似 C++ 的 `vector`。

```python
a = []
a.append(x)
a.pop()
a[-1]
len(a)
```

对应 C++：

```cpp
vector<int> a;
a.push_back(x);
a.pop_back();
a.back();
a.size();
```

### 初始化

```python
a = [0] * n
ma = [[] for _ in range(n)]
dp = [[0] * m for _ in range(n)]
```

注意大坑：

```python
dp = [[0] * m] * n  # 错
```

这会让每一行引用同一个列表。

正确：

```python
dp = [[0] * m for _ in range(n)]
```

### 切片

```python
a[l:r]      # [l, r)
a[l:r + 1]  # [l, r]
a[::-1]     # 反转副本
```

对应 C++：

```cpp
vector<int> b(a.begin() + l, a.begin() + r);
reverse(a.begin(), a.end());
```

注意：

- 切片会复制，复杂度是切片长度。
- 大数据里不要频繁切片。

### 排序

```python
a.sort()
a.sort(reverse=True)
a.sort(key=lambda x: x[1])
b = sorted(a)
```

对应 C++：

```cpp
sort(a.begin(), a.end());
sort(a.rbegin(), a.rend());
sort(a.begin(), a.end(), [](auto& x, auto& y) { return x[1] < y[1]; });
```

区别：

- `a.sort()` 原地排序，返回 `None`。
- `sorted(a)` 返回新列表。
- Python 的排序是稳定排序。

### 自定义排序

```python
a.sort(key=lambda x: (x[0], -x[1]))
```

对应 C++：

```cpp
sort(a.begin(), a.end(), [](auto& x, auto& y) {
    if (x[0] != y[0]) return x[0] < y[0];
    return x[1] > y[1];
});
```

如果必须用比较函数：

```python
from functools import cmp_to_key

def cmp(x, y):
    return x - y

a.sort(key=cmp_to_key(cmp))
```

竞赛里优先用 `key`，少用 `cmp_to_key`，因为慢。

## 字符串 str

```python
s = input().strip()
len(s)
s[i]
s[l:r]
s.count("a")
s.find("abc")
```

对应 C++：

```cpp
string s;
cin >> s;
s.size();
s[i];
s.substr(l, r - l);
```

区别：

- Python 字符串不可变，不能 `s[i] = 'a'`。
- 要修改字符串，转成列表：

```python
t = list(s)
t[i] = "a"
s = "".join(t)
```

### 字符和 ASCII

```python
ord("a")      # 97
chr(97)       # 'a'
ord(c) - ord("a")
```

对应 C++：

```cpp
c - 'a'
char('a' + x)
```

## 元组 tuple

```python
p = (x, y)
x, y = p
```

类似 C++：

```cpp
pair<int, int> p;
auto [x, y] = p;
```

元组可比较：

```python
(1, 2) < (1, 3)
```

类似 C++ 的 `pair/vector` 字典序比较。

常用于堆、排序 key、字典 key。

## 解包和交换

```python
x, y = y, x
a, b, c = p
first, *mid, last = a
```

对应 C++：

```cpp
swap(x, y);
auto [a, b, c] = p;
```

注意：

- `*mid` 会创建新列表，可能有额外开销。

## 列表推导式和生成器

### 列表推导式

```python
a = [int(x) for x in input().split()]
squares = [x * x for x in range(n)]
```

对应 C++：

```cpp
for (...) a.push_back(...);
```

### 带条件

```python
b = [x for x in a if x > 0]
```

对应 C++：

```cpp
for (int x : a) if (x > 0) b.push_back(x);
```

### 生成器表达式

```python
ans = sum(x * x for x in a)
ok = all(x > 0 for x in a)
has = any(x == target for x in a)
```

区别：

- `[...]` 会生成列表。
- `(...)` 是惰性生成器，常配合 `sum/all/any/max/min`。

## 三目表达式

```python
x = a if cond else b
```

对应 C++：

```cpp
int x = cond ? a : b;
```

例子：

```python
lo = int(num1[cnt - dif]) if limit_low and cnt >= dif else 0
```

## lambda

```python
a.sort(key=lambda x: x[1])
```

对应 C++：

```cpp
sort(a.begin(), a.end(), [](auto& x, auto& y) {
    return x[1] < y[1];
});
```

Python 的 `lambda` 只能写表达式，不能写多行语句。

## 作用域、闭包、nonlocal

嵌套函数可以读外层变量：

```python
def solve():
    n = 10

    def dfs(x):
        return x + n
```

如果要修改外层变量，要用 `nonlocal`：

```python
ans = 0

def dfs(x):
    nonlocal ans
    ans = max(ans, x)
```

对应 C++ lambda：

```cpp
int ans = 0;
auto dfs = [&](auto&& dfs, int x) -> void {
    ans = max(ans, x);
};
```

区别：

- Python 读取外层变量可以直接读。
- Python 赋值会被视为局部变量，必须 `nonlocal`。
- 如果外层变量是列表/字典，修改其内容不需要 `nonlocal`。

## 记忆化搜索 cache

```python
from functools import cache

@cache
def dfs(x, y):
    ...
```

对应 C++：

```cpp
map<pair<int,int>, long long> memo;
auto dfs = [&](...) { ... };
```

Python 中用了 `@cache` 后，不需要再手写 `ma` 字典存状态。

数位 DP 常见写法：

```python
@cache
def dfs(cnt: int, s: int, limit_low: bool, limit_high: bool) -> int:
    ...
```

注意：

- 参数必须可哈希，比如 `int / bool / str / tuple`。
- `list / dict / set` 不能作为 `@cache` 参数。
- 多组测试时，如果 `dfs` 定义在 `solve` 内部，每组会自动新建缓存。
- 如果 `dfs` 定义在外面，多组测试要 `dfs.cache_clear()`。

## dict

Python 的 `dict` 类似 C++ 的 `unordered_map`。

```python
ma = {}
ma[x] = y
if x in ma:
    ...
ma.get(x, 0)
```

对应 C++：

```cpp
unordered_map<int, int> ma;
ma[x] = y;
if (ma.count(x)) ...
```

区别：

- Python `dict` 平均 O(1)，且保持插入顺序。
- `ma[x]` 如果不存在会报错。
- `ma.get(x, default)` 不存在时返回默认值。

## defaultdict

```python
from collections import defaultdict

ma = defaultdict(list)
ma[x].append(y)

cnt = defaultdict(int)
cnt[x] += 1
```

对应 C++：

```cpp
unordered_map<int, vector<int>> ma;
ma[x].push_back(y);

unordered_map<int, int> cnt;
cnt[x]++;
```

常见建图：

```python
ma = [[] for _ in range(n)]
for x, y in edges:
    ma[x].append(y)
    ma[y].append(x)
```

如果点编号很稀疏：

```python
ma = defaultdict(list)
```

## Counter

```python
from collections import Counter

cnt = Counter(a)
cnt[x] += 1
cnt[x] -= 1
cnt.most_common()
```

类似 C++：

```cpp
unordered_map<int, int> cnt;
```

区别：

- `Counter` 查询不存在的键返回 0。
- 减到 0 后键可能仍存在。
- 可以手动删除：

```python
if cnt[x] == 0:
    del cnt[x]
```

## set

```python
s = set()
s.add(x)
s.remove(x)   # 不存在会报错
s.discard(x)  # 不存在不报错
x in s
```

对应 C++：

```cpp
unordered_set<int> s;
s.insert(x);
s.erase(x);
s.count(x);
```

集合运算：

```python
a & b  # 交集
a | b  # 并集
a ^ b  # 对称差
a - b  # 差集
```

## deque

```python
from collections import deque

q = deque()
q.append(x)
q.appendleft(x)
q.pop()
q.popleft()
```

对应 C++：

```cpp
deque<int> q;
queue<int> q;
```

常用于 BFS：

```python
q = deque([start])
while q:
    x = q.popleft()
```

注意：

- `list.pop(0)` 是 O(n)，队列要用 `deque.popleft()`。

## heapq

Python 的 `heapq` 是小根堆。

```python
from heapq import heappop, heappush

h = []
heappush(h, (dis, x))
dis, x = heappop(h)
```

对应 C++：

```cpp
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
```

大根堆：

```python
heappush(h, -x)
x = -heappop(h)
```

多关键字：

```python
heappush(h, (dist, node))
```

元组会按字典序比较，类似 C++ `pair`。

注意：

- Python 堆不支持直接删除任意元素。
- 常用“懒删除”：弹出时检查是否过期。

Dijkstra 常见写法：

```python
dis = [inf] * n
dis[start] = 0
h = [(0, start)]
while h:
    dis_x, x = heappop(h)
    if dis_x > dis[x]:
        continue
    for y, z in ma[x]:
        new_y = dis_x + z
        if new_y < dis[y]:
            dis[y] = new_y
            heappush(h, (new_y, y))
```

## bisect

```python
from bisect import bisect_left, bisect_right

i = bisect_left(a, x)
j = bisect_right(a, x)
```

对应 C++：

```cpp
lower_bound(a.begin(), a.end(), x) - a.begin();
upper_bound(a.begin(), a.end(), x) - a.begin();
```

语义：

- `bisect_left(a, x)`：第一个 `>= x` 的位置
- `bisect_right(a, x)`：第一个 `> x` 的位置

计数：

```python
cnt = bisect_right(a, r) - bisect_left(a, l)
```

表示排序数组中 `[l, r]` 的元素个数。

插入：

```python
from bisect import insort
insort(a, x)
```

注意：

- 查找 O(log n)
- 插入 O(n)
- 所以 `bisect + list` 不能替代 C++ `set` 的频繁插删。

## sortedcontainers

需要：

```python
from sortedcontainers import SortedSet, SortedDict, SortedList
```

### SortedList

类似 C++ `multiset`。

```python
sl = SortedList()
sl.add(x)
sl.remove(x)
sl.discard(x)
sl[0]
sl[-1]
i = sl.bisect_left(x)
j = sl.bisect_right(x)
```

区别：

- 支持重复元素。
- 下标访问方便。

### SortedSet

类似 C++ `set`。

```python
s = SortedSet()
s.add(x)
s.discard(x)
s[0]
s[-1]
i = s.bisect_left(x)
```

区别：

- 不支持重复元素。

### SortedDict

类似 C++ `map`。

```python
sd = SortedDict()
sd[x] = y
del sd[x]
keys = sd.keys()
items = sd.items()
```

找前驱后继：

```python
i = sd.bisect_left(x)
if i > 0:
    pre_key = sd.keys()[i - 1]
if i < len(sd):
    nxt_key = sd.keys()[i]
```

注意：

- `sortedcontainers` 常数比 C++ `set/map` 大。
- 不一定所有 OJ 都安装。

## math

```python
import math

math.gcd(a, b)
math.lcm(a, b)
math.isqrt(x)
math.ceil(x)
math.floor(x)
math.comb(n, k)
math.perm(n, k)
math.factorial(n)
```

对应 C++：

```cpp
std::gcd(a, b)
std::lcm(a, b)
sqrt(x)
```

常用：

```python
r = math.isqrt(x)
if r * r == x:
    ...
```

`math.isqrt` 是整数平方根，不会有浮点误差。

## itertools

虽然 `1.py` 没写，但竞赛很常用。

```python
from itertools import accumulate, permutations, combinations, product
```

前缀和：

```python
pre = [0] + list(accumulate(a))
sum_lr = pre[r + 1] - pre[l]
```

对应 C++：

```cpp
partial_sum(a.begin(), a.end(), pre.begin() + 1);
```

排列组合：

```python
for p in permutations(a):
    ...

for c in combinations(a, k):
    ...
```

笛卡尔积：

```python
for x, y in product(range(n), range(m)):
    ...
```

注意：

- 这些函数返回迭代器。
- 组合/排列数量爆炸，慎用。

## min/max/sum/all/any

```python
min(a)
max(a)
sum(a)
all(cond for x in a)
any(cond for x in a)
```

带 key：

```python
p = min(points, key=lambda x: x[0] + x[1])
```

对应 C++：

```cpp
*min_element(a.begin(), a.end())
accumulate(a.begin(), a.end(), 0LL)
```

注意：

- `sum` 初始值默认是 `0`，如果累加大整数没问题。
- 拼接字符串不要用 `sum`，用 `"".join(...)`。

## enumerate / zip

```python
for i, x in enumerate(a):
    ...
```

对应 C++：

```cpp
for (int i = 0; i < n; i++) {
    int x = a[i];
}
```

```python
for x, y in zip(a, b):
    ...
```

类似同时遍历两个数组。

注意：

- `zip` 长度取较短的那个。

## reversed

```python
for x in reversed(a):
    ...
```

对应 C++：

```cpp
for (auto it = a.rbegin(); it != a.rend(); ++it)
```

如果要得到反转列表：

```python
b = a[::-1]
b = list(reversed(a))
```

## copy

```python
b = a[:]       # 浅拷贝
b = a.copy()   # 浅拷贝
```

二维数组深拷贝：

```python
b = [row[:] for row in a]
```

注意：

- `b = a` 不是拷贝，是引用同一个列表。

## 函数和类

### 普通函数模板

```python
def qpow(x: int, y: int, mod: int = MOD) -> int:
    z = 1
    while y:
        if y & 1:
            z = z * x % mod
        x = x * x % mod
        y >>= 1
    return z
```

### LeetCode class Solution

LeetCode 要写：

```python
class Solution:
    def count(self, num1: str, num2: str, min_sum: int, max_sum: int) -> int:
        ...
```

注意复制模板时：

- 顶层模板粘进 `class Solution` 后，整个函数体要多缩进 4 个空格。
- 如果模板本身是 `def count(...)`，复制到 `class Solution` 里时，需要把 `def` 和函数体整体右移一级。
- 最稳是单独准备 LeetCode 版模板。

## 类型提示 typing

```python
from typing import List, Tuple, Set

def f(a: List[int]) -> Tuple[int, int]:
    ...
```

对应 C++：

```cpp
pair<int,int> f(vector<int>& a)
```

说明：

- 类型提示不影响运行。
- LeetCode 函数签名常用。
- 普通 OJ 可以不写。

Python 3.9+ 可以写：

```python
list[int]
tuple[int, int]
set[int]
```

但为了兼容 LeetCode 旧环境，`List[int]` 仍然常见。

## 常用 C++ 到 Python 对照表

| C++ | Python |
| --- | --- |
| `vector<int> a` | `a = []` 或 `a = [0] * n` |
| `a.push_back(x)` | `a.append(x)` |
| `a.pop_back()` | `a.pop()` |
| `a.back()` | `a[-1]` |
| `a.size()` | `len(a)` |
| `sort(a.begin(), a.end())` | `a.sort()` |
| `reverse(a.begin(), a.end())` | `a.reverse()` |
| `queue<int>` | `deque` |
| `priority_queue` 小根堆 | `heapq` |
| `map/unordered_map` | `dict/defaultdict/Counter` |
| `set/unordered_set` | `set` |
| `multiset` | `SortedList` 或 `Counter` |
| `lower_bound` | `bisect_left` |
| `upper_bound` | `bisect_right` |
| `pair<int,int>` | `tuple`，如 `(x, y)` |
| `auto [x, y] = p` | `x, y = p` |
| `__builtin_popcount(x)` | `x.bit_count()` |
| `__lg(x) + 1` | `x.bit_length()` |
| `long long` | `int` |
| `__int128` | `int` |
| `const int INF = ...` | `inf` 或 `10**30` |
| `lambda [&]` | 嵌套函数 + `nonlocal` |
| 手写 memo 数组/map | `@cache` |

## 常见坑

### 缩进

Python 缩进就是语法。复制模板时尤其注意：

```python
class Solution:
    def f(self):
        @cache
        def dfs(x):
            ...
        return dfs(0)
```

`@cache`、`def dfs`、`return dfs` 都属于外层函数 `f`，必须比 `def f` 多 4 个空格。

### 可变默认参数

不要写：

```python
def f(a=[]):
    ...
```

应该写：

```python
def f(a=None):
    if a is None:
        a = []
```

### 递归深度

Python 默认递归深度不高。

```python
import sys
sys.setrecursionlimit(10**6)
```

树 DFS 深度大时要加。

但注意：

- 有些 OJ Python 递归太深仍可能栈炸。
- 可以改成迭代 DFS。

### 浅拷贝

```python
dp = [[0] * m] * n  # 错
```

正确：

```python
dp = [[0] * m for _ in range(n)]
```

### 堆是小根堆

C++ 默认 `priority_queue` 是大根堆，Python `heapq` 是小根堆。

### 整除和取模

Python 负数除法/取模和 C++ 不同。涉及负数时要格外小心。

### 字典遍历时不要修改大小

不要：

```python
for k in ma:
    del ma[k]
```

可以：

```python
for k in list(ma):
    del ma[k]
```

### list 删除中间元素是 O(n)

```python
a.pop(i)
a.insert(i, x)
```

都是 O(n)。频繁中间插删要换数据结构。

### `is` 和 `==`

```python
x is None
x == 0
```

`is` 判断是不是同一个对象，`==` 判断值是否相等。

### `and/or` 返回的不一定是 bool

```python
x = a or b
```

如果 `a` 真，返回 `a`；否则返回 `b`。

竞赛里能用，但别和 C++ 的 `||` 完全等同。

## 常用模板片段

### 建图

```python
ma = [[] for _ in range(n)]
for x, y in edges:
    ma[x].append(y)
    ma[y].append(x)
```

带权：

```python
ma = [[] for _ in range(n)]
for x, y, z in edges:
    ma[x].append((y, z))
    ma[y].append((x, z))
```

### BFS

```python
q = deque([0])
vis = [False] * n
vis[0] = True
while q:
    x = q.popleft()
    for y in ma[x]:
        if vis[y]:
            continue
        vis[y] = True
        q.append(y)
```

### DFS

```python
def dfs(x: int, fa: int) -> None:
    for y in ma[x]:
        if y == fa:
            continue
        dfs(y, x)
```

### 记忆化 DFS

```python
@cache
def dfs(i: int, j: int) -> int:
    if i == n:
        return 0
    res = dfs(i + 1, j)
    if j:
        res = max(res, dfs(i + 1, j - 1) + a[i])
    return res
```

### 前缀和

```python
pre = [0] * (n + 1)
for i, x in enumerate(a):
    pre[i + 1] = pre[i] + x

def query(l: int, r: int) -> int:
    return pre[r + 1] - pre[l]
```

### 差分

```python
diff = [0] * (n + 1)
diff[l] += x
diff[r + 1] -= x

cur = 0
for i in range(n):
    cur += diff[i]
```

### 离散化

```python
b = sorted(set(a))
idx = {x: i for i, x in enumerate(b)}
rank = [idx[x] for x in a]
```

对应 C++：

```cpp
sort(b.begin(), b.end());
b.erase(unique(b.begin(), b.end()), b.end());
lower_bound(b.begin(), b.end(), x) - b.begin();
```

### 二分答案

```python
def check(x: int) -> bool:
    ...

l, r = 0, 10**18
while l < r:
    mid = (l + r) // 2
    if check(mid):
        r = mid
    else:
        l = mid + 1
```

找最大可行：

```python
l, r = 0, 10**18
while l < r:
    mid = (l + r + 1) // 2
    if check(mid):
        l = mid
    else:
        r = mid - 1
```

### 子集枚举

```python
sub = s
while sub:
    ...
    sub = (sub - 1) & s
```

包含空集：

```python
sub = s
while True:
    ...
    if sub == 0:
        break
    sub = (sub - 1) & s
```

## 性能建议

- 多用局部变量，少在热循环里反复访问全局变量。
- `for x in a` 通常比 `for i in range(len(a))` 更 Python。
- 需要下标时用 `enumerate`。
- 队列用 `deque`，不要 `list.pop(0)`。
- 堆用 `heapq`。
- 大量输入用 `sys.stdin.buffer.read()`。
- 递归深时考虑改迭代。
- Python 常数大，能 O(n log n) 不写 O(n sqrt n)，能预处理就预处理。

