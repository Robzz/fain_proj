#include "shapes.h"
#include "matrix33f.h"
#include <limits>
#include <cmath>

Line::Line(Vec2i p1, Vec2i p2) :
    m_p1(p1),
    m_p2(p2)
{ }

Line::Line(Line const& other) :
    m_p1(other.m_p1),
    m_p2(other.m_p2)
{ }

Line& Line::operator=(Line const& other) {
    m_p1 = other.m_p1;
    m_p2 = other.m_p2;
    return *this;
}

Vec2i Line::p1() const {
    return m_p1;
}

Vec2i Line::p2() const {
    return m_p2;
}

void Line::set_p1(Vec2i const& p) {
    m_p1 = p;
}

void Line::set_p2(Vec2i const& p) {
    m_p2 = p;
}

Circle::Circle(Vec2i center, int radius) :
    m_center(center),
    m_radius(radius)
{ }

Circle::Circle(Circle const& other) :
    m_center(other.m_center),
    m_radius(other.m_radius)
{ }

Circle& Circle::operator=(Circle const& other) {
    m_center = other.m_center;
    m_radius = other.m_radius;
    return *this;
}

Vec2i Circle::center() const { return m_center; }
int Circle::radius() const { return m_radius; }

void Circle::set_center(Vec2i const& center) { m_center = center; }
void Circle::set_radius(int r) { m_radius = r; }

Polygon::Polygon() :
    m_pts()
{ }

Polygon::Polygon(Polygon const& other) :
    m_pts(other.m_pts)
{ }

Polygon& Polygon::operator=(Polygon const& other) {
    m_pts = other.m_pts;
    return *this;
}

void Polygon::add_point(Vec2i const& p) {
    // Discard the point if already the last point
    if(m_pts.empty() || p != last())
        m_pts.push_back(p);
}

size_t Polygon::n_points() const { return m_pts.size(); }

Vec2i Polygon::first() const { return m_pts.front(); }
Vec2i Polygon::last() const { return m_pts.back(); }

Vec2i Polygon::barycenter() const {
    Vec2i b(0,0);
    for(auto it = begin() ; it != end() ; ++it) {
        b += *it;
    }
    return b / n_points();
}

bool Polygon::is_convex() const {
    /* Using an overridden local() function avoids having to recompute
     * the barycenter for every vertex */
    Vec2i b = barycenter();
    auto sign = []  (int x) { return (x < 0) ? -1 : 1; };
    std::function<Vec2i(Vec2i)> local = [&b] (Vec2i v) { return b - v; };
    int s = local(last()) ^ local(first());
    s = sign(s);
    for(auto it = begin() ; it+1 != end() ; ++it) {
        Vec2i p1 = *it, p2 = *(it+1);
        int s2 = local(p1) ^ local(p2);
        s2 = sign(s2);
        if(s != s2)
            return false;
    }
    return true;
}

Polygon::Orientation Polygon::orientation() const {
    if(is_convex()) {
        return ((local(last()) ^ local(first())) > 0) ? Left : Right;
    }
    else {
        int m = std::numeric_limits<int>::max();
        Vec2i leftmost(m, m);
        for(auto it = begin() ; it != end() ; ++it) {
            if((*it).x() < leftmost.x() || ((*it).x() == leftmost.x() && (*it).y() < leftmost.y()))
                leftmost = *it;
        }
        Vec2i p, prev, next;
        for(auto it = begin() ; it != end() ; ++it) {
            if(*it == leftmost) {
                p = *it;
                if(it == begin()) {
                    prev = last();
                }
                if(it+1 == end()) {
                    next = first();
                }
            }
        }
        float a1 = (p.y() - prev.y()) / (p.x() - prev.x()), a2 = (p.y() - next.y())/(p.x() - next.x());
        return (a2 < a1) ? Left : Right ;
    }
}

Vec2i Polygon::local(Vec2i const& v) const {
    return barycenter() - v;
}

void Polygon::scale(float k) {
    Vec2i b = barycenter();
    Matrix33f t = Matrix33f::newTranslation(Vec2f(-b.x(), -b.y()));
    Matrix33f ti = Matrix33f::newTranslation(Vec2f(b.x(), b.y()));
    Matrix33f s = Matrix33f::newScale(k);
    Matrix33f m = ti * s * t;
    for(auto it = begin() ; it != end() ; ++it) {
        Vec3f v((*it).x(), (*it).y(), 1);
        v = m * v;
        *it = Vec2i(v.x(), v.y());
    }
}

void Polygon::rotate(Polygon::Orientation dir) {
    Vec2i b = barycenter();
    Matrix33f t = Matrix33f::newTranslation(Vec2f(-b.x(), -b.y()));
    Matrix33f ti = Matrix33f::newTranslation(Vec2f(b.x(), b.y()));
    Matrix33f r = Matrix33f::newRotation(((dir == Polygon::Left) ? 90.f : -90.f) * M_PI / 180.f);
    Matrix33f m = ti * r * t;
    for(auto it = begin() ; it != end() ; ++it) {
        Vec3f v((*it).x(), (*it).y(), 1);
        v = m * v;
        *it = Vec2i(v.x(), v.y());
    }
}

std::vector<Vec2i>::iterator Polygon::begin() { return m_pts.begin(); }
std::vector<Vec2i>::const_iterator Polygon::begin() const { return m_pts.begin(); }
std::vector<Vec2i>::iterator Polygon::end() { return m_pts.end(); }
std::vector<Vec2i>::const_iterator Polygon::end() const { return m_pts.end(); }
