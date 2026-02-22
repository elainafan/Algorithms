// 模板题，最大子序列异或和
class XorBasis {
    vi b;

public:
    XorBasis(int n) : b(n) {}
    void insert(int x) {
        frep(i, sz(b) - 1, 0) {
            if (x >> i) {
                if (b[i] == 0) {
                    b[i] = x;
                    return;
                }
                x ^= b[i];
            }
        }
    }
    int max_xor() {
        int res = 0;
        frep(i, sz(b) - 1, 0) { res = max(res, res ^ b[i]); }
        return res;
    }
};
class Solution {
public:
    int maxXorSubsequences(vector<int>& nums) {
        int m = bit_width((uint32_t)ranges::max(nums));
        XorBasis b(m);
        for (int x : nums) b.insert(x);
        return b.max_xor();
    }
};