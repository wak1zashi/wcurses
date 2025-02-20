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
  #include "ncurses/ncurses.h"
#endif // _WIN32

#include <string>

#include <wcurses/Key.h>
#include <wcurses/Point.h>

namespace curs {
    
class Wcurses {
  public:

    ~Wcurses();
  
    // Creates an instance of Wcurses.
    static Wcurses& CreateWcurses();

#ifdef _WIN32
    // Initializes the terminal with the specified size (Windows only).
    void Start(Size size);
#else
    // Initializes the terminal (Unix-based systems).
    void Start();
#endif

    // Ends the terminal session.
    void End();
    
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
    short ERR() { return internal::InputManager::ERR(); }

    // Overloaded input stream operators.
    Wcurses& operator>>(int& val);
    Wcurses& operator>>(Key& val);

    // Reads the key input, returning the key code.
    int GetCh();

    // Reads a key input, returning a Key object.
    Key GetKey();

    // Enables or disables non-blocking input mode.
    void SetNoDelay(bool enable);

    // Clears any pending input.
    void FlushInput();

    // Refreshes the screen to reflect changes.
    void RefreshScreen();

    bool HasColor();

    // Initializes color support.
    void StartColor();

    // Defines a custom color.
    void InitColor(internal::ColorManager::ColorIndex color_index, const RGB& rgb);
    void InitColor(internal::ColorManager::ColorIndex color_index, short r, short g, short b);

    // Defines a color pair.
    void InitPair(internal::ColorManager::PairIndex pair_index, const ColorPair& color_pair);
    void InitPair(internal::ColorManager::PairIndex pair_index, short foreground, short background);

    // Initializes a default color pair.
    void InitDefaultPair(internal::ColorManager::PairIndex pair_index);

    // Sets the active color pair.
    void SetActivePair(internal::ColorManager::PairIndex pair_index);

    // Resets the active color pair to default (0).
    void ResetToDefaultPair();

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
    Point GetPosition() const;

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