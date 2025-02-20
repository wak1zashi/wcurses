// This file is part of wcuses.
//
// wcuses is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// wcuses is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with wcuses.  If not, see <http://www.gnu.org/licenses/>.

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