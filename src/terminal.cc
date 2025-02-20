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

#ifdef _WIN32

#include "wcurses/terminal.h"

#include <Windows.h>

#include <string>

#include "wcurses/point.h"

curs::internal::Terminal::Terminal() {
  cursor_visibility_ = true;

  // Retrieves the terminal handle for standard output
  terminal_handle_ = GetStdHandle(STD_OUTPUT_HANDLE);

  // Retrieves the handle for the terminal window.
  terminal_window_ = GetConsoleWindow();

  // Retrieves the current console mode settings.
  GetConsoleMode(terminal_handle_, &terminal_mode_);

  // Enables virtual processing mode for advanced terminal features.
  EnableVirtualMode();
}

curs::internal::Terminal& curs::internal::Terminal::operator<<(
  const std::string& str) {
  // Outputs the provided string to the console.
  WriteConsoleA(terminal_handle_, str.c_str(), static_cast<DWORD>(str.length()), NULL, NULL);
  return *this;
}

curs::internal::Terminal& curs::internal::Terminal::operator<<(char ch) {
  // Outputs the provided character to the console.
  WriteConsoleA(terminal_handle_, &ch, 1, NULL, NULL);
  return *this;
}

void curs::internal::Terminal::ClearScreen() {
  // Clears the terminal screen by invoking the "cls" command.
  system("cls");
}

void curs::internal::Terminal::ResetCursor() {
  // Moves the cursor to the top-left corner (0, 0) of the terminal.
  MoveCursor(0, 0);
}

void curs::internal::Terminal::MoveCursor(const Point& cursor) {
  // Moves the cursor to the specified coordinates represented by the Point object.
  COORD new_cursor {cursor.x, cursor.y};
  SetConsoleCursorPosition(terminal_handle_, new_cursor);
}

void curs::internal::Terminal::MoveCursor(short y, short x) {
  // Moves the cursor to the specified row (y) and column (x).
  MoveCursor({y, x});
}

void curs::internal::Terminal::SetTerminalSize(SHORT height, SHORT width) { 
  // Retrieve maximum allowable terminal size
  COORD max_size = GetMaxSize();

  // Ensure the requested size is within allowable limits
  if(height <= 0 || height > max_size.Y) {
    height = max_size.Y;
  }

  if(width <= 0 || width > max_size.X) {
    width = max_size.X;
  }

  // Temporarily sets the window size to (1, 1) to allow resizing the buffer
  SMALL_RECT terminal_size = { 0, 0, 1, 1 };
  SetConsoleWindowInfo(terminal_handle_, TRUE, &terminal_size);

  // Sets the new screen buffer size.
  COORD buffer_size = { width, height };
  SetConsoleScreenBufferSize(terminal_handle_, buffer_size);

  // Sets the new window size.
  terminal_size.Right = width - 1;
  terminal_size.Bottom = height - 1;
  SetConsoleWindowInfo(terminal_handle_, TRUE, &terminal_size);
}

void curs::internal::Terminal::SetFontSettings(
    SHORT font_size,
    const std::wstring& font_name) {
  // Customizes the font settings of the terminal by defining the font size and name.
  CONSOLE_FONT_INFOEX cfi;

  cfi.cbSize = sizeof(cfi);
  cfi.nFont = 0;
  cfi.dwFontSize.X = 0;
  cfi.dwFontSize.Y = font_size;
  cfi.FontFamily = FF_DONTCARE;
  cfi.FontWeight = FW_NORMAL;
  wcscpy_s(cfi.FaceName, sizeof(L"Consolas"), font_name.c_str());

  // Applies the new font settings to the terminal.
  SetCurrentConsoleFontEx(terminal_handle_, FALSE, &cfi);
}

void curs::internal::Terminal::SetTitle(const std::string& title) {
  // Sets the title of the terminal window
  SetConsoleTitleA(title.c_str());
}

void curs::internal::Terminal::SetMaximizeButton(bool enable) {
  // Get the current properties of the terminal window.
  DWORD window_style = GetWindowLong(terminal_window_, GWL_STYLE);

  // If 'enable' is true, the maximize button is enabled; otherwise, it is disabled.
  if(enable) {
    window_style |= WS_MAXIMIZEBOX;
  } else {
    window_style &= ~WS_MAXIMIZEBOX;
  }

  // Apply new window style settings.
  SetWindowLong(terminal_window_, GWL_STYLE, window_style);

  // Refresh the window with the new style so that new styles are displayed properly.
  SetWindowPos(terminal_window_, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
}

void curs::internal::Terminal::SetWindowResizing(bool enable) {
  // Get the current properties of the terminal window.
  DWORD window_style = GetWindowLong(terminal_window_, GWL_STYLE);

  // Enables or disables window resizing based on the 'enable' flag.
  if(enable) {
    window_style |= (WS_MAXIMIZEBOX | WS_SIZEBOX);
  } else {
    window_style &= ~(WS_MAXIMIZEBOX | WS_SIZEBOX);
  }

  // Apply a new window style.
  SetWindowLong(terminal_window_, GWL_STYLE, window_style);
}

void curs::internal::Terminal::SetCursorVisible(bool visible) {
  // Gets the cursor settings.
  CONSOLE_CURSOR_INFO cursor_info;

  // Sets the cursor size to 100 (full size).
  cursor_info.dwSize = 100;
  
   // Makes the cursor visible or invisible based on the 'visible' flag.
  if(visible) {
    cursor_info.bVisible = TRUE;
    cursor_visibility_ = true;
  } else {
    cursor_info.bVisible = FALSE;
    cursor_visibility_ = false;
  } 

  // Apply the cursor settings.
  SetConsoleCursorInfo(terminal_handle_, &cursor_info);
}

void curs::internal::Terminal::RestoreTerminalMode() {
  // Restore the initial mode of the terminal
  if(SetConsoleMode(terminal_handle_, terminal_mode_)) {
    is_virtual_mode_enabled = false;
  }
}

void curs::internal::Terminal::EnableVirtualMode() {
  // Enable virtual terminal mode for additional features
  if(SetConsoleMode(
      terminal_handle_,
      terminal_mode_ | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
    is_virtual_mode_enabled = true;
  }
}

curs::Point curs::internal::Terminal::GetSize() const {
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  GetConsoleScreenBufferInfo(terminal_handle_, &csbi);

  // Height is calculated as the difference between the 
  // Bottom and Top coordinates (including 1 for correct calculation).
  // Width is calculated as the difference between the 
  // Right and Left coordinates (also with the addition of 1).
  return {static_cast<short>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1),
          static_cast<short>(csbi.srWindow.Right - csbi.srWindow.Left + 1)};
}

COORD curs::internal::Terminal::GetMaxSize() const {
  // Retrieves the maximum size of the terminal screen.
  return GetLargestConsoleWindowSize(terminal_handle_);
}

#endif  // _WIN32 