#ifndef VECTOR_H
#define VECTOR_H

#include "array.h"
#include <algorithm>
#include <functional>
#include <numeric>

template <class T, int size>
class Vector: public Array<T, size> {
    public:
        Vector() : Array<T,size>() {}

        Vector(Vector<T,size> const& other) : Array<T,size>(static_cast<Array<T,size> >(other)) { }

        Vector(Array<T,size> const& other) : Array<T,size>(other) { }

        Vector(T raw_array[size]) : Array<T,size>(raw_array) { }

        Vector(T const& item) : Array<T,size>(item) { }

        Vector(std::initializer_list<T> const& l) : Array<T, size>(l) { }

        Vector operator+(Vector const& other) const {
            return binary_op(*this, other, [] (T t1, T t2) { return t1 + t2; });
        }

        Vector& operator+=(Vector const& other) {
            binary_op_in_place(*this, other, [] (T t1, T t2) { return t1 + t2; });
            return *this;
        }

        Vector operator-(Vector const& other) const {
            return binary_op(*this, other, [] (T t1, T t2) { return t1 - t2; });
        }

        Vector operator-() const {
            return unary_op(*this, [] (T t1) { return -t1; });
        }

        Vector& operator-=(Vector const& other) {
            binary_op_in_place(*this, other, [] (T t1, T t2) { return t1 - t2; });
            return *this;
        }

        Vector operator*(T scalar) const {
            return unary_op(*this, [=] (T t) -> T { return t * scalar; });
        }

        Vector& operator*=(T scalar) {
            unary_op_in_place(*this, [=] (T t) { return t * scalar; });
            return *this;
        }

        Vector operator/(T scalar) const {
            return unary_op(*this, [=] (T t) -> T { return t / scalar; });
        }

        Vector& operator/=(T scalar) {
            unary_op_in_place(*this, [=] (T t) { return t / scalar; });
            return *this;
        }

        static Vector binary_op(Vector const& v1, Vector const& v2,
                                      std::function<T(T, T)> f) {
            Vector v;
            std::transform(v1.begin(), v1.end(), v2.begin(), v.begin(),
                           [&] (T t1, T t2) -> T { return f(t1, t2); });
            return v;
        }

        static void binary_op_in_place(Vector& v1, Vector const& v2,
                                             std::function<T(T, T)> f) {
            std::transform(v1.begin(), v1.end(), v2.begin(), v1.begin(),
                           [&] (T t1, T t2) -> T { return f(t1, t2); });
        }

        static Vector unary_op(Vector const& v, std::function<T(T)> f) {
            Vector r;
            std::transform(v.begin(), v.end(), r.begin(),
                           [&] (T t) -> T { return f(t); });
            return r;
        }

        static void unary_op_in_place(Vector& v, std::function<T(T)> f) {
            std::transform(v.begin(), v.end(), v.begin(),
                           [&] (T t) -> T { return f(t); });
        }
};

class Vec2f: public Vector<float, 2> {
    public:
        Vec2f() : Vector<float, 2>() { }

        Vec2f(float x, float y) : Vector<float, 2>() {
            m_items[0] = x;
            m_items[1] = y;
        }

        Vec2f(Vector<float,2> const& other) : Vector<float,2>(other) { }

        Vec2f(Array<float,2> const& other) : Vector<float,2>(other){ }

        Vec2f(float raw_array[2]) : Vector<float,2>(raw_array){ }

        Vec2f(float const& item) : Vector<float,2>(item) { }

        Vec2f(std::initializer_list<float> const& l) : Vector<float, 2>(l) { }

        float operator*(Vec2f const& other) {
            return std::inner_product(begin(), end(), other.begin(), 0.f);
        }

        float operator^(Vec2f const& other) {
            return (*this)[0] * other[1] - (*this)[1] * other[0];
        }

        float x() const {
            return (*this)[0];
        }

        float& x() {
            return (*this)[0];
        }

        float y() const {
            return (*this)[1];
        }

        float& y() {
            return (*this)[1];
        }
};

class Vec2i: public Vector<int, 2> {
    public:
        Vec2i() : Vector<int, 2>() { }

        Vec2i(int x, int y) : Vector<int, 2>() {
            m_items[0] = x;
            m_items[1] = y;
        }

        Vec2i(Vector<int,2> const& other) : Vector<int,2>(other) { }

        Vec2i(Array<int,2> const& other) : Vector<int,2>(other){ }

        Vec2i(int raw_array[2]) : Vector<int,2>(raw_array){ }

        Vec2i(int const& item) : Vector<int,2>(item) { }

        Vec2i(std::initializer_list<int> const& l) : Vector<int, 2>(l) { }

        int operator*(Vec2i const& other) {
            return std::inner_product(begin(), end(), other.begin(), 0.f);
        }

        int operator^(Vec2i const& other) {
            return (*this)[0] * other[1] - (*this)[1] * other[0];
        }

        int x() const {
            return (*this)[0];
        }

        int& x() {
            return (*this)[0];
        }

        int y() const {
            return (*this)[1];
        }

        int& y() {
            return (*this)[1];
        }
};


class Vec3f: public Vector<float, 3> {
    public:
        Vec3f() : Vector<float, 3>() { }

        Vec3f(float x, float y, float z) : Vector<float, 3>() {
            m_items[0] = x;
            m_items[1] = y;
            m_items[2] = z;
        }

        Vec3f(Vector<float,3> const& other) : Vector<float,3>(other) { }

        Vec3f(Array<float,3> const& other) : Vector<float,3>(other){ }

        Vec3f(float raw_array[3]) : Vector<float,3>(raw_array){ }

        Vec3f(float const& item) : Vector<float,3>(item) { }

        Vec3f(std::initializer_list<float> const& l) : Vector<float, 3>(l) { }

        float operator*(Vec3f const& other) {
            return std::inner_product(begin(), end(), other.begin(), 0.f);
        }

        Vec3f operator^(Vec3f const& other) {
            return Vec3f((*this)[2] * other[1] - (*this)[1] * other[2],
                         (*this)[2] * other[0] - (*this)[0] * other[2],
                         (*this)[0] * other[1] - (*this)[1] * other[0]);
        }

        float x() const {
            return (*this)[0];
        }

        float& x() {
            return (*this)[0];
        }

        float y() const {
            return (*this)[1];
        }

        float& y() {
            return (*this)[1];
        }

        float z() const {
            return (*this)[2];
        }

        float& z() {
            return (*this)[2];
        }
};

template<class T,int size>
Vector<T,size> operator*(T f, Vector<T,size> v) {
    return v * f;
}

#endif // VECTOR_H

