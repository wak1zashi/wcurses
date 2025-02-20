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