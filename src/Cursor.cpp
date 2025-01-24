#include "wcurses/Cursor.h"

__internal::Cursor::Cursor()
    : _cursor{_MIN_CURSOR, _MIN_CURSOR}, _range{_MIN_RANGE, _MIN_RANGE}
{ }

__internal::Cursor::Cursor(short maxY, short maxX)
    : _cursor{_MIN_CURSOR, _MIN_CURSOR}, _range{_MIN_RANGE, _MIN_RANGE}
{
    setRange(maxY, maxX);
}

__internal::Cursor::Cursor(const waki::Point& range)
    : _cursor{_MIN_CURSOR, _MIN_CURSOR}, _range{_MIN_RANGE, _MIN_RANGE}
{ 
    setRange(range);
}

__internal::Cursor::Cursor(short y, short x, short maxY, short maxX)
    : _cursor{_MIN_CURSOR, _MIN_CURSOR}, _range{_MIN_RANGE, _MIN_RANGE}
{
    setRange(maxY, maxX);
    move(y, x);
}

__internal::Cursor::Cursor(const waki::Point& cursor, const waki::Point& range)
    : _cursor{_MIN_CURSOR, _MIN_CURSOR}, _range{_MIN_RANGE, _MIN_RANGE}
{
    setRange(range);
    move(cursor);
}

inline bool __internal::Cursor::isPointInRange(short y, short x) const
{
    return (y >= _MIN_CURSOR && y < _range.y && x >= _MIN_CURSOR && x < _range.x);
}

void __internal::Cursor::incrementY()
{
    moveY(_cursor.y + 1);
}
void __internal::Cursor::incrementX()
{
    moveX(_cursor.x + 1);
}

void __internal::Cursor::decrementY()
{
    moveY(_cursor.y - 1);
}

void __internal::Cursor::decrementX()
{
    moveX(_cursor.x - 1);
}

void __internal::Cursor::reset()
{
    move(0, 0);
}

void __internal::Cursor::resetY()
{
    moveY(0);
}

void __internal::Cursor::resetX()
{
    moveX(0);
}

bool __internal::Cursor::move(short y, short x)
{
    if (!isPointInRange(y, x)) {
	    return false;
    }

    _cursor.y = y;
    _cursor.x = x;

    return true;
}

bool __internal::Cursor::move(const waki::Point& cursor)
{
    return move(cursor.y, cursor.x);
}

bool __internal::Cursor::moveBy(short dy, short dx)
{
    return move(_cursor.y + dy, _cursor.x + dx);
}

bool __internal::Cursor::moveBy(const waki::Point& dCursor)
{
    return moveBy(dCursor.y, dCursor.x);
}

bool __internal::Cursor::moveY(short y)
{
	if (!isPointInRange(y, _cursor.x)) {
		return false;
	}

	_cursor.y = y;

	return true;
}

bool __internal::Cursor::moveX(short x)
{
    if (!isPointInRange(_cursor.y, x)) {
        return false;
    }

    _cursor.x = x;

    return true;
}

bool __internal::Cursor::setRange(short maxY, short maxX)
{
    if(maxY < _MIN_RANGE || maxX < _MIN_RANGE)
    {
        return false;
    }

    _range.y = maxY;
    _range.x = maxX;

    if(!isPointInRange(_cursor.y, _cursor.x))
    {
        move(_MIN_CURSOR, _MIN_CURSOR);
    }

    return true;
}

bool __internal::Cursor::setRange(const waki::Point& range)
{
    return setRange(range.y, range.x);
}