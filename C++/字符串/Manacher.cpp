// d1[i]: 以 i 为中心的奇回文半径，回文区间为 [i-d1[i]+1, i+d1[i]-1]
// d2[i]: 以 i-1 和 i 之间为中心的偶回文半径，回文区间为 [i-d2[i], i+d2[i]-1]
pair<vector<int>, vector<int>> manacher(const string& s) {
    int n = s.size();
    vector<int> d1(n), d2(n);

    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = i > r ? 1 : min(d1[l + r - i], r - i + 1);
        while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
        d1[i] = k--;
        if (i + k > r) l = i - k, r = i + k;
    }

    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = i > r ? 0 : min(d2[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
        d2[i] = k--;
        if (i + k > r) l = i - k - 1, r = i + k;
    }

    return {d1, d2};
}

// 数组版本，支持 vector<int> 等任意元素可比较的 vector
template<class T>
pair<vector<int>, vector<int>> manacher(const vector<T>& a) {
    int n = a.size();
    vector<int> d1(n), d2(n);

    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = i > r ? 1 : min(d1[l + r - i], r - i + 1);
        while (i - k >= 0 && i + k < n && a[i - k] == a[i + k]) k++;
        d1[i] = k--;
        if (i + k > r) l = i - k, r = i + k;
    }

    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = i > r ? 0 : min(d2[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n && a[i - k - 1] == a[i + k]) k++;
        d2[i] = k--;
        if (i + k > r) l = i - k - 1, r = i + k;
    }

    return {d1, d2};
}
