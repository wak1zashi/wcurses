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

#include "wcurses/cursor.h"

#include "wcurses/point.h"

curs::internal::Cursor::Cursor()
  : cursor_(kMinCursor, kMinCursor), limit_(kMinLimit, kMinLimit) {}

curs::internal::Cursor::Cursor(const Point& range) 
  : cursor_(kMinCursor, kMinCursor), limit_(kMinLimit, kMinLimit) {
  SetLimit(range);
}

curs::internal::Cursor::Cursor(short max_y, short max_x) 
  : cursor_(kMinCursor, kMinCursor), limit_(kMinLimit, kMinLimit) {
  SetLimit(max_y, max_x);
}

inline bool curs::internal::Cursor::IsPointInRange(Point cursor) const {
  return ((cursor.y >= kMinCursor && cursor.y < limit_.y) &&
          (cursor.x >= kMinCursor && cursor.x < limit_.x));
}

inline bool curs::internal::Cursor::IsPointInRange(short y, short x) const {
  return ((y >= kMinCursor && y < limit_.y) && (x >= kMinCursor && x < limit_.x));
}

void curs::internal::Cursor::MoveDown() {
  SetY(cursor_.y + 1);
}

void curs::internal::Cursor::MoveRight() {
  SetX(cursor_.x + 1);
}

void curs::internal::Cursor::MoveUp() {
  SetY(cursor_.y - 1);
}

void curs::internal::Cursor::MoveLeft() {
  SetX(cursor_.x - 1);
}

void curs::internal::Cursor::Reset() {
  Move(0, 0);
}
void curs::internal::Cursor::ResetY() {
  SetY(0);
}
void curs::internal::Cursor::ResetX() {
  SetX(0);
}

bool curs::internal::Cursor::Move(const Point& cursor) {
  if (!IsPointInRange(cursor.y, cursor.x)) {
    return false;
  }

  cursor_ = cursor;

  return true;
}

bool curs::internal::Cursor::Move(short y, short x)  {
  return Move({y, x});
}

bool curs::internal::Cursor::MoveBy(const Point& cursor) {
  return Move(cursor_.y + cursor.y, cursor_.x + cursor.x);
}

bool curs::internal::Cursor::MoveBy(short dy, short dx) {
  return Move(cursor_.y + dy, cursor_.x + dx);
}

bool curs::internal::Cursor::SetY(short y) {
  return Move(y, cursor_.x);
}

bool curs::internal::Cursor::SetX(short x) {
  return Move(cursor_.y, x);
}

bool curs::internal::Cursor::SetLimit(const Point& limit) {
  if(limit.y < kMinLimit || limit.x < kMinLimit) {
      return false;
  }

  limit_ = limit;

  if(!IsPointInRange(cursor_)) {
    Move(kMinCursor, kMinCursor);
  }

  return true;
}

bool curs::internal::Cursor::SetLimit(short max_y, short max_x) {
  return SetLimit({max_y, max_x});
}