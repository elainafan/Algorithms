// 依赖 Point.cpp，复制在 Point.cpp 后面

// 过 a、b 两点的直线，p 为起点，v 为方向向量。
struct Line {
    Point p, v;

    Line() = default;
    Line(const Point& a, const Point& b) : p(a), v(b - a) {}

    static auto fromVector(const Point& p, const Point& v) -> Line {
        return Line(p, p + v);
    }
};

auto parallel(const Line& a, const Line& b) -> bool {
    return sgn(cross(a.v, b.v)) == 0;
}

auto onLine(const Point& p, const Line& l) -> bool {
    if (norm2(l.v) <= EPS * EPS) return same(p, l.p);
    return sgn(cross(l.v, p - l.p)) == 0;
}

auto onSegment(const Point& p, const Point& a, const Point& b) -> bool {
    return sgn(cross(a, b, p)) == 0 && sgn(dot(p - a, p - b)) <= 0;
}

// 平行或重合时没有唯一交点，返回 nullopt。
auto lineIntersection(const Line& a, const Line& b) -> optional<Point> {
    auto d = cross(a.v, b.v);
    if (sgn(d) == 0) return nullopt;
    return a.p + a.v * (cross(b.p - a.p, b.v) / d);
}

auto projection(const Point& p, const Line& l) -> Point {
    auto d = norm2(l.v);
    if (d <= EPS * EPS) return l.p;
    return l.p + l.v * (dot(p - l.p, l.v) / d);
}

auto reflection(const Point& p, const Line& l) -> Point {
    return projection(p, l) * 2 - p;
}

auto distanceToLine(const Point& p, const Line& l) -> ld {
    auto d = length(l.v);
    if (sgn(d) == 0) return dist(p, l.p);
    return fabsl(cross(l.v, p - l.p)) / d;
}

auto distanceToSegment(const Point& p, const Point& a, const Point& b) -> ld {
    auto v = b - a;
    auto d = norm2(v);
    if (d <= EPS * EPS) return dist(p, a);
    auto t = dot(p - a, v) / d;
    if (t <= 0) return dist(p, a);
    if (t >= 1) return dist(p, b);
    return dist(p, a + v * t);
}

// 返回 0/1/2 个点；两线段重叠时返回公共部分的两个端点。
auto segmentIntersection(
    const Point& a, const Point& b, const Point& c, const Point& d
) -> vector<Point> {
    auto c1 = sgn(cross(a, b, c));
    auto c2 = sgn(cross(a, b, d));
    auto c3 = sgn(cross(c, d, a));
    auto c4 = sgn(cross(c, d, b));

    if (c1 * c2 < 0 && c3 * c4 < 0) {
        return {*lineIntersection(Line(a, b), Line(c, d))};
    }

    vector<Point> res;
    auto add = [&](const Point& p) -> void {
        for (const auto& q : res) {
            if (same(p, q)) return;
        }
        res.push_back(p);
    };
    if (onSegment(a, c, d)) add(a);
    if (onSegment(b, c, d)) add(b);
    if (onSegment(c, a, b)) add(c);
    if (onSegment(d, a, b)) add(d);

    sort(res.begin(), res.end());
    if (res.size() > 2) return {res.front(), res.back()};
    return res;
}

auto segmentsIntersect(
    const Point& a, const Point& b, const Point& c, const Point& d
) -> bool {
    return !segmentIntersection(a, b, c, d).empty();
}

// 用法说明：
// Line l(a,b) 表示经过 a、b 的直线；Line::fromVector(p,v) 表示过 p、方向为 v。
// parallel(l1,l2) 判断平行；onLine(p,l) 判断点在线上。
// lineIntersection(l1,l2) 返回唯一交点；平行或重合都返回 nullopt。
// projection(p,l) / reflection(p,l) 分别求投影点 / 对称点。
// onSegment(p,a,b) 判断 p 是否在线段 ab 上，包含端点。
// distanceToLine(p,l) / distanceToSegment(p,a,b) 分别求点到直线 / 线段距离。
// segmentIntersection(a,b,c,d) 求两线段公共部分：
// 空 vector 表示不交，1 个点表示相交或相切，2 个点表示重叠区间的两端。
// 只需要判断是否相交时使用 segmentsIntersect(a,b,c,d)。

