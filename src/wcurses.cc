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

#include "wcurses/Wcurses.h"

#ifdef _WIN32
  #include "iostream"
  #include <sstream>

  #include "wcurses/buffer.h"
  #include "wcurses/color_manager.h"
  #include "wcurses/input_manager.h"
  #include "wcurses/terminal.h"
#else
  #include "ncurses/ncurses.h"
#endif

#include <string>
#include <thread> 

#include <wcurses/Key.h>
#include <wcurses/Point.h>

curs::Wcurses::~Wcurses() {
  End();
}

// Method for creating and obtaining a single instance of Wcurses (Singleton).
curs::Wcurses& curs::Wcurses::CreateWcurses() {
  static Wcurses wcurses;
  return wcurses;
}

#ifdef _WIN32
// Wcurses initialization method for Windows.
void curs::Wcurses::Start(Size size) {
  if(was_initialized_) {
    return;
  }

  // Allocate necessary resources.
  terminal_ = new internal::Terminal;
  buffer_ = new internal::Buffer(size);
  input_manager_ = new internal::InputManager; 

  // Configure terminal settings.
  terminal_->ClearScreen();
  terminal_->SetTerminalSize(size.rows, size.cols);
  terminal_->SetMaximizeButton(false);
  terminal_->SetWindowResizing(false);

  // Redirect standard I/O to prevent conflicts.
  original_cout_buffer_ = std::cout.rdbuf();
  original_cin_buffer_ = std::cin.rdbuf();

  std::cout.rdbuf(dummy_stream_.rdbuf());
  std::cin.rdbuf(dummy_stream_.rdbuf());

  RefreshScreen();

  was_initialized_ = true;
}
#else
// Wcurses initialization method for Linux/macOS.
void curs::Wcurses::Start() {
  initscr();
  keypad(stdscr, TRUE);
  noecho();
}
#endif

void curs::Wcurses::End() {
#ifdef _WIN32
  if(!was_initialized_) {
    return;
  }

  // Restore terminal settings.
  *terminal_ << buffer_->GetCodeResetColor();
  terminal_->RestoreTerminalMode();
  terminal_->SetMaximizeButton(true);
  terminal_->SetWindowResizing(true);
  terminal_->SetCursorVisible(true);

  terminal_->ClearScreen();

  // Free allocated resources.
  delete terminal_;
  terminal_ = nullptr;

  delete buffer_;
  buffer_ = nullptr;

  delete input_manager_;
  input_manager_ = nullptr;

  std::cout.rdbuf(original_cout_buffer_);
  std::cin.rdbuf(original_cin_buffer_);

  was_initialized_ = false;
#else
  endwin(); // Shutdown ncurses.
#endif
}

curs::Wcurses& curs::Wcurses::operator<<(char ch) {
#ifdef _WIN32
  if(!was_initialized_) {
    return *this;
  }

  *buffer_ << ch;
#else
  addch(ch);
#endif

  return *this;
}

curs::Wcurses& curs::Wcurses::operator<<(const char* str) {
#ifdef _WIN32
  if(!was_initialized_) {
    return *this;
  }

  *buffer_ << str;
#else
  printw(str);
#endif

  return *this;
}

curs::Wcurses& curs::Wcurses::operator<<(const std::string& str) {
#ifdef _WIN32
  if(!was_initialized_) {
    return *this;
  }

  *buffer_ << str;
#else
  printw(str.c_str());
#endif

  return *this;
}

curs::Wcurses& curs::Wcurses::operator<<(short val) {
#ifdef _WIN32
  if(!was_initialized_) {
    return *this;
  }

  *buffer_ << val;
#else
  printw("%d", val);
#endif

  return *this;
}

curs::Wcurses& curs::Wcurses::operator<<(int val) {
#ifdef _WIN32
  if(!was_initialized_) {
    return *this;
  }

  *buffer_ << val;
#else
  printw("%d", val);
#endif

  return *this;
}

curs::Wcurses& curs::Wcurses::operator<<(long val) {
#ifdef _WIN32
  if(!was_initialized_) {
    return *this;
  }

  *buffer_ << val;
#else
  printw("%ld", val);
#endif

  return *this;
}

curs::Wcurses& curs::Wcurses::operator<<(long long val) {
#ifdef _WIN32
  if(!was_initialized_) {
    return *this;
  }

  *buffer_ << val;
#else
  printw("%lld", val);
#endif

  return *this;
}

curs::Wcurses& curs::Wcurses::operator<<(unsigned short val) {
#ifdef _WIN32
  if(!was_initialized_) {
    return *this;
  }

  *buffer_ << val;
#else
  printw("%u", val);
#endif

  return *this;
}

