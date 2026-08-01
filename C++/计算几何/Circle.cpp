// 依赖 Point.cpp 和 Line.cpp，依次复制后再复制本文件

struct Circle {
    Point o;
    ld r = 0;
};

// -1：圆内，0：圆上，1：圆外。
auto pointCircleRelation(const Point& p, const Circle& c) -> int {
    return sgn(dist(p, c.o) - c.r);
}

auto lineCircleIntersection(const Line& l, const Circle& c)
    -> vector<Point> {
    if (norm2(l.v) <= EPS * EPS) {
        return pointCircleRelation(l.p, c) == 0
            ? vector<Point>{l.p}
            : vector<Point>{};
    }

    auto h = projection(c.o, l);
    auto d2 = norm2(h - c.o);
    auto r2 = c.r * c.r;
    int type = sgn(sqrtl(max<ld>(0, d2)) - c.r);
    if (type > 0) return {};
    if (type == 0) return {h};

    auto v = l.v * (sqrtl(max<ld>(0, r2 - d2)) / length(l.v));
    return {h - v, h + v};
}

// 两圆重合时有无穷多个交点，这里返回空 vector。
auto circleIntersection(const Circle& a, const Circle& b)
    -> vector<Point> {
    auto d = dist(a.o, b.o);
    if (sgn(d) == 0) return {};
    if (sgn(d - a.r - b.r) > 0 || sgn(d - fabsl(a.r - b.r)) < 0) return {};

    auto x = (a.r * a.r - b.r * b.r + d * d) / (2 * d);
    auto h2 = a.r * a.r - x * x;
    auto e = (b.o - a.o) / d;
    auto p = a.o + e * x;
    if (h2 <= EPS * EPS) return {p};

    auto q = perp(e) * sqrtl(max<ld>(0, h2));
    return {p - q, p + q};
}

// 从 p 向圆作切线，返回切点。
auto tangentPoints(const Point& p, const Circle& c) -> vector<Point> {
    auto v = p - c.o;
    auto d2 = norm2(v);
    auto r2 = c.r * c.r;
    int type = sgn(sqrtl(max<ld>(0, d2)) - c.r);
    if (type < 0) return {};
    if (type == 0) return {p};
    if (sgn(c.r) == 0) return {c.o};

    auto q = c.o + v * (r2 / d2);
    auto h = perp(v) * (c.r * sqrtl(d2 - r2) / d2);
    return {q - h, q + h};
}

auto circumcircle(const Point& a, const Point& b, const Point& c)
    -> optional<Circle> {
    auto u = b - a;
    auto v = c - a;
    auto d = 2 * cross(u, v);
    if (sgn(d) == 0) return nullopt;

    auto o = a + (perp(u) * norm2(v) - perp(v) * norm2(u)) / d;
    return Circle{o, dist(o, a)};
}

auto circleIntersectionArea(const Circle& a, const Circle& b) -> ld {
    auto d = dist(a.o, b.o);
    if (sgn(d - a.r - b.r) >= 0) return 0;
    if (sgn(d - fabsl(a.r - b.r)) <= 0) {
        auto r = min(a.r, b.r);
        return PI * r * r;
    }

    auto x = clamp(
        (d * d + a.r * a.r - b.r * b.r) / (2 * d * a.r),
        -1.0L, 1.0L
    );
    auto y = clamp(
        (d * d + b.r * b.r - a.r * a.r) / (2 * d * b.r),
        -1.0L, 1.0L
    );
    auto alpha = acosl(x);
    auto beta = acosl(y);
    return a.r * a.r * alpha + b.r * b.r * beta
         - d * a.r * sinl(alpha);
}

// 用法说明：
// Circle c{o,r} 表示圆心为 o、半径为 r 的圆，默认约定 r>=0。
// pointCircleRelation(p,c)：返回 -1 圆内，0 圆上，1 圆外。
// lineCircleIntersection(l,c)：返回直线与圆的 0/1/2 个交点。
// circleIntersection(a,b)：返回两圆的 0/1/2 个交点；
// 两圆重合时交点无穷多，也返回空 vector，需要时自行先判断圆心和半径。
// tangentPoints(p,c)：返回从点 p 向圆作切线的 0/1/2 个切点。
// circumcircle(a,b,c)：返回三点外接圆；三点共线时返回 nullopt。
// circleIntersectionArea(a,b)：返回两圆公共部分面积。