// 整数坐标版本：谓词使用精确的 ll 运算；交点和距离等可能为非整数，返回 Point / ld。
struct LineI {
    PointI p, v;

    LineI() = default;
    LineI(const PointI& a, const PointI& b) : p(a), v(b - a) {}

    static auto fromVector(const PointI& p, const PointI& v) -> LineI {
        return LineI(p, p + v);
    }
};

auto parallel(const LineI& a, const LineI& b) -> bool {
    return cross(a.v, b.v) == 0;
}

auto onLine(const PointI& p, const LineI& l) -> bool {
    return norm2(l.v) == 0 ? p == l.p : cross(l.v, p - l.p) == 0;
}

auto onSegment(const PointI& p, const PointI& a, const PointI& b) -> bool {
    return cross(a, b, p) == 0 && dot(p - a, p - b) <= 0;
}

auto lineIntersection(const LineI& a, const LineI& b) -> optional<Point> {
    auto d = cross(a.v, b.v);
    if (d == 0) return nullopt;
    ld t = (ld)cross(b.p - a.p, b.v) / d;
    return toPoint(a.p) + toPoint(a.v) * t;
}

auto projection(const PointI& p, const LineI& l) -> Point {
    auto d = norm2(l.v);
    if (d == 0) return toPoint(l.p);
    ld t = (ld)dot(p - l.p, l.v) / d;
    return toPoint(l.p) + toPoint(l.v) * t;
}

auto reflection(const PointI& p, const LineI& l) -> Point {
    return projection(p, l) * 2 - toPoint(p);
}

auto distanceToLine(const PointI& p, const LineI& l) -> ld {
    auto d = norm2(l.v);
    if (d == 0) return dist(p, l.p);
    return fabsl((ld)cross(l.v, p - l.p)) / sqrtl((ld)d);
}

auto distanceToSegment(const PointI& p, const PointI& a, const PointI& b) -> ld {
    auto v = b - a;
    auto d = norm2(v);
    if (d == 0) return dist(p, a);
    ld t = (ld)dot(p - a, v) / d;
    if (t <= 0) return dist(p, a);
    if (t >= 1) return dist(p, b);
    return dist(toPoint(p), projection(p, LineI(a, b)));
}

auto segmentIntersection(
    const PointI& a, const PointI& b, const PointI& c, const PointI& d
) -> vector<Point> {
    auto sign = [](ll x) -> int { return (x > 0) - (x < 0); };
    auto c1 = sign(cross(a, b, c));
    auto c2 = sign(cross(a, b, d));
    auto c3 = sign(cross(c, d, a));
    auto c4 = sign(cross(c, d, b));
    if (c1 * c2 < 0 && c3 * c4 < 0) {
        return {*lineIntersection(LineI(a, b), LineI(c, d))};
    }

    vector<Point> res;
    auto add = [&](const PointI& p) -> void {
        auto q = toPoint(p);
        for (const auto& x : res) {
            if (same(q, x)) return;
        }
        res.push_back(q);
    };
    if (onSegment(a, c, d)) add(a);
    if (onSegment(b, c, d)) add(b);
    if (onSegment(c, a, b)) add(c);
    if (onSegment(d, a, b)) add(d);
    sort(res.begin(), res.end());
    if (res.size() > 2) return {res.front(), res.back()};
    return res;
}

auto segmentsIntersect(
    const PointI& a, const PointI& b, const PointI& c, const PointI& d
) -> bool {
    auto sign = [](ll x) -> int { return (x > 0) - (x < 0); };
    auto c1 = sign(cross(a, b, c));
    auto c2 = sign(cross(a, b, d));
    auto c3 = sign(cross(c, d, a));
    auto c4 = sign(cross(c, d, b));
    if (c1 * c2 < 0 && c3 * c4 < 0) return true;
    return (c1 == 0 && onSegment(c, a, b))
        || (c2 == 0 && onSegment(d, a, b))
        || (c3 == 0 && onSegment(a, c, d))
        || (c4 == 0 && onSegment(b, c, d));
}

// 用法：LineI / onLine / onSegment / segmentsIntersect 可全程保持整数判断；
// lineIntersection、projection、reflection 和距离接口在结果不是整数时返回浮点值。
