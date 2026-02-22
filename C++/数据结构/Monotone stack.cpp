int largestRectangleArea(vector<int>& nums) {
    int n = nums.size();
    vector<int> r(n, n);
    vector<int> l(n, -1);
    stack<int> s;
    for (int i = n - 1; i >= 0; i--) {
        while (!s.empty() && nums[s.top()] >= nums[i]) s.pop();
        if (!s.empty()) r[i] = s.top();
        s.push(i);
    } // 求右边第一个小于的下标
    while (!s.empty()) s.pop();
    for (int i = 0; i <= n - 1; i++) {
        while (!s.empty() && nums[s.top()] >= nums[i]) s.pop();
        if (!s.empty()) l[i] = s.top();
        s.push(i);
    } // 求左边第一个小于的下标
    int maxx = INT_MIN;
    for (int i = 0; i <= n - 1; i++) maxx = max(maxx, nums[i] * (r[i] - l[i] - 1));
    return maxx;
}