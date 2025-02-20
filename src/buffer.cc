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

#include "wcurses/buffer.h"

#include <cstring>

#include <string>
#include <vector>

#include "wcurses/color_manager.h"
#include "wcurses/cursor.h"
#include "wcurses/point.h"

curs::internal::Buffer::Buffer(Size size) {
  Initialize(size);
}

curs::internal::Buffer::Buffer(short rows, short cols) {
  Initialize({rows, cols});
}

curs::internal::Buffer& curs::internal::Buffer::operator<<(char ch) {
  // Move cursor to the next line if it reaches or exceeds the rightmost column.
  if (cursor_.GetX() >= size_.cols) {
    NewLine();
  }

  // Handle newline character explicitly.
  if (ch == '\n') {
    NewLine();
    return *this;
  }

  if(color_manager_.IsStartedColor()) {
    // Saving a character with a color if color mode is supported
    buffer_[cursor_.GetY()][cursor_.GetX()].symbol = ch;
    buffer_[cursor_.GetY()][cursor_.GetX()].color_pair = color_manager_.GetActivePair();
  } else {
    // Saving only a character
    buffer_char_[cursor_.GetY()][cursor_.GetX()] = ch;
  }

  // If the cursor reaches the last column, go to a new line
  if (cursor_.GetX() >= size_.cols - 1) {
    NewLine();
  } else {
    cursor_.MoveRight();
  }

  return *this;
}

curs::internal::Buffer& curs::internal::Buffer::operator<<(const char* str) {
  size_t length = std::strlen(str); // Get the length of the C-string

  for (int i = 0; i < length; ++i) {
    *this << str[i]; // Use an overloaded operator for the symbol
  }
 
  return *this;
}

curs::internal::Buffer& curs::internal::Buffer::operator<<(const std::string& str) {
  for (size_t i = 0; i < str.size(); ++i) {
  *this << str[i];
  }

  return *this;
}

curs::internal::Buffer& curs::internal::Buffer::operator<<(short val) {
  *this << std::to_string(val);

  return *this;
}

curs::internal::Buffer& curs::internal::Buffer::operator<<(unsigned short val) {
  *this << std::to_string(val);

  return *this;
}

curs::internal::Buffer& curs::internal::Buffer::operator<<(int val) {
  *this << std::to_string(val);

  return *this;
}

curs::internal::Buffer& curs::internal::Buffer::operator<<(unsigned int val) {
  *this << std::to_string(val);

  return *this;
}

curs::internal::Buffer& curs::internal::Buffer::operator<<(long val) {
  *this << std::to_string(val);

  return *this;
}

curs::internal::Buffer& curs::internal::Buffer::operator<<(unsigned long val) {
  *this << std::to_string(val);

  return *this;
}

curs::internal::Buffer& curs::internal::Buffer::operator<<(long long val) {
  *this << std::to_string(val);

  return *this;
}

curs::internal::Buffer& curs::internal::Buffer::operator<<(unsigned long long val) {
  *this << std::to_string(val);

  return *this;
}

curs::internal::Buffer& curs::internal::Buffer::operator<<(float val) {
  *this << std::to_string(val);

  return *this;
}

curs::internal::Buffer& curs::internal::Buffer::operator<<(double val) {
  *this << std::to_string(val);

  return *this;
}

curs::internal::Buffer& curs::internal::Buffer::operator<<(long double val) {
  *this << std::to_string(val);

  return *this;
}

void curs::internal::Buffer::Resize(Size new_size) {
  Initialize(new_size);
}

void curs::internal::Buffer::Resize(short new_rows, short new_cols) {
  Initialize({new_rows, new_cols});
}

void curs::internal::Buffer::RefreshScreenBuffer() {
  ColorManager::PairIndex current_pair = 0;
  bool is_color_active = color_manager_.IsStartedColor();

  if(is_color_active) {
    // Save the initial color pair to track changes
    current_pair = buffer_[0][0].color_pair;
    // Get the ESC code to set the initial text and background color
    screen_buffer_ = color_manager_.MakeColorCode(current_pair);
  }

  for(int i = 0; i < size_.rows; ++i) {
    for (int j = 0; j < size_.cols; ++j) {
      if(is_color_active) {
        ColorManager::PairIndex new_pair = buffer_[i][j].color_pair;

        if(new_pair != current_pair) {
          // Generate ESC code only for changed parameters
          screen_buffer_ += color_manager_.MakeColorCode(current_pair, new_pair);
    
          // Update the current color pair 
          current_pair = new_pair;
        }

        screen_buffer_ += buffer_[i][j].symbol;
      } else {
        screen_buffer_ += buffer_char_[i][j];
      }
    }
    // Add a newline character after each line except the last one
    if (i < size_.rows - 1) {
      screen_buffer_ += '\n';
    }
  }
}

