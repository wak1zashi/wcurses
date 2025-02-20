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

#ifndef WCURSES_CURSOR_H_
#define WCURSES_CURSOR_H_

#include "point.h"

namespace curs {
namespace internal {

// Represents a cursor in a bounded 2D space.
// This class provides movement and boundary-checking operations.
class Cursor {
  public:

    // Default constructor initializes cursor at (0,0) with limit (1,1).
    Cursor();

    // Initializes cursor limit using a Point object.
    explicit Cursor(const Point& limit);
    
    // Initializes cursor with the specified maximum dimensions.
    explicit Cursor(short max_y, short max_x);

    // Checks if the given (y, x) coordinate is within the allowed range.
    bool IsPointInRange(Point cursor) const;
    bool IsPointInRange(short y, short x) const;

    // Moves the cursor one step (Down, Right, Up, Left).
    void MoveDown();
    void MoveRight();
    void MoveUp();
    void MoveLeft();

    // Resets the cursor position to (0,0).
    void Reset();

    // Resets only the Y position to 0.
    void ResetY();

    // Resets only the X position to 0.
    void ResetX();

    // Moves the cursor to the specified position.
    bool Move(const Point& cursor);
    bool Move(short y, short x);

    // Moves the cursor relative to its current position by (dy, dx).
    bool MoveBy(const Point& cursor);
    bool MoveBy(short dy, short dx);

    // Moves the cursor's Y position to a specific value.
    bool SetY(short y);

    // Moves the cursor's X position to a specific value.
    bool SetX(short x);

    // Sets a new limit for the cursor.
    bool SetLimit(const Point& limit);
    bool SetLimit(short max_y, short max_x);

    // Checks if the cursor is at the top boundary.
    bool IsAtTop() const    { return cursor_.y == kMinCursor; }
      
    // Checks if the cursor is at the bottom boundary.
    bool IsAtBottom() const { return cursor_.y == limit_.y - 1; }

    // Checks if the cursor is at the left boundary.
    bool IsAtLeft() const   { return cursor_.x == kMinCursor; }

    // Checks if the cursor is at the right boundary.
    bool IsAtRight() const  { return cursor_.x == limit_.x - 1; }

    // Getter methods
    const Point& GetPosition() const { return cursor_; } 
    short GetY() const { return cursor_.y; } 
    short GetX() const { return cursor_.x; } 

    const Point& GetLimit() const { return limit_; }
    short GetHeightLimit() const { return limit_.y; } 
    short GetWidthLimit() const { return limit_.x; } 

  private:
    // The minimum allowed cursor position.
    const short kMinCursor = 0; 

    // The minimum allowed limit size.
    const short kMinLimit = 1; 

    Point cursor_; // Current position of the cursor.
    Point limit_;  // Maximum allowed coordinates for the cursor.
};

} // namespace internal
} // namespace curs

#endif // WCURSES_CURSOR_H_
