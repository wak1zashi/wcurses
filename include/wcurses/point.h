// MIT License
//
// Copyright (c) 2025 wakizashi
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef WCURSES_POINT_H_
#define WCURSES_POINT_H_

#include <utility>

namespace curs {

template <typename T>
struct BasePoint {
  T y;
  T x;

  BasePoint(T x, T y);

  BasePoint(const BasePoint& other);
  BasePoint(const BasePoint&& other) noexcept;

  BasePoint& operator=(const BasePoint& other);
  BasePoint& operator=(BasePoint&& other) noexcept;

  bool operator==(const BasePoint& other) const;
  bool operator!=(const BasePoint& other) const;
};

template<typename T>
BasePoint<T>::BasePoint(T y, T x) : y(y), x(x) {}

template<typename T>
BasePoint<T>::BasePoint(const BasePoint& other) {
  if (this != &other) {
    x = other.x;
    y = other.y;
  }
}

template<typename T>
BasePoint<T>::BasePoint(const BasePoint&& other) noexcept {
  if (this != &other) {
    x = std::move(other.x);
    y = std::move(other.y);
  }
}

template<typename T>
BasePoint<T>& BasePoint<T>::operator=(const BasePoint& other) {
  if (this != &other) {
      x = other.x;
      y = other.y;
  }
  return *this;
}

template<typename T>
BasePoint<T>& BasePoint<T>::operator=(BasePoint&& other) noexcept {
  if (this != &other) {
      x = std::move(other.x);
      y = std::move(other.y);
  }
  return *this;
}

template<typename T>
bool BasePoint<T>::operator==(const BasePoint& other) const {
  return x == other.x && y == other.y;
}

template<typename T>
bool BasePoint<T>::operator!=(const BasePoint& other) const {
  return !(*this == other);
}

template<typename T>
BasePoint<T>& operator++(BasePoint<T>& point) {
  ++point.x;
  ++point.y;
  return point;
}

template<typename T>
BasePoint<T> operator++(BasePoint<T>& point, int) {
  BasePoint<T> temp(point);
  ++point;
  return temp;
}

template<typename T>
BasePoint<T>& operator--(BasePoint<T>& point) {
  --point.x;
  --point.y;
  return point;
}

template<typename T>
BasePoint<T> operator--(BasePoint<T>& point, int) {
  BasePoint<T> temp(point);
  --point;
  return temp;
}

using Point   = BasePoint<short>;
using PointI  = BasePoint<int>;
using PointL  = BasePoint<long>;
using PointLL = BasePoint<long long>;
using PointF  = BasePoint<float>;
using PointD  = BasePoint<double>;
using PointLD = BasePoint<long double>;
using PointC  = BasePoint<char>;

} // namespace curs

#endif // WCURSES_POINT_H_