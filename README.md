## 打算法竞赛用的板子

作者还是菜鸡，因此没有收录太多板子，还在学习中，一般使用C++和Python打比赛（后者正在学习，板子不全），当前目录如下：

### Python 语法
- ``Python算法竞赛语法清单.md``：Python 算法竞赛常用语法糖、库函数，以及和 C++ 的对应区别。

### 动态规划
- ``Bitmask.md``：状压DP常见题型，整理中
- ``Digit.cpp``：数位DP
- ``Tree.cpp``：树形DP喵喵题

### 图论
- ``Kruscal.cpp``：Kruscal算法，求MST。
- ``Dijsktra.cpp``：Dijkstra算法，单源最短路。
- ``Floyd.cpp``：Floyd算法，全源最短路。
- ``toposort.cpp``：拓扑排序

### 网络流

- [`NetworkFlow.md`](C++/网络流/NetworkFlow.md)：选板子、常用建图、上下界流与方案恢复。
- [`Dinic.cpp`](C++/网络流/Dinic.cpp)：最大流、限制流量与最小割源点侧。
- [`HopcroftKarp.cpp`](C++/网络流/HopcroftKarp.cpp)：二分图最大匹配与最小点覆盖。
- [`LowerFlow.cpp`](C++/网络流/LowerFlow.cpp)：上下界可行流、最大流与最小非负流。
- [`MCMF.cpp`](C++/网络流/MCMF.cpp)：势能与 Dijkstra 实现的最小费用流。

### 树论
- ``LCA.cpp``：倍增法求最近公共祖先
- ``Diameter.cpp``：树的直径

### 数学
- ``Qpow.cpp``：快速幂
- ``Mulpow.cpp``：矩阵快速幂
- ``Sieve_E.cpp``：质数的埃氏筛法
- ``Sieve_Eu.cpp``：质数的欧拉筛法
- ``Bitmask.cpp``：常见位运算操作
- ``Prime_factorization.cpp``：质因数分解
- ``Divisors.cpp``：因数分解
- ``NumberTheoryBlock.cpp``：数论分块/整除分块
- ``Combinatorics.cpp``：组合数学预处理
- ``FFT.cpp``：快速傅里叶变换，求整数多项式卷积
- ``NTT.cpp``：快速数论变换，求多项式卷积
- ``Xorbasis.cpp``：线性异或基
- ``Phi.cpp``：预处理欧拉函数

### 数据结构
- ``Block.cpp``：分块
- ``Monotone Stack``：单调栈
- ``UnionFind.cpp``：并查集（带大小）
- ``UnionFind_With_Weights.cpp``：带权并查集
- ``RollbackUnionFind.cpp``：回滚并查集，支持撤销合并
- ``PersistentUnionFind.cpp``：可持久化并查集，支持从任意历史版本合并、连通性和集合大小查询。
- ``Trie.cpp``：Trie/字典树，前缀匹配
- ``PersistentTrie.cpp``：可持久化 01-Trie，支持从任意历史版本增删，并查询异或极值、计数和第 k 小。
- ``Treearray.cpp``：树状数组，单点修改区间查询，离散化。
- ``SegmentTree.cpp``：线段树与动态开点线段树，单点修改区间查询，线段树二分。
- ``LazySeg.cpp``：懒标记线段树与动态开点懒标记线段树，区间修改区间查询，线段树二分。
- ``PersistentSegmentTree.cpp``：可持久化线段树，支持从任意历史版本单点修改、区间查询和线段树二分。
- ``PersistentDynamicSegmentTree.cpp``：可持久化动态开点线段树，支持在大值域上从任意历史版本分叉、单点赋值、区间查询和线段树二分。
- ``ChairmanTree.cpp``：可持久化权值线段树/主席树，单点修改，区间查询第k小
- ``ChthollyTree.cpp``：珂朵莉树，支持区间推平、区间修改、区间查询和区间第k小。

### 字符串
- ``kmp.cpp``：KMP算法，求border。
- ``zfunc.cpp``：Zfunc/拓展kmp算法，求LCP。
- ``Hashing.cpp``：字符串哈希。
- ``Manacher.cpp``：Manacher算法，线性求所有奇偶回文半径。
- ``SA.cpp``：后缀数组，构造 sa、rk、height，支持 LCP、子串比较和常见统计。

### 计算几何

- ``Point.cpp``：点和向量、点积、叉积、距离与旋转。
- ``Line.cpp``：直线交点、线段相交、投影、对称与距离。
- ``Polygon.cpp``：多边形面积、点包含、重心、凸包与旋转卡壳。
- ``Circle.cpp``：圆的交点、切点、外接圆与两圆相交面积。
