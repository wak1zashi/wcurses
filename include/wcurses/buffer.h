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

#ifndef WCURSES_BUFFER_H_
#define WCURSES_BUFFER_H_

#include <string>
#include <vector>

#include "color_manager.h"
#include "cursor.h"
#include "point.h"
#include "structures.h"

namespace curs {
namespace internal {

struct ChType {
  char symbol = ' ';
  ColorManager::PairIndex color_pair = 0;
};

// The Buffer class implements an internal mechanism for storing and
// manipulating text data in a buffer, including support for color settings
// and cursor position control. This class is responsible for managing the
// contents of the text buffer, which contains characters, color information,
// and the current cursor position.
class Buffer {
  public:
    using ScreenBufferType = std::string;

    // Constructs a Buffer with a specified color manager and size.
    explicit Buffer(Size size);
    explicit Buffer(short rows, short cols);

    // Overloaded stream operators for appending different types of data to the buffer.
    Buffer& operator<<(char ch);
    Buffer& operator<<(const char* str);
    Buffer& operator<<(const std::string& str);
    Buffer& operator<<(short val);
    Buffer& operator<<(unsigned short val);
    Buffer& operator<<(int val);
    Buffer& operator<<(unsigned int val);
    Buffer& operator<<(long val);
    Buffer& operator<<(unsigned long val);
    Buffer& operator<<(long long val);
    Buffer& operator<<(unsigned long long val);
    Buffer& operator<<(float val);
    Buffer& operator<<(double val);
    Buffer& operator<<(long double val);
    
    // Deletes the old buffer completely and creates a new one with the given size.
    void Resize(Size new_size);
    void Resize(short new_rows, short new_cols);

    // Converts the internal buffer (buffer_ or buffer_char_) into a linear string format.
    // If color support is available, it adds appropriate escape sequences.
    void RefreshScreenBuffer();

    // Clears the internal buffer but does not modify the screen buffer.
    void Clear();

    // Moves the cursor to a new line.
    void NewLine();

    // Moves the cursor to the specified position.
    void Move(short y, short x);
    void Move(const Point& cursor_position);

    // Moves the cursor by the specified offset.
    void MoveBy(short delta_y, short delta_x);
    void MoveBy(const Point& cursor_offset);

    // Initializes color support.
    void StartColor();

    // Defines a custom color.
    void InitColor(ColorManager::ColorIndex color_index, const RGB& rgb);
    void InitColor(ColorManager::ColorIndex color_index, short r, short g, short b);

    // Defines a color pair.
    void InitPair(ColorManager::PairIndex pair_index, const ColorPair& color_pair);
    void InitPair(ColorManager::PairIndex pair_index, short foreground, short background);

    // Initializes a default color pair.
    void InitDefaultPair(ColorManager::PairIndex pair_index);

    // Sets the active color pair.
    void SetActivePair(ColorManager::PairIndex pair_index);

    // Resets the active color pair to default (0).
    void ResetToDefaultPair();

    // Getter methods
    std::string GetCodeResetColor() { return color_manager_.GetResetCode(); }
    const Point& GetCursorPosition() const { return cursor_.GetPosition(); } 
    const Size& GetSize() const { return size_; } 
    const std::string& GetScreenBuffer() const { return screen_buffer_; }

  private:
    using BufferType = std::vector<std::vector<ChType>>;
    using BufferCharType = std::vector<std::vector<char>>;

    const int kMinSize = 1;

    ScreenBufferType screen_buffer_;
    BufferType buffer_; // Stores characters with color information.
    BufferCharType buffer_char_; // Stores characters without color formatting.
    Cursor cursor_; // Tracks the current cursor position within the buffer.
    Size size_;
    ColorManager color_manager_; // Manages color attributes for text rendering.

    // Initializes the entire Buffer object.
    void Initialize(Size size);
   
    // Initializes the specified buffer to the given size.
    void InitializeBuffer(BufferType& buffer, Size size);
    void InitializeBuffer(BufferCharType& buffer, Size size, char fill_char = ' ');

    // Move all data from source to destination and clear source.
    void MigrateBuffer(BufferCharType& source, BufferType& destination);

};

} // namespace internal
} // namespace curs

#endif // WCURSES_BUFFER_H_