void curs::internal::Buffer::Clear() {
  if(!buffer_.empty()) {
    for(auto& rows : buffer_) {
      for(auto& ch : rows) {
        ch = {' ', ColorManager::GetDefaultPair()};
      }
    }
  }

  if(!buffer_char_.empty()) {
    for(auto& rows : buffer_char_) {
      for(auto& ch : rows) {
        ch = ' ';
      }
    }
  }

  cursor_.Reset();
}

void curs::internal::Buffer::NewLine() {
	if (!cursor_.IsAtBottom()) {
		cursor_.MoveDown();
		cursor_.ResetX();
	}
}

void curs::internal::Buffer::Move(short y, short x) {
  cursor_.Move(y, x);
}

void curs::internal::Buffer::Move(const Point& cursor_position) {
  cursor_.Move(cursor_position);
}

void curs::internal::Buffer::MoveBy(short delta_y, short delta_x) {
  cursor_.MoveBy(delta_y, delta_x);
}

void curs::internal::Buffer::MoveBy(const Point& cursor_offset) {
  cursor_.MoveBy(cursor_offset);
}

void curs::internal::Buffer::StartColor() {
  if(color_manager_.IsStartedColor()) {
    return;
  }

  color_manager_.StartColor();

  // Move all characters from buffer_char_ to buffer_ since buffer_ supports color rendering
  MigrateBuffer(buffer_char_, buffer_);  
}

void curs::internal::Buffer::InitColor(
    ColorManager::ColorIndex color_index,
    const RGB& rgb) {
  color_manager_.InitColor(color_index, rgb);
}
void curs::internal::Buffer::InitColor(
    ColorManager::ColorIndex color_index,
    short r, short g, short b) {
  color_manager_.InitColor(color_index, r, g, b);
}

void curs::internal::Buffer::InitPair(
    ColorManager::PairIndex pair_index,
    const ColorPair& color_pair) {
  color_manager_.InitPair(pair_index, color_pair);
}

void curs::internal::Buffer::InitPair(
    ColorManager::PairIndex pair_index,
    short foreground, short background) {
  color_manager_.InitPair(pair_index, foreground, background);
}

void curs::internal::Buffer::InitDefaultPair(ColorManager::PairIndex pair_index) {
  color_manager_.InitDefaultPair(pair_index);
}

void curs::internal::Buffer::SetActivePair(ColorManager::PairIndex pair_index) {
  color_manager_.SetActivePair(pair_index);
}

void curs::internal::Buffer::ResetToDefaultPair() {
  color_manager_.ResetToDefault();
}

void curs::internal::Buffer::Initialize(Size size) {
  if(size.rows < kMinSize || size.cols < kMinSize) {
      size.rows = kMinSize;
      size.cols = kMinSize;
  }

  screen_buffer_.clear();
  screen_buffer_.shrink_to_fit();
  
  buffer_.clear();
  buffer_.shrink_to_fit();

  buffer_char_.clear();
  buffer_char_.shrink_to_fit();

  size_ = size;

  cursor_.SetLimit(size_.rows, size_.cols);

  if(color_manager_.IsStartedColor()) {
    InitializeBuffer(buffer_, size);
  } else {
    InitializeBuffer(buffer_char_, size);
  }
}

void curs::internal::Buffer::InitializeBuffer(BufferType& buffer, Size size) {
  buffer.resize(size.rows, std::vector<ChType>(size.cols)); 
}

void curs::internal::Buffer::InitializeBuffer(BufferCharType& buffer, Size size,
                                              char fill_char) {
    buffer.resize(size.rows, std::vector<char>(size.cols, fill_char));
}

void curs::internal::Buffer::MigrateBuffer(BufferCharType& source,
                                           BufferType& destination) {
  if(source.empty() || !destination.empty()) {
    return;
  }

  InitializeBuffer(destination, size_);

  for (int i = 0; i < size_.rows; ++i) {
    for (int j = 0; j < size_.cols; ++j) {
      destination[i][j].symbol = source[i][j];
    }
  }

  source.clear();
  source.shrink_to_fit();
}