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

#ifndef WCURSES_TERMINAL_H_
#define WCURSES_TERMINAL_H_

#include <Windows.h>

#include <string>

#include "point.h"

namespace curs {
namespace internal {

// The Terminal class for managing a terminal on Windows.
// This class provides methods for customizing the terminal: resizing, fonts,
// cursor control, and other window parameters.
class Terminal {
  public:
    Terminal();

    // Outputs the given string to the terminal.
    Terminal& operator<<(const std::string& str);

    // Outputs the given character to the terminal.
    Terminal& operator<<(char ch);

    // Clears the terminal screen.
    void ClearScreen();

    // Resets the cursor to its default position.
    void ResetCursor();

    // Moves the cursor to the specified position through the Point structure.
    void MoveCursor(const Point& cursor);

    // Use the MoveCursor method with two parameters (y, x) for convenience.
    void MoveCursor(short y, short x);

    // Sets the terminal size with the given height and width.
    void SetTerminalSize(SHORT height, SHORT width);

    // Sets the terminal font size and font name.
    void SetFontSettings(SHORT font_size, const std::wstring& font_name);

    // Sets the terminal window title.
    void SetTitle(const std::string& title);

    // Enables or disables the window maximization button
    void SetMaximizeButton(bool enable);

    // Enables or disables the ability to resize the window
    void SetWindowResizing(bool enable);

    // Sets the visibility of the cursor
    void SetCursorVisible(bool visible);

    // Restores the standard console mode
    void RestoreTerminalMode();

    // Enables virtual terminal mode, which provides 
    // support for escaping sequences for color change
    void EnableVirtualMode();
    
    // Returns the current size of the terminal
    // Returns a Point structure with the current height and width
    Point GetSize() const;

    // Returns the maximum size of the terminal
    // Returns a COORD structure with the maximum height and width
    COORD GetMaxSize() const;
   
    bool GetCursorVisible() const { return cursor_visibility_; }
    bool IsVirtualModeEnabled() const { return is_virtual_mode_enabled; }

  private:
    HANDLE terminal_handle_;
    HWND terminal_window_;
    DWORD terminal_mode_;

    bool cursor_visibility_;
    bool is_virtual_mode_enabled;
    
}; 

} // namespace internal
} // namespace curs

#endif // WCURSES_TERMINAL_H_
#endif // _WIN32