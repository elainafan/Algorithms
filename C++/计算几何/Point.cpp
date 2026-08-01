// 基础点与向量，直接复制到计算几何代码最前面

using ld = long double;

inline constexpr ld EPS = 1e-12L;
inline const ld PI = acosl(-1.0L);

auto sgn(ld x) -> int {
    return (x > EPS) - (x < -EPS);
}

auto eq(ld x, ld y) -> bool {
    return sgn(x - y) == 0;
}

struct Point {
    ld x = 0, y = 0;

    auto operator+(const Point& p) const -> Point {
        return {x + p.x, y + p.y};
    }

    auto operator-(const Point& p) const -> Point {
        return {x - p.x, y - p.y};
    }

    auto operator*(ld k) const -> Point {
        return {x * k, y * k};
    }

    auto operator/(ld k) const -> Point {
        return {x / k, y / k};
    }

    auto operator<(const Point& p) const -> bool {
        return x != p.x ? x < p.x : y < p.y;
    }

    auto operator==(const Point& p) const -> bool {
        return x == p.x && y == p.y;
    }
};

auto operator*(ld k, const Point& p) -> Point {
    return p * k;
}

auto dot(const Point& a, const Point& b) -> ld {
    return a.x * b.x + a.y * b.y;
}

auto cross(const Point& a, const Point& b) -> ld {
    return a.x * b.y - a.y * b.x;
}

auto cross(const Point& o, const Point& a, const Point& b) -> ld {
    return cross(a - o, b - o);
}

auto norm2(const Point& p) -> ld {
    return dot(p, p);
}

auto length(const Point& p) -> ld {
    return sqrtl(max<ld>(0, norm2(p)));
}

auto dist(const Point& a, const Point& b) -> ld {
    return length(a - b);
}

auto same(const Point& a, const Point& b) -> bool {
    return eq(a.x, b.x) && eq(a.y, b.y);
}

auto perp(const Point& p) -> Point {
    return {-p.y, p.x};
}

auto rotate(const Point& p, ld rad) -> Point {
    auto c = cosl(rad), s = sinl(rad);
    return {p.x * c - p.y * s, p.x * s + p.y * c};
}

// 用法说明：
// Point 同时表示点和向量，b-a 表示从 a 指向 b 的向量。
// dot(a,b) 是点积；cross(a,b) 是叉积。
// cross(o,a,b) 的符号判断转向：>0 左转，=0 共线，<0 右转。
// norm2(v) 是长度平方，length(v) 是向量长度，dist(a,b) 是两点距离。
// perp(v) 将向量逆时针转 90°；rotate(v,rad) 按弧度旋转。
// same(a,b) 使用 EPS 判等；operator== 是精确判等，只用于排序去重。
