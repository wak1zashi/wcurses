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


// Wcurses - Cross-platform library for terminal handling.
//
// The Wcurses library implements partial ncurses functionality for Windows, allowing 
// the creation of text-based interfaces in the terminal. On Unix systems, it acts as a 
// wrapper around ncurses, providing a unified interface for terminal operations.  
//
// Features:
// - On Windows, the library uses <windows.h> to interact with the terminal API and 
//   applies the necessary settings. Avoid calling <windows.h> functions that modify 
//   the terminal configuration to prevent incorrect library behavior.
// - The library does not work correctly in Microsoft Windows Terminal due to restricted 
//   access to terminal management APIs.
// - Provides a consistent interface for terminal handling across Windows and Unix systems.

#ifndef WCURSES_H_
#define WCURSES_H_

#ifdef _WIN32
  #include <sstream>

  #include "buffer.h"
  #include "color_manager.h"
  #include "input_manager.h"
  #include "terminal.h"
#else
  #include <ncurses.h>
#endif // _WIN32

#include <string>

#include "wcurses/key.h"
#include "wcurses/point.h"
#include "wcurses/structures.h"

namespace curs {

class Wcurses {
  public:

    ~Wcurses();
  
    // Creates an instance of Wcurses.
    static Wcurses& CreateWcurses();

#ifdef _WIN32
    // Initializes the library and optionally sets the terminal size (Windows only).
    void Initscr(Size size);
    void Initscr(short rows, short cols) { Initscr({rows, cols}); }
#else
    // Initializes the library (Unix-based systems).
    void Initscr();

    // These methods ensure a unified interface for Windows and Linux.
    // On Linux, they simply initialize the library without changing the terminal size.
    void Initscr(Size) { Initscr(); }
    void Initscr(short, short) { Initscr(); }
#endif

    // Ends the terminal session.
    void Endwin();
    
    // Overloaded output stream operators for various types.
    Wcurses& operator<<(char ch);
    Wcurses& operator<<(const char* str);
    Wcurses& operator<<(const std::string& str);
    Wcurses& operator<<(short val);
    Wcurses& operator<<(int val);
    Wcurses& operator<<(long val);
    Wcurses& operator<<(long long val);
    Wcurses& operator<<(unsigned short val);
    Wcurses& operator<<(unsigned val);
    Wcurses& operator<<(unsigned long val);
    Wcurses& operator<<(unsigned long long val);
    Wcurses& operator<<(float val);
    Wcurses& operator<<(double val);
    Wcurses& operator<<(long double val);
    Wcurses& operator<<(Wcurses& (*pf)(Wcurses&));

    // Returns the error value (-1).
    #ifdef _WIN32
      short Err() { return internal::InputManager::Err(); }
    #else
      short Err() { return ERR; }
    #endif

    // Overloaded input stream operators.
    Wcurses& operator>>(int& val);
    Wcurses& operator>>(Key& val);

    // Reads the key input, returning the key code.
    int GetCh();

    // Reads a key input, returning a Key object.
    Key GetKey();

    // Enables or disables non-blocking input mode.
    void Nodelay(bool enable);

    // Clears any pending input.
    void FlushInput();

    // Refreshes the screen to reflect changes.
    void Refresh();

    bool HasColor();

    // Initializes color support.
    void StartColor();

    // Defines a custom color.
    void InitColor(short color_index, const RGB& rgb);
    void InitColor(short color_index, short r, short g, short b);

    // Defines a color pair.
    void InitPair(short pair_index, const ColorPair& color_pair);
    void InitPair(short pair_index, short foreground, short background);

    // Initializes a default color pair.
    void BkGd(short pair_index);

    // Sets the active color pair.
    void Attron(short pair_index);

    // Resets the active color pair to default.
    void Attroff();

    // Pauses execution for a given number of milliseconds.
    void Sleep(unsigned milliseconds);

#ifdef _WIN32
    // Sets the terminal window title (Windows only).
    void SetTitle(const std::string& title);

    // Configures font settings (Windows only).
    void SetFontSettings(SHORT fontSize, const std::wstring& fontName);
#endif

    // Moves the cursor to the specified coordinates.
    void MoveTo(short y, short x);

    // Moves the cursor by the given offset.
    void MoveBy(short y, short x);
    
    // Returns the current cursor position.
    Point Getyx() const;

    // Clears the screen.
    void ClearScreen();

    // Sets cursor visibility
    void SetCursorVisibility(int visibility);

  private:
#ifdef _WIN32
    internal::Terminal* terminal_ = nullptr;
    internal::Buffer* buffer_ = nullptr;
    internal::InputManager* input_manager_ = nullptr;

    std::streambuf* original_cout_buffer_ = nullptr;
    std::streambuf* original_cin_buffer_ = nullptr;
    std::stringstream dummy_stream_;

    bool was_initialized_ = false;
#endif

  // Private constructor to enforce singleton pattern.
  Wcurses() = default;

  // Delete copy and move constructors.
  Wcurses(const Wcurses&) = delete;
  Wcurses(Wcurses&&) = delete;
  Wcurses& operator=(const Wcurses&) = delete;
  Wcurses& operator=(Wcurses&&) = delete;
};    

// Outputs a newline to the Wcurses instance.
Wcurses& Endl(Wcurses& wcurses);

// A global instance of the Wcurses class.  
// Provides convenient access to the object from anywhere in the program.
extern Wcurses& wcurses;

} // namespace curs

#endif // WCURSES_H_