struct Node {
    Node* son[26]{};
    int pre = 0;
    int endcnt = 0;
};
class Trie {
    Node* root = new Node();

    int find(string word) {
        Node* cur = root;
        for (char c : word) {
            c -= 'a';
            if (!cur->son[c]) return 0;
            cur = cur->son[c];
        }
        return cur->endcnt > 0 ? 2 : 1;
    }
    string findpre(string word) {
        Node* cur = root;
        for (int i = 0; i < word.size(); i++) {
            char c = word[i] - 'a';
            if (!cur->son[c]) return "";
            cur = cur->son[c];
            if (cur->endcnt > 0) return word.substr(0, i + 1);
        }
        return "";
    }
    int findequal(string word) {
        Node* cur = root;
        for (char c : word) {
            c -= 'a';
            if (!cur->son[c]) return 0;
            cur = cur->son[c];
        }
        return cur->endcnt;
    }
    int findprefix(string prefix) {
        Node* cur = root;
        for (char c : prefix) {
            c -= 'a';
            if (!cur->son[c]) return 0;
            cur = cur->son[c];
        }
        return cur->pre;
    }
    void destory(Node* node) {
        if (!node) return;
        for (Node* son : node->son) destory(son);
        delete node;
    }

public:
    // 析构函数
    ~Trie() { destory(root); }

    // 往trie中插入word
    void insert(string word) {
        Node* cur = root;
        cur->pre++;
        for (char c : word) {
            c -= 'a';
            if (!cur->son[c]) cur->son[c] = new Node();
            cur = cur->son[c];
            cur->pre++;
        }
        cur->endcnt++;
    }

    // 查询trie中是否有word的前缀，若有返回，若无返回空串
    string pre(string word) { return findpre(word); }

    // 查询是否存在与word相等的字符串
    bool search(string word) { return find(word) == 2; }

    // 查询是否存在以prefix为前缀的字符串
    bool startsWith(string prefix) { return find(prefix) != 0; }

    // 查询插入了与word相同的数量
    int cntword(string word) { return findequal(word); }

    // 查询插入了以prefix为前缀的数量
    int cntprefix(string prefix) { return findprefix(prefix); }

    // 往trie中删除word
    void erase(string word) {
        Node* cur = root;
        cur->pre--;
        for (char c : word) {
            c -= 'a';
            cur = cur->son[c];
            cur->pre--;
        }
        cur->endcnt--;
    }
};