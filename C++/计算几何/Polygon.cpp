// 依赖 Point.cpp 和 Line.cpp，依次复制后再复制本文件

auto signedArea2(const vector<Point>& p) -> ld {
    ld res = 0;
    for (int i = 0, n = p.size(); i < n; i++) {
        res += cross(p[i], p[(i + 1) % n]);
    }
    return res;
}

auto area(const vector<Point>& p) -> ld {
    return fabsl(signedArea2(p)) / 2;
}

auto perimeter(const vector<Point>& p) -> ld {
    ld res = 0;
    for (int i = 0, n = p.size(); i < n; i++) {
        res += dist(p[i], p[(i + 1) % n]);
    }
    return res;
}

// 0：外部，1：边界，2：内部。
auto pointInPolygon(const vector<Point>& poly, const Point& p) -> int {
    bool in = false;
    for (int i = 0, n = poly.size(); i < n; i++) {
        const auto& a = poly[i];
        const auto& b = poly[(i + 1) % n];
        if (onSegment(p, a, b)) return 1;

        if ((a.y > p.y) != (b.y > p.y)) {
            auto x = a.x + (b.x - a.x) * (p.y - a.y) / (b.y - a.y);
            if (sgn(x - p.x) > 0) in = !in;
        }
    }
    return in ? 2 : 0;
}

// 面积为 0 时退化成所有顶点的平均值。
auto centroid(const vector<Point>& p) -> Point {
    if (p.empty()) return {};

    ld sum = 0;
    Point res;
    for (int i = 0, n = p.size(); i < n; i++) {
        auto c = cross(p[i], p[(i + 1) % n]);
        sum += c;
        res = res + (p[i] + p[(i + 1) % n]) * c;
    }
    if (sgn(sum) != 0) return res / (3 * sum);

    res = {};
    for (const auto& x : p) res = res + x;
    return res / p.size();
}

// 返回逆时针凸包，不重复首点；边上的共线点只保留端点。
auto convexHull(vector<Point> p) -> vector<Point> {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());
    if (p.size() <= 1) return p;

    vector<Point> h(2 * p.size());
    int k = 0;
    for (const auto& x : p) {
        while (k >= 2 && sgn(cross(h[k - 2], h[k - 1], x)) <= 0) k--;
        h[k++] = x;
    }
    for (int i = int(p.size()) - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && sgn(cross(h[k - 2], h[k - 1], p[i])) <= 0) k--;
        h[k++] = p[i];
    }
    h.resize(k - 1);
    return h;
}

// 返回 0 外部、1 边界、2 内部；h 需要是 convexHull 返回的逆时针严格凸包。
auto pointInConvex(const vector<Point>& h, const Point& p) -> int {
    int n = h.size();
    if (!n) return 0;
    if (n == 1) return same(h[0], p) ? 1 : 0;
    if (n == 2) return onSegment(p, h[0], h[1]) ? 1 : 0;

    int lside = sgn(cross(h[0], h[1], p));
    int rside = sgn(cross(h[0], h[n - 1], p));
    if (lside < 0 || rside > 0) return 0;
    if (lside == 0) return onSegment(p, h[0], h[1]) ? 1 : 0;
    if (rside == 0) return onSegment(p, h[0], h[n - 1]) ? 1 : 0;

    int l = 1, r = n - 1;
    while (r - l > 1) {
        int m = (l + r) / 2;
        if (sgn(cross(h[0], h[m], p)) >= 0)
            l = m;
        else
            r = m;
    }
    int side = sgn(cross(h[l], h[l + 1], p));
    if (side < 0) return 0;
    if (side == 0) return onSegment(p, h[l], h[l + 1]) ? 1 : 0;
    return 2;
}

// p 需要是沿边界排列的凸多边形，返回直径。
auto convexDiameter(const vector<Point>& p) -> ld {
    int n = p.size();
    if (n <= 1) return 0;
    if (n == 2) return dist(p[0], p[1]);

    int j = 1;
    ld ans = 0;
    for (int i = 0; i < n; i++) {
        int ni = (i + 1) % n;
        while (fabsl(cross(p[ni] - p[i], p[(j + 1) % n] - p[i]))
             > fabsl(cross(p[ni] - p[i], p[j] - p[i])) + EPS) {
            j = (j + 1) % n;
        }
        ans = max({ans, norm2(p[i] - p[j]), norm2(p[ni] - p[j])});
    }
    return sqrtl(ans);
}

// 用法说明：
// 多边形用 vector<Point> 表示，顶点顺时针或逆时针依次存放，不重复首点。
// signedArea2(p) 返回两倍有向面积：逆时针为正，顺时针为负。
// area(p) / perimeter(p) 返回面积绝对值 / 周长；centroid(p) 返回重心。
// pointInPolygon(p,q) 判断点与任意简单多边形的位置：
// 返回 0 表示外部，1 表示边界，2 表示内部，复杂度 O(n)。
// convexHull(p) 接收无序点集，返回逆时针严格凸包，不重复首点；
// 重复点会被删除，边上的共线点只保留两端。
// pointInConvex(h,q) 判断点与凸包的位置，返回值同 pointInPolygon；
// h 必须是 convexHull 的返回值，单次查询复杂度 O(log n)。
// convexDiameter(h) 要求 h 已按凸包顺序排列，通常写：
// auto h = convexHull(p);
// ld diameter = convexDiameter(h);
