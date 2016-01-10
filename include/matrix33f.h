#ifndef MATRIX33F_H
#define MATRIX33F_H

#include "vector.h"
#include <ostream>

class Matrix33f
{
    public:
        Matrix33f();
        Matrix33f(Matrix33f const& other);
        Matrix33f(float diag_value);
        Vec3f& operator[](std::size_t index);
        Vec3f const& operator[](std::size_t index) const;
        Matrix33f operator*(Matrix33f const& other) const;
        Vec3f operator*(Vec3f const& v) const;
        Vec2f operator*(Vec2f const& v) const;

        static Matrix33f identity();
        static Matrix33f newTranslation(Vec2f const& v);
        static Matrix33f newRotation(float rad_angle);
        static Matrix33f newScale(Vec2f const& v);
        static Matrix33f newScale(float k);
        Matrix33f& translate(Vec2f const& v);
        Matrix33f& rotate(float rad_angle);
        Matrix33f& scale(Vec2f const& v);
        Matrix33f& scale(float k);
        float det() const;
        Matrix33f inverse() const;

        Vec3f* begin();
        const Vec3f* begin() const;
        Vec3f* end();
        const Vec3f* end() const;

    private:
        Vec3f m_rows[3];
};

std::ostream& operator<<(std::ostream& s, Matrix33f const& m);

#endif // MATRIX33F_H
