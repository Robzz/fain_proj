#include "matrix33f.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <sstream>

Matrix33f::Matrix33f() {
    std::fill(begin(), end(), Vec3f(0.f));
}

Matrix33f::Matrix33f(Matrix33f const& other) {
    std::copy(other.begin(), other.end(), begin());
}

Matrix33f::Matrix33f(float diag_value) {
    int i = 0;
    std::generate(begin(), end(), [&] () { Vec3f v(0.f); v[i] = diag_value; ++i; return v; });
}

Vec3f &Matrix33f::operator[](std::size_t index) {
    if(index > 2) {
        std::ostringstream oss;
        oss << "Invalid index for (3,3) matrix : " << index;
        throw std::out_of_range(oss.str());
    }
    return m_rows[index];
}

Vec3f const& Matrix33f::operator[](std::size_t index) const {
    if(index > 2) {
        std::ostringstream oss;
        oss << "Invalid index for (3,3) matrix : " << index;
        throw std::out_of_range(oss.str());
    }
    return m_rows[index];
}

Matrix33f Matrix33f::operator*(Matrix33f const& other) const {
    Matrix33f m;
    for(int i = 0 ; i != 3 ; ++i)
    for(int j = 0 ; j != 3 ; ++j) {
        float f = 0;
        for(int k = 0 ; k != 3 ; ++k)
            f += (*this)[i][k] * other[k][j];
        m[i][j] = f;
    }
    return m;
}

Vec3f Matrix33f::operator*(Vec3f const& v) const {
    Vec3f v2;
    for(int i = 0 ; i != 3 ; ++i)
        v2[i] = std::inner_product(v.begin(), v.end(), (*this)[i].begin(), 0.f);
    return v2;
}

Vec2f Matrix33f::operator*(Vec2f const& v) const {
    Vec3f v2 = (*this) * Vec3f(v.x(), v.y(), 1);
    return Vec2f(v2.x(), v2.y());
}

Matrix33f Matrix33f::identity() {
    return Matrix33f(1.f);
}

Matrix33f Matrix33f::newTranslation(Vec2f const& v) {
    Matrix33f m = identity();
    m[0][2] = v.x();
    m[1][2] = v.y();
    return m;
}

Matrix33f& Matrix33f::translate(Vec2f const& v) {
    m_rows[0][2] += v.x();
    m_rows[1][2] += v.y();

    return *this;
}

Matrix33f Matrix33f::newRotation(float rad_angle) {
    Matrix33f m = identity();
    float c = cos(rad_angle), s = sin(rad_angle);
    m[0][0] = c; m[0][1] = -s;
    m[1][0] = s; m[1][1] =  c;
    return m;
}

Matrix33f Matrix33f::newScale(const Vec2f &v) {
    Matrix33f m = Matrix33f::identity();
    m[0][0] *= v[0];
    m[1][1] *= v[1];

    return m;
}

Matrix33f Matrix33f::newScale(float f) {
    Matrix33f m = Matrix33f::identity();
    m[0][0] *= f;
    m[1][1] *= f;
    return m;
}

Matrix33f& Matrix33f::rotate(float rad_angle) {
    Matrix33f r = newRotation(rad_angle);
    *this = (*this) * r;

    return *this;
}

Matrix33f& Matrix33f::scale(Vec2f const& v) {
    (*this)[0][0] *= v.x();
    (*this)[0][1] *= v.y();
    (*this)[1][0] *= v.x();
    (*this)[1][1] *= v.y();

    return (*this);
}

Matrix33f& Matrix33f::scale(float k) {
    (*this)[0][0] *= k;
    (*this)[0][1] *= k;
    (*this)[1][0] *= k;
    (*this)[1][1] *= k;
    return (*this);
}

float Matrix33f::det() const {
    float a00 = (*this)[0][0], a01 = (*this)[0][1], a02 = (*this)[0][2],
          a10 = (*this)[1][0], a11 = (*this)[1][1], a12 = (*this)[1][2],
          a20 = (*this)[2][0], a21 = (*this)[2][1], a22 = (*this)[2][2];
    return (a00 * a11 * a22) +
           (a10 * a21 * a02) +
           (a20 * a01 * a12) -
           (a20 * a11 * a02) -
           (a10 * a01 * a22) -
           (a00 * a21 * a12);
}

#include <iostream>
Matrix33f Matrix33f::inverse() const {
    float d = det();
    if(d == 0.f)
        throw std::domain_error("Matrix is not inversible");
    Matrix33f i(Matrix33f::identity()), m(*this);
    // Make m sup triangular
    float k = (m[1][0]/m[0][0]);
    m[1] -= k * m[0];
    i[1] -= k * i[0];
    k = (m[2][0]/m[0][0]);
    m[2] -= k * m[0];
    i[2] -= k * i[0];
    k = (m[2][1]/m[1][1]);
    m[2] -= k * m[1];
    i[2] -= k * i[1];

    // Eliminate remaining non diagonal values
    k = (m[1][2]/m[2][2]);
    m[1] -= k * m[2];
    i[1] -= k * i[2];
    k = (m[0][2]/m[2][2]);
    m[0] -= k * m[2];
    i[0] -= k * i[2];
    k = (m[0][1]/m[1][1]);
    m[0] -= k * m[1];
    i[0] -= k * i[1];

    for(int j = 0 ; j != 3 ; ++j){
        k = m[j][j];
        m[j] /= k;
        i[j] /= k;
    }

    return i;
}

Vec3f* Matrix33f::begin() {
    return m_rows;
}

const Vec3f* Matrix33f::begin() const {
    return m_rows;
}

Vec3f* Matrix33f::end() {
    return m_rows + 3;
}

const Vec3f* Matrix33f::end() const {
    return m_rows + 3;
}

std::ostream&operator<<(std::ostream& s, const Matrix33f& m) {
    for(auto &it : m)
        s << it << std::endl;
    return s;
}
