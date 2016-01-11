#ifndef SHAPES_H
#define SHAPES_H

#include "vector.h"

class Line {
    public:
    /* Default constructor */
    Line(Vec2i p1, Vec2i p2);

    Vec2i p1() const;
    Vec2i p2() const;
    void set_p1(Vec2i const& p);
    void set_p2(Vec2i const& p);

    /* Copy constructor */
    Line(Line const& other);
    /* Assignment operator */
    Line& operator=(Line const& other);

    private:
    Vec2i m_p1, m_p2;
};

class Circle {
    public:
    /* Default constructor */
    Circle(Vec2i center, int radius);

    Vec2i center() const;
    int radius() const;
    void set_center(Vec2i const& p);
    void set_radius(int r);

    /* Copy constructor */
    Circle(Circle const& other);
    /* Assignment operator */
    Circle& operator=(Circle const& other);

    private:
    Vec2i m_center;
    int m_radius;
};

class Polygon {
    public:
    /* Default constructor */
    Polygon();

    /* Copy constructor */
    Polygon(Polygon const& other);
    /* Assignment operator */
    Polygon& operator=(Polygon const& other);

    void add_point(Vec2i const& p);
    size_t n_points() const;

    std::vector<Vec2i>::iterator begin();
    std::vector<Vec2i>::const_iterator begin() const;
    std::vector<Vec2i>::iterator end();
    std::vector<Vec2i>::const_iterator end() const;

    void scale(float k);

    Vec2i first() const;
    Vec2i last() const;

    Vec2i barycenter() const;

    bool is_convex() const;

    enum Orientation { Left, Right };

    Orientation orientation() const;

    private:
    std::vector<Vec2i> m_pts;
    /* Convert v to polygon space, centered at its barycenter */
    Vec2i local(Vec2i const& v) const;
};

#endif
