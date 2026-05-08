from collections import defaultdict
from typing import Dict, Hashable, Iterable, List


class TrieNode:
    __slots__ = ("son", "pre", "endcnt")

    def __init__(self):
        self.son: Dict[Hashable, "TrieNode"] = {}
        self.pre = 0
        self.endcnt = 0


class Trie:
    # 析构函数

    def __init__(self):
        self.root = TrieNode()

    def insert(self, word: Iterable[Hashable]) -> None:
        # 往trie中插入word
        cur = self.root
        cur.pre += 1
        for c in word:
            if c not in cur.son:
                cur.son[c] = TrieNode()
            cur = cur.son[c]
            cur.pre += 1
        cur.endcnt += 1

    def _find(self, word: Iterable[Hashable]) -> TrieNode | None:
        cur = self.root
        for c in word:
            if c not in cur.son:
                return None
            cur = cur.son[c]
        return cur

    def search(self, word: Iterable[Hashable]) -> bool:
        # 查询是否存在与word相等的字符串
        node = self._find(word)
        return node is not None and node.endcnt > 0

    def starts_with(self, prefix: Iterable[Hashable]) -> bool:
        # 查询是否存在以prefix为前缀的字符串
        return self._find(prefix) is not None

    def cntword(self, word: Iterable[Hashable]) -> int:
        # 查询插入了与word相同的数量
        node = self._find(word)
        return 0 if node is None else node.endcnt

    def cntprefix(self, prefix: Iterable[Hashable]) -> int:
        # 查询插入了以prefix为前缀的数量
        node = self._find(prefix)
        return 0 if node is None else node.pre

    def first_prefix(self, word: List[Hashable] | str):
        # 查询trie中是否有word的前缀，若有返回，若无返回空串
        cur = self.root
        for i, c in enumerate(word):
            if c not in cur.son:
                return type(word)()
            cur = cur.son[c]
            if cur.endcnt > 0:
                return word[: i + 1]
        return type(word)()

    def erase(self, word: Iterable[Hashable]) -> None:
        # 往trie中删除word
        cur = self.root
        cur.pre -= 1
        for c in word:
            cur = cur.son[c]
            cur.pre -= 1
        cur.endcnt -= 1


class PrefixSuffixFilter:
    # 下面是非字母版的
    # Trie变式，同时检索前缀和后缀

    UND = "#"

    def __init__(self):
        self.root = defaultdict(dict)
        self.idx = {}

    def insert(self, word: str, idx: int) -> None:
        n = len(word)
        for i in range(n):
            cur = self.root
            for j in range(i, n):
                key = (word[j], self.UND)
                cur = cur.setdefault(key, {})
                self.idx[id(cur)] = max(self.idx.get(id(cur), -1), idx)

            cur = self.root
            for j in range(i, n):
                key = (self.UND, word[n - 1 - j])
                cur = cur.setdefault(key, {})
                self.idx[id(cur)] = max(self.idx.get(id(cur), -1), idx)

            cur = self.root
            for j in range(i, n):
                key = (word[j], word[n - 1 - j])
                cur = cur.setdefault(key, {})
                self.idx[id(cur)] = max(self.idx.get(id(cur), -1), idx)

    def find(self, pre: str, suf: str) -> int:
        cur = self.root
        m = max(len(pre), len(suf))
        for i in range(m):
            c1 = pre[i] if i < len(pre) else self.UND
            c2 = suf[len(suf) - 1 - i] if i < len(suf) else self.UND
            key = (c1, c2)
            if key not in cur:
                return -1
            cur = cur[key]
        return self.idx.get(id(cur), -1)
