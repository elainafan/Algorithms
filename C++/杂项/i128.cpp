__int128 read() {
    string s;
    cin >> s;
    __int128 x = 0;
    int f = 1, i = 0;
    if (s[0] == '-') f = -1, i = 1;
    for (; i < s.size(); i++) x = x * 10 + (s[i] - '0');
    return x * f;
}

void print(__int128 x) {
    if (x < 0) {
        cout << '-';
        x = -x;
    }
    if (x > 9) print(x / 10);
    cout << (int)(x % 10);
}
