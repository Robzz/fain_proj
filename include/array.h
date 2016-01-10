#ifndef ARRAY_H
#define ARRAY_H

#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>

template <typename T, int size>
class Array
{
    public:
        Array() : m_items() {}

        Array(Array<T,size> const& other) : m_items() {
            std::copy(other.begin(), other.end(), begin());
        }

        Array(T raw_array[size]) : m_items() {
            std::copy(&raw_array[0], &raw_array[0] + size, begin());
        }

        Array(T const& item) : m_items() {
            std::fill(begin(), end(), item);
        }

        Array(std::initializer_list<T> const& l) : m_items() {
            if(l.size() != size) {
                throw std::length_error("Wrong number of items in initializer list");
            }
            std::copy(l.begin(), l.end(), begin());
        }

        T& operator[](std::size_t index) {
            if(index >= size) {
                std::ostringstream oss;
                oss << "Invalid index for array of size " << size << ": " << index;
                throw std::out_of_range(oss.str());
            }
            return m_items[index];
        }

        T const& operator[](std::size_t index) const {
            if(index >= size) {
                std::ostringstream oss;
                oss << "Invalid index for array of size " << size << ": " << index;
                throw std::out_of_range(oss.str());
            }
            return m_items[index];
        }

        bool operator==(Array<T, size> const& other) const {
            return std::equal(other.begin(), other.end(), begin());
        }

        Array<T, size>& operator=(Array<T,size> const& other) {
            std::copy(other.begin(), other.end(), begin());
            return *this;
        }

        Array<T, size>& operator=(T const other[size]) {
            std::copy(&other[0], &other[0] + size, begin());
            return *this;
        }

        void swap(Array<T, size>& other) {
            std::swap_ranges(other.begin(), other.end(), begin());
        }

        T* begin() {
            return m_items;
        }

        const T* begin() const {
            return m_items;
        }

        T* end() {
            return m_items + size;
        }

        const T* end() const {
            return m_items + size;
        }


    protected:
        T m_items[size];
};

template <class T, int size>
std::ostream& operator<<(std::ostream& s, Array<T, size> a) {
    for(T& v: a)
        s << v << ' ';
    return s;
}

#endif // ARRAY_H
