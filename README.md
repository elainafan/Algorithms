# 算法竞赛模板

自用的算法竞赛板子，以 C++ 为主，也整理了一部分 Python 实现和题型示例。

[动态规划](#动态规划) · [图论](#图论) · [网络流](#网络流) · [树论](#树论) · [数据结构](#数据结构) · [数学](#数学) · [字符串](#字符串) · [计算几何](#计算几何) · [杂项](#杂项)

常用入口：[C++ 目录](C++) · [Python 目录](Python) · [Python 比赛头](Python/Base.py) · [Python 算法竞赛语法清单](Python算法竞赛语法清单.md)

## 使用说明

- 按需复制对应的代码片段。同一文件可能放了多个独立版本，选择需要的一段使用。
- C++ 模板沿用个人比赛头中的宏和别名，例如 `ll`、`rep`、`frep`、`all`、`sz`、`vi`、`vl`；源码中提到的 `1.cpp` 不在本仓库，其他依赖按文件说明补齐。部分模板使用 C++23 的 `this auto&&` 递归 lambda。
- 下标、区间端点和返回值约定以各文件为准。C++ 与 Python 同主题的实现不一定具有相同接口或功能。
- 下表中的 `—` 表示未收录对应语言的文件；待补充的内容单独标注。

## 动态规划

| 内容 | C++ | Python |
| --- | --- | --- |
| 数位 DP：上下界约束下的数位和计数 | [Digit.cpp](C++/动态规划/Digit.cpp) | [Digit.py](Python/动态规划/Digit.py) |
| 树形 DP：换根与最远点示例 | [Tree.cpp](C++/动态规划/Tree.cpp) | [Tree.py](Python/动态规划/Tree.py) |
| SOS DP：子集计数 | [SOS.cpp](C++/动态规划/SOS.cpp) | — |
| 状压 DP 笔记：待补充，当前文件为空 | [Bitmask.md](C++/动态规划/Bitmask.md) | — |

## 图论

| 内容 | C++ | Python |
| --- | --- | --- |
| Kruskal 最小生成树 | [Kruscal.cpp](C++/图论/Kruscal.cpp) | [Kruscal.py](Python/图论/Kruscal.py) |
| Dijkstra 单源最短路 | [dijkstra.cpp](C++/图论/dijkstra.cpp) | [dijkstra.py](Python/图论/dijkstra.py) |
| Floyd 全源最短路 | [floyd.cpp](C++/图论/floyd.cpp) | [floyd.py](Python/图论/floyd.py) |
| 拓扑排序 | [toposort.cpp](C++/图论/toposort.cpp) | [toposort.py](Python/图论/toposort.py) |
| 欧拉路径与欧拉回路 | [euler.cpp](C++/图论/euler.cpp) | [euler.py](Python/图论/euler.py) |
| 强连通分量 | [SCC.cpp](C++/图论/SCC.cpp) | — |
| 边双连通分量与桥 | [EBCC.cpp](C++/图论/EBCC.cpp) | — |
| 点双连通分量与割点 | [VBCC.cpp](C++/图论/VBCC.cpp) | — |
| BFS 示例：二分边权阈值与最少边数 | — | [bfs.py](Python/图论/bfs.py) |
| DFS 示例：连通块计数 | — | [dfs.py](Python/图论/dfs.py) |

## 网络流

建模与接口说明见 [NetworkFlow.md](C++/网络流/NetworkFlow.md)，包含选板子、常用建图、上下界流与方案恢复。

| 内容 | C++ |
| --- | --- |
| 最大流、限制流量与最小割源点侧 | [Dinic.cpp](C++/网络流/Dinic.cpp) |
| 二分图最大匹配与最小点覆盖 | [HopcroftKarp.cpp](C++/网络流/HopcroftKarp.cpp) |
| 上下界可行流、最大流与最小非负流；依赖 Dinic | [LowerFlow.cpp](C++/网络流/LowerFlow.cpp) |
| 势能与 Dijkstra 实现的最小费用流 | [MCMF.cpp](C++/网络流/MCMF.cpp) |

## 树论

| 内容 | C++ | Python |
| --- | --- | --- |
| 倍增祖先与 LCA | [LCA.cpp](C++/树论/LCA.cpp) | [LCA.py](Python/树论/LCA.py) |
| 无权树直径与端点 | [Diameter.cpp](C++/树论/Diameter.cpp) | [Diameter.py](Python/树论/Diameter.py) |
| 无权树中心 | [TreeCenter.cpp](C++/树论/TreeCenter.cpp) | — |
| 欧拉序：将子树映射为连续区间 | [EulerTour.cpp](C++/树论/EulerTour.cpp) | — |
| 重链剖分：LCA、路径区间与子树区间 | [HLD.cpp](C++/树论/HLD.cpp) | — |
| 虚树：按关键点建树，依赖 HLD | [VirtualTree.cpp](C++/树论/VirtualTree.cpp) | — |
| 点分治：统计距离为 k 的无序点对 | [CentroidDecomposition.cpp](C++/树论/CentroidDecomposition.cpp) | — |
| 点分树：动态标记、取消标记与最近标记点查询 | [CentroidTree.cpp](C++/树论/CentroidTree.cpp) | — |
| Kruskal 重构树：升序 / 降序建树、瓶颈与阈值连通块查询 | [KruskalTree.cpp](C++/树论/KruskalTree.cpp) | — |
| LCT：动态森林连边、断边、连通性、单点赋值与路径点权和 | [LCT.cpp](C++/树论/LCT.cpp) | — |

## 数据结构

### 区间查询与线段树

| 内容 | C++ | Python |
| --- | --- | --- |
| ST 表：静态区间最值，可修改合并函数维护 GCD、按位与 / 或 | [STTable.cpp](C++/数据结构/STTable.cpp) | — |
| 树状数组：单点加、区间和、前缀二分与前缀最大值 | [Treearray.cpp](C++/数据结构/Treearray.cpp) | [Treearray.py](Python/数据结构/Treearray.py) |
| 二维树状数组：单点加、矩形和与 O(nm) 建树 | [Treearray2D.cpp](C++/数据结构/Treearray2D.cpp) | — |
| 线段树：单点修改、区间查询与二分；C++ 另含动态开点、整树 / 区间合并及优化建图 | [SegmentTree.cpp](C++/数据结构/SegmentTree.cpp) | [SegmentTree.py](Python/数据结构/SegmentTree.py) |
| 懒标记线段树：区间修改、查询与二分；C++ 另含动态开点版本 | [LazySeg.cpp](C++/数据结构/LazySeg.cpp) | [LazySeg.py](Python/数据结构/LazySeg.py) |
| 线段树 Beats：区间加、chmin、chmax 与和 / 最值查询 | [SegmentTreeBeats.cpp](C++/数据结构/SegmentTreeBeats.cpp) | — |
| 李超线段树：直线 / 线段插入，查询最优值及编号 | [LiChaoTree.cpp](C++/数据结构/LiChaoTree.cpp) | — |
| 可持久化线段树：历史版本分叉、单点赋值、区间查询与二分 | [PersistentSegmentTree.cpp](C++/数据结构/PersistentSegmentTree.cpp) | — |
| 可持久化动态开点线段树：大值域上的历史版本查询与修改 | [PersistentDynamicSegmentTree.cpp](C++/数据结构/PersistentDynamicSegmentTree.cpp) | — |
| 主席树：可持久化权值线段树，区间第 k 小与计数 | [ChairmanTree.cpp](C++/数据结构/ChairmanTree.cpp) | [ChairmanTree.py](Python/数据结构/ChairmanTree.py) |
| 珂朵莉树：区间赋值、修改、查询与第 k 小 | [ChthollyTree.cpp](C++/数据结构/ChthollyTree.cpp) | — |

### 并查集与字典树

| 内容 | C++ | Python |
| --- | --- | --- |
| 并查集：连通性与集合大小 | [UnionFind.cpp](C++/数据结构/UnionFind.cpp) | [UnionFind.py](Python/数据结构/UnionFind.py) |
| 带权并查集 | [UnionFind_With_Weights.cpp](C++/数据结构/UnionFind_With_Weights.cpp) | [UnionFind_With_Weights.py](Python/数据结构/UnionFind_With_Weights.py) |
| 回滚并查集：撤销合并与历史快照 | [RollbackUnionFind.cpp](C++/数据结构/RollbackUnionFind.cpp) | — |
| 可持久化并查集：从任意历史版本合并与查询 | [PersistentUnionFind.cpp](C++/数据结构/PersistentUnionFind.cpp) | — |
| Trie：字符串 / 序列计数、前缀与前后缀匹配 | [Trie.cpp](C++/数据结构/Trie.cpp) | [Trie.py](Python/数据结构/Trie.py) |
| 精简 Trie：字符串 / 序列计数与最大异或 | [TrieLite.cpp](C++/数据结构/TrieLite.cpp) | — |
| 01-Trie：整数增删、计数与异或查询 | [01-Trie.cpp](C++/数据结构/01-Trie.cpp) | — |
| 可持久化 01-Trie：历史版本增删、异或极值、计数与第 k 小 | [PersistentTrie.cpp](C++/数据结构/PersistentTrie.cpp) | — |

### 平衡树与笛卡尔树

| 内容 | C++ | Python |
| --- | --- | --- |
| FHQ Treap：有序可重集合、排名、第 k 小与前驱后继 | [Treap.cpp](C++/数据结构/Treap.cpp) | — |
| 隐式 FHQ Treap：序列插入、删除、翻转、区间修改与查询 | [ImplicitTreap.cpp](C++/数据结构/ImplicitTreap.cpp) | — |
| Splay：序列维护、区间翻转、加法、赋值与查询 | [Splay.cpp](C++/数据结构/Splay.cpp) | — |
| 笛卡尔树：默认大根堆 | [CartesianTree.cpp](C++/数据结构/CartesianTree.cpp) | [CartesianTree.py](Python/数据结构/CartesianTree.py) |

### 分块、莫队与单调栈

| 内容 | C++ | Python |
| --- | --- | --- |
| 分块：区间加与区间和 | [Block.cpp](C++/数据结构/Block.cpp) | [Block.py](Python/数据结构/Block.py) |
| 普通莫队：静态区间不同数个数 | [Mo.cpp](C++/数据结构/Mo.cpp) | — |
| 回滚莫队：静态区间最高出现次数 | [RollbackMo.cpp](C++/数据结构/RollbackMo.cpp) | — |
| 单调栈：左右更小元素与柱状图最大矩形 | [Monotone stack.cpp](C++/数据结构/Monotone%20stack.cpp) | [Monotone stack.py](Python/数据结构/Monotone%20stack.py)、[Monostack.py](Python/数据结构/Monostack.py) |

## 数学

| 内容 | C++ | Python |
| --- | --- | --- |
| 快速幂与模运算 | [Qpow.cpp](C++/数学/Qpow.cpp) | [Qpow.py](Python/数学/Qpow.py) |
| 矩阵乘法与快速幂 | [Mulpow.cpp](C++/数学/Mulpow.cpp) | [Mulpow.py](Python/数学/Mulpow.py) |
| 埃氏筛 | [Sieve_E.cpp](C++/数学/Sieve_E.cpp) | [Sieve_E.py](Python/数学/Sieve_E.py) |
| 欧拉筛 | [Sieve_Eu.cpp](C++/数学/Sieve_Eu.cpp) | [Sieve_Eu.py](Python/数学/Sieve_Eu.py) |
| 最小素因子预处理与质因数分解 | [Prime_factorization.cpp](C++/数学/Prime_factorization.cpp) | [Prime_factorization.py](Python/数学/Prime_factorization.py) |
| 正因数列表预处理 | [Divisors.cpp](C++/数学/Divisors.cpp) | [Divisors.py](Python/数学/Divisors.py) |
| 欧拉函数 | [Phi.cpp](C++/数学/Phi.cpp) | [Phi.py](Python/数学/Phi.py) |
| 阶乘、逆阶乘与组合数 | [Combinatorics.cpp](C++/数学/Combinatorics.cpp) | [Combinatorics.py](Python/数学/Combinatorics.py) |
| CRT / exCRT 与线性不定方程 | [CRT.cpp](C++/数学/CRT.cpp) | — |
| 数论分块 / 整除分块 | [NumberTheoryBlock.cpp](C++/数学/NumberTheoryBlock.cpp) | — |
| 位运算与集合操作 | [Bitmask.cpp](C++/数学/Bitmask.cpp) | [Bitmask.py](Python/数学/Bitmask.py) |
| 异或线性基；C++ 另含前缀线性基与区间查询 | [Xorbasis.cpp](C++/数学/Xorbasis.cpp) | [Xorbasis.py](Python/数学/Xorbasis.py) |
| 随机异或哈希：按各位置所属的区间状态集合分组 | [RandomXorHash.cpp](C++/数学/RandomXorHash.cpp) | — |
| FFT：整数系数卷积 | [FFT.cpp](C++/数学/FFT.cpp) | — |
| NTT：模 998244353 卷积 | [NTT.cpp](C++/数学/NTT.cpp) | — |

## 字符串

| 内容 | C++ | Python |
| --- | --- | --- |
| KMP：前缀函数与字符串匹配 | [kmp.cpp](C++/字符串/kmp.cpp) | [kmp.py](Python/字符串/kmp.py) |
| Z 函数：与整串前缀的最长公共前缀 | [zfunc.cpp](C++/字符串/zfunc.cpp) | [zfunc.py](Python/字符串/zfunc.py) |
| 字符串哈希与自定义哈希 | [Hashing.cpp](C++/字符串/Hashing.cpp) | [Hashing.py](Python/字符串/Hashing.py) |
| Manacher：奇偶回文半径 | [Manacher.cpp](C++/字符串/Manacher.cpp) | — |
| 后缀数组：LCP、子串比较、匹配与统计 | [SA.cpp](C++/字符串/SA.cpp) | — |
| AC 自动机与 Fail 树：多模式匹配与子树查询 | [ACAM.cpp](C++/字符串/ACAM.cpp) | — |

## 计算几何

每个文件均包含浮点版本和 `i128` 整数坐标版本；交点、投影、距离等实数结果仍使用浮点类型。先复制 `Point.cpp`，`Polygon.cpp` 和 `Circle.cpp` 还依赖 `Line.cpp`。

| 内容 | C++ |
| --- | --- |
| 点和向量：点积、叉积、距离与旋转 | [Point.cpp](C++/计算几何/Point.cpp) |
| 直线与线段：交点、相交判断、投影、对称与距离 | [Line.cpp](C++/计算几何/Line.cpp) |
| 多边形：面积、点包含、重心、凸包与旋转卡壳直径 | [Polygon.cpp](C++/计算几何/Polygon.cpp) |
| 圆：交点、切点、外接圆与相交面积 | [Circle.cpp](C++/计算几何/Circle.cpp) |

## 杂项

| 内容 | C++ | Python |
| --- | --- | --- |
| 比赛头与输入输出 | — | [Base.py](Python/Base.py) |
| 大整数输入输出：C++ 的 i128 与 Python 的 int | [i128.cpp](C++/杂项/i128.cpp) | [i128.py](Python/杂项/i128.py) |
| 随机数据生成器 | [Gen.cpp](C++/杂项/Gen.cpp) | — |
| 对拍器：生成输入、运行候选解与暴力解、比较输出 | [Stress.cpp](C++/杂项/Stress.cpp) | — |