curs::Wcurses& curs::Wcurses::operator<<(unsigned val) {
#ifdef _WIN32
  if(!was_initialized_) {
    return *this;
  }

  *buffer_ << val;
#else
  printw("%u", val);
#endif

  return *this;
}

curs::Wcurses& curs::Wcurses::operator<<(unsigned long val) {
#ifdef _WIN32
  if(!was_initialized_) {
    return *this;
  }

  *buffer_ << val;
#else
  printw("%lu", val);
#endif

  return *this;
}

curs::Wcurses& curs::Wcurses::operator<<(unsigned long long val) {
#ifdef _WIN32
  if(!was_initialized_) {
    return *this;
  }

  *buffer_ << val;
#else
  printw("%llu", val);
#endif

  return *this;
}

curs::Wcurses& curs::Wcurses::operator<<(float val) {
#ifdef _WIN32
  if(!was_initialized_) {
    return *this;
  }

  *buffer_ << val;
#else
  printw("%f", val);
#endif

  return *this;
}

curs::Wcurses& curs::Wcurses::operator<<(double val) {
#ifdef _WIN32
  if(!was_initialized_) {
    return *this;
  }

  *buffer_ << val;
#else
  printw("%f", val);
#endif

  return *this;
}

curs::Wcurses& curs::Wcurses::operator<<(long double val) {
#ifdef _WIN32
  if(!was_initialized_) {
    return *this;
  }

  *buffer_ << val;
#else
  printw("%f", val);
#endif

  return *this;
}

curs::Wcurses& curs::Wcurses::operator<<(Wcurses& (*pf)(Wcurses&)) {
  if(!was_initialized_) {
    return *this;
  }

  return pf(*this);
}

curs::Wcurses& curs::Wcurses::operator>>(int& val) {
#ifdef _WIN32
  if(!was_initialized_) {
    return *this;
  }

  *input_manager_ >> val;
#else
  val = getch();
#endif

  return *this;
}

curs::Wcurses& curs::Wcurses::operator>>(Key& val) {
#ifdef _WIN32
  if(!was_initialized_) {
    return *this;
  }

  *input_manager_ >> val;
#else
  val = static_cast<Key>(getch());
#endif

  return *this;
}


int curs::Wcurses::GetCh() {
#ifdef _WIN32
  if(!was_initialized_) {
    return input_manager_->ERR();
  }

  return input_manager_->GetCh();
#else
  return getch();
#endif
}

curs::Key curs::Wcurses::GetKey() {
#ifdef _WIN32
  if(!was_initialized_) {
    return Key::kError;
  }

  return input_manager_->GetKey();
#else
  return static_cast<Key>(getch());
#endif
}

void curs::Wcurses::SetNoDelay(bool enable) {
#ifdef _WIN32
  if(!was_initialized_) {
    return;
  }

  input_manager_->NoDelay(enable);
#else 
  nodelay(stdscr, enable);
#endif
}

void curs::Wcurses::FlushInput() {
#ifdef _WIN32
  if(!was_initialized_) {
    return;
  }

  input_manager_->Clear();
#else 
  flushinp();
#endif
}

void curs::Wcurses::RefreshScreen() {
#ifdef _WIN32
  if(!was_initialized_) {
    return;
  }

  // Get the current state of cursor visibility
  bool cursor_visibility = terminal_->GetCursorVisible();

  // If the cursor is visible, hide it to avoid flickering
  if(cursor_visibility) {
    SetCursorVisibility(false);
  }

  // Move the cursor to the upper left corner of the screen
  terminal_->ResetCursor();

  // Update the screen buffer
  buffer_->RefreshScreenBuffer();

  // Print the contents of the buffer to the terminal
  *terminal_ << buffer_->GetScreenBuffer();

  // Get the current cursor position from the buffer
  Point cursor = buffer_->GetCursorPosition(); 

  // Move the cursor to the desired position after the screen refreshes
  terminal_->MoveCursor(cursor.y, cursor.x);

  if (cursor_visibility) {
    SetCursorVisibility(true);
  }
#else   
  refresh();
#endif
}

bool curs::Wcurses::HasColor() {
#ifdef _WIN32
  if(!was_initialized_) {
    return false;
  }
  return terminal_->IsVirtualModeEnabled();
#else 
  return has_colors();
#endif
}

void curs::Wcurses::StartColor() {
#ifdef _WIN32
  if(!was_initialized_ || !HasColor()) {
    return;
  }

  buffer_->StartColor();
#else  
  start_color();
#endif
}

