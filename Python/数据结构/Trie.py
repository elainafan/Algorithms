# C++ 中三段都叫 solve()，每段独立使用；Python 按用途加后缀，避免同名覆盖。


# 小写字母 Trie
def solve() -> None:
    son = [[0] * 26]
    cnt = [0]
    endcnt = [0]

    # 插入字符串 s
    def insert(s: str) -> None:
        cur = 0
        cnt[cur] += 1
        for ch in s:
            c = ord(ch) - ord("a")
            if not son[cur][c]:
                son[cur][c] = len(son)
                son.append([0] * 26)
                cnt.append(0)
                endcnt.append(0)
            cur = son[cur][c]
            cnt[cur] += 1
        endcnt[cur] += 1

    # 删除一个已存在的字符串 s
    def erase(s: str) -> None:
        cur = 0
        cnt[cur] -= 1
        for ch in s:
            cur = son[cur][ord(ch) - ord("a")]
            cnt[cur] -= 1
        endcnt[cur] -= 1

    # 返回 s 对应的节点，不存在则返回 -1
    def find(s: str) -> int:
        cur = 0
        for ch in s:
            c = ord(ch) - ord("a")
            if not son[cur][c]:
                return -1
            cur = son[cur][c]
        return cur

    # 查询字符串 s 的出现次数
    def cntword(s: str) -> int:
        cur = find(s)
        return 0 if cur == -1 else endcnt[cur]

    # 查询以 s 为前缀的字符串数量
    def cntprefix(s: str) -> int:
        cur = find(s)
        return 0 if cur == -1 else cnt[cur]

    # 返回 s 的最短已插入前缀，没有则返回空串
    def findpre(s: str) -> str:
        cur = 0
        for i, ch in enumerate(s):
            c = ord(ch) - ord("a")
            if not son[cur][c]:
                return ""
            cur = son[cur][c]
            if endcnt[cur]:
                return s[: i + 1]
        return ""

    # insert(s), erase(s), cntword(s), cntprefix(s), findpre(s)


# 非字母 Trie：维护 list[int]
def solve_sequence() -> None:
    son = [{}]
    cnt = [0]
    endcnt = [0]

    # 插入序列 a
    def insert(a: list[int]) -> None:
        cur = 0
        cnt[cur] += 1
        for x in a:
            if x not in son[cur]:
                son[cur][x] = len(son)
                son.append({})
                cnt.append(0)
                endcnt.append(0)
            cur = son[cur][x]
            cnt[cur] += 1
        endcnt[cur] += 1

    # 删除一个已存在的序列 a
    def erase(a: list[int]) -> None:
        cur = 0
        cnt[cur] -= 1
        for x in a:
            cur = son[cur][x]
            cnt[cur] -= 1
        endcnt[cur] -= 1

    # 返回 a 对应的节点，不存在则返回 -1
    def find(a: list[int]) -> int:
        cur = 0
        for x in a:
            if x not in son[cur]:
                return -1
            cur = son[cur][x]
        return cur

    # 查询序列 a 的出现次数
    def cntword(a: list[int]) -> int:
        cur = find(a)
        return 0 if cur == -1 else endcnt[cur]

    # 查询以 a 为前缀的序列数量
    def cntprefix(a: list[int]) -> int:
        cur = find(a)
        return 0 if cur == -1 else cnt[cur]

    # 返回 a 的最短已插入前缀，没有则返回空序列
    def findpre(a: list[int]) -> list[int]:
        cur = 0
        for i, x in enumerate(a):
            if x not in son[cur]:
                return []
            cur = son[cur][x]
            if endcnt[cur]:
                return a[: i + 1]
        return []

    # insert(a), erase(a), cntword(a), cntprefix(a), findpre(a)


# 典题：同时限制前缀和后缀，求最大的单词下标（LeetCode 745）
# 插入所有 suffix + "{" + s，建树 O(sum |s|^2)，查询 O(|pre|+|suf|)
def solve_prefix_suffix() -> None:
    n = int(input())
    son = [[0] * 27]
    idx = [-1]

    # 插入 s 的所有后缀组合并维护最大下标
    def insert(s: str, id: int) -> None:
        t = s + "{" + s
        for i in range(len(s) + 1):
            cur = 0
            for ch in t[i:]:
                c = 26 if ch == "{" else ord(ch) - ord("a")
                if not son[cur][c]:
                    son[cur][c] = len(son)
                    son.append([0] * 27)
                    idx.append(-1)
                cur = son[cur][c]
                idx[cur] = id

    # 查询同时满足前缀 pre 和后缀 suf 的最大下标
    def query(pre: str, suf: str) -> int:
        cur = 0
        for ch in suf + "{" + pre:
            c = 26 if ch == "{" else ord(ch) - ord("a")
            if not son[cur][c]:
                return -1
            cur = son[cur][c]
        return idx[cur]

    for i in range(n):
        insert(input().strip(), i)

    q = int(input())
    for _ in range(q):
        pre, suf = input().split()
        print(query(pre, suf))
