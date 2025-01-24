#pragma once

namespace waki
{
    template <typename _Ty>
    struct BasePoint {
        _Ty y;
        _Ty x;

        BasePoint& operator=(const BasePoint& other);
    };

    template<typename _Ty>
    inline BasePoint<_Ty>& BasePoint<_Ty>::operator=(const BasePoint& other)
    {
        y = other.y;
        x = other.x;
        return *this;
    }

    template<typename _Ty>
    inline bool operator==(const BasePoint<_Ty>& left, const BasePoint<_Ty>& right)
    {
        return left.y == right.y && left.x == right.x;
    }

    template<typename _Ty>
    inline bool operator!=(const BasePoint<_Ty>& left, const BasePoint<_Ty>& right)
    {
        return left.y != right.y || left.x != right.x;
    }

    template<typename _Ty>
    inline bool operator>(const BasePoint<_Ty>& left, const BasePoint<_Ty>& right)
    {
        return left.y != right.y || left.x != right.x;
    }

    template<typename _Ty>
    inline BasePoint<_Ty>& operator++(BasePoint<_Ty>& point)
    {
        ++point.y;
        ++point.x;
        return point;
    }

    template<typename _Ty>
    inline BasePoint<_Ty> operator++(BasePoint<_Ty>& point, int)
    {
        BasePoint<_Ty> temp(point);
        ++point.y;
        ++point.x;
        return temp;
    }

    template<typename _Ty>
    inline BasePoint<_Ty>& operator--(BasePoint<_Ty>& point)
    {
        --point.y;
        --point.x;
        return point;
    }

    template<typename _Ty>
    inline BasePoint<_Ty> operator--(BasePoint<_Ty>& point, int)
    {
        BasePoint<_Ty> temp(point);
        --point.y;
        --point.x;
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
} // namespace waki