void curs::Wcurses::InitColor(
    internal::ColorManager::ColorIndex color_index,
    const RGB& rgb) {
#ifdef _WIN32
  if(!was_initialized_ || !HasColor()) {
    return;
  }

  buffer_->InitColor(color_index, rgb);
#else  
  init_color(pair, rgb.red * 1000 / 255, rgb.green * 1000 / 255, rgb.blue * 1000 / 255);
#endif
}

void curs::Wcurses::InitColor(
    internal::ColorManager::ColorIndex color_index,
    short r, short g, short b) {
#ifdef _WIN32
  if(!was_initialized_ || !HasColor()) {
    return;
  }

  buffer_->InitColor(color_index, r, g, b);
#else  
  init_color(pair, r * 1000 / 255, g * 1000 / 255, b * 1000 / 255);
#endif
}

void curs::Wcurses::InitPair(
    internal::ColorManager::PairIndex pair_index,
    const ColorPair& color_pair) {
#ifdef _WIN32
  if(!was_initialized_ || !HasColor()) {
    return;
  }

  buffer_->InitPair(pair_index, color_pair);
#else 
  init_pair(pair_index, color_pair.foreground, color_pair.background);
#endif
}

void curs::Wcurses::InitPair(
    internal::ColorManager::PairIndex pair_index,
    short foreground, short background) {
#ifdef _WIN32
  if(!was_initialized_ || !HasColor()) {
    return;
  }

  buffer_->InitPair(pair_index, foreground, background);
#else 
  init_pair(pair_index, foreground, background);
#endif
}

void curs::Wcurses::InitDefaultPair(internal::ColorManager::PairIndex pair_index) {
#ifdef _WIN32
  if(!was_initialized_ || !HasColor()) {
    return;
  }

  buffer_->InitDefaultPair(pair_index);
#else   
  bkgd(COLOR_PAIR(pair_index));
#endif 
}



void curs::Wcurses::SetActivePair(internal::ColorManager::PairIndex pair_index) {
#ifdef _WIN32
  if(!was_initialized_ || !HasColor()) {
    return;
  }

  buffer_->SetActivePair(pair_index);
#else 
  attron(COLOR_PAIR(pair));
#endif  
}

void curs::Wcurses::ResetToDefaultPair() {
#ifdef _WIN32
  if(!was_initialized_ || !HasColor()) {
    return;
  }

  buffer_->ResetToDefaultPair();
#else 
  attroff(A_COLOR);
#endif
}

void curs::Wcurses::Sleep(unsigned milliseconds) {
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}


#ifdef _WIN32
void curs::Wcurses::SetTitle(const std::string& title) {
  if(!was_initialized_) {
    return;
  }

  terminal_->SetTitle(title);
}

void curs::Wcurses::SetFontSettings(SHORT fontSize, const std::wstring& fontName) {
  if(!was_initialized_) {
    return;
  }

  terminal_->SetFontSettings(fontSize, fontName);
}
#endif

void curs::Wcurses::MoveTo(short y, short x) {
#ifdef _WIN32
  if(!was_initialized_) {
      return;
  }

  buffer_->Move(y, x);
#else 
  move(y, x);
#endif
}

void curs::Wcurses::MoveBy(short y, short x) {
#ifdef _WIN32
  if(!was_initialized_) {
    return;
  }

  buffer_->MoveBy(y, x);
#else 
  Point cursorPosition = getPos();
  move(cursorPosition.y + y, cursorPosition.x + x);
#endif
}

curs::Point curs::Wcurses::GetPosition() const {
#ifdef _WIN32
  if(!was_initialized_) {
    return {0, 0};
  }

  return buffer_->GetCursorPosition();
#else 
  Point cursorPosition{0, 0};

	getyx(stdscr, cursorPosition.y, cursorPosition.x);

	return cursorPosition; 
#endif
}

void curs::Wcurses::ClearScreen() {   
#ifdef _WIN32
  if(!was_initialized_) {
    return;
  }

  return buffer_->Clear();
  RefreshScreen();
#else   
  clear();
  RefreshScreen();
#endif
}

void curs::Wcurses::SetCursorVisibility(int visibility) {
#ifdef _WIN32
terminal_->SetCursorVisible(visibility);
#else 
  curs_set(visibility);
#endif
}

curs::Wcurses& curs::Endl(Wcurses& wcurses) {
  return wcurses << '\n';
}

namespace curs
{
	Wcurses& wcurses = Wcurses::CreateWcurses();
}