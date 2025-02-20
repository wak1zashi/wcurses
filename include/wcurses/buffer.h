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

#ifndef WCURSES_BUFFER_H_
#define WCURSES_BUFFER_H_

#include <string>
#include <vector>

#include "color_manager.h"
#include "cursor.h"
#include "point.h"

namespace curs {

struct Size {
  short rows;
  short cols;
};

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