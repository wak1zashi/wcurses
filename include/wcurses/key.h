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

#ifndef WCURSES_KEY_H_
#define WCURSES_KEY_H_

namespace curs {

enum class Key {
  kError        = -1,  // Error code
  kTab          = 9,   // Tab key ('TAB')
  kEnter        = 10,  // Enter key ('\n')
  kEscape       = 27,  // Escape key ('ESC')
  kSpace        = 32,  // Spacebar (' ')
  kExclamation  = 33,  // Exclamation mark ('!')
  kDoubleQuote  = 34,  // Double quote ('"')
  kHash         = 35,  // Hash symbol ('#')
  kDollar       = 36,  // Dollar sign ('$')
  kPercent      = 37,  // Percent sign ('%')
  kAmpersand    = 38,  // Ampersand ('&')
  kSingleQuote  = 39,  // Single quote ('\'')
  kLeftParen    = 40,  // Left parenthesis ('(')
  kRightParen   = 41,  // Right parenthesis (')')
  kAsterisk     = 42,  // Asterisk ('*')
  kPlus         = 43,  // Plus sign ('+')
  kComma        = 44,  // Comma (',')
  kMinus        = 45,  // Minus sign ('-')
  kPeriod       = 46,  // Period ('.')
  kSlash        = 47,  // Slash ('/')
  kDigit0       = 48,  // Digit '0'
  kDigit1       = 49,  // Digit '1'
  kDigit2       = 50,  // Digit '2'
  kDigit3       = 51,  // Digit '3'
  kDigit4       = 52,  // Digit '4'
  kDigit5       = 53,  // Digit '5'
  kDigit6       = 54,  // Digit '6'
  kDigit7       = 55,  // Digit '7'
  kDigit8       = 56,  // Digit '8'
  kDigit9       = 57,  // Digit '9'
  kColon        = 58,  // Colon (':')
  kSemicolon    = 59,  // Semicolon (';')
  kLessThan     = 60,  // Less than ('<')
  kEquals       = 61,  // Equals sign ('=')
  kGreaterThan  = 62,  // Greater than ('>')
  kQuestion     = 63,  // Question mark ('?')
  kAtSign       = 64,  // At sign ('@')
  kUpperA       = 65,  // Uppercase 'A'
  kUpperB       = 66,  // Uppercase 'B'
  kUpperC       = 67,  // Uppercase 'C'
  kUpperD       = 68,  // Uppercase 'D'
  kUpperE       = 69,  // Uppercase 'E'
  kUpperF       = 70,  // Uppercase 'F'
  kUpperG       = 71,  // Uppercase 'G'
  kUpperH       = 72,  // Uppercase 'H'
  kUpperI       = 73,  // Uppercase 'I'
  kUpperJ       = 74,  // Uppercase 'J'
  kUpperK       = 75,  // Uppercase 'K'
  kUpperL       = 76,  // Uppercase 'L'
  kUpperM       = 77,  // Uppercase 'M'
  kUpperN       = 78,  // Uppercase 'N'
  kUpperO       = 79,  // Uppercase 'O'
  kUpperP       = 80,  // Uppercase 'P'
  kUpperQ       = 81,  // Uppercase 'Q'
  kUpperR       = 82,  // Uppercase 'R'
  kUpperS       = 83,  // Uppercase 'S'
  kUpperT       = 84,  // Uppercase 'T'
  kUpperU       = 85,  // Uppercase 'U'
  kUpperV       = 86,  // Uppercase 'V'
  kUpperW       = 87,  // Uppercase 'W'
  kUpperX       = 88,  // Uppercase 'X'
  kUpperY       = 89,  // Uppercase 'Y'
  kUpperZ       = 90,  // Uppercase 'Z'
  kLeftBracket  = 91,  // Left bracket ('[')
  kBackslash    = 92,  // Backslash ('\\')
  kRightBracket = 93,  // Right bracket (']')
  kCaret        = 94,  // Caret ('^')
  kUnderscore   = 95,  // Underscore ('_')
  kBacktick     = 96,  // Backtick ('`')
  kLowerA       = 97,  // Lowercase 'a'
  kLowerB       = 98,  // Lowercase 'b'
  kLowerC       = 99,  // Lowercase 'c'
  kLowerD       = 100, // Lowercase 'd'
  kLowerE       = 101, // Lowercase 'e'
  kLowerF       = 102, // Lowercase 'f'
  kLowerG       = 103, // Lowercase 'g'
  kLowerH       = 104, // Lowercase 'h'
  kLowerI       = 105, // Lowercase 'i'
  kLowerJ       = 106, // Lowercase 'j'
  kLowerK       = 107, // Lowercase 'k'
  kLowerL       = 108, // Lowercase 'l'
  kLowerM       = 109, // Lowercase 'm'
  kLowerN       = 110, // Lowercase 'n'
  kLowerO       = 111, // Lowercase 'o'
  kLowerP       = 112, // Lowercase 'p'
  kLowerQ       = 113, // Lowercase 'q'
  kLowerR       = 114, // Lowercase 'r'
  kLowerS       = 115, // Lowercase 's'
  kLowerT       = 116, // Lowercase 't'
  kLowerU       = 117, // Lowercase 'u'
  kLowerV       = 118, // Lowercase 'v'
  kLowerW       = 119, // Lowercase 'w'
  kLowerX       = 120, // Lowercase 'x'
  kLowerY       = 121, // Lowercase 'y'
  kLowerZ       = 122, // Lowercase 'z'
  kLeftBrace    = 123, // Left brace ('{')
  kPipe         = 124, // Pipe ('|')
  kRightBrace   = 125, // Right brace ('}')
  kTilde        = 126, // Tilde ('~')
  kArrowDown    = 258, // Arrow Down
  kArrowUp      = 259, // Arrow Up
  kArrowLeft    = 260, // Arrow Left
  kArrowRight   = 261, // Arrow Right
  kBackspace    = 263, // Backspace
  kF1           = 265, // Function key F1
  kF2           = 266, // Function key F2
  kF3           = 267, // Function key F3
  kF4           = 268, // Function key F4
  kF5           = 269, // Function key F5
  kF6           = 270, // Function key F6
  kF7           = 271, // Function key F7
  kF8           = 272, // Function key F8
  kF9           = 273, // Function key F9
  kF10          = 274, // Function key F10
  kF11          = 275, // Function key F11
  kF12          = 276, // Function key F12
  kDelete       = 330, // Delete key
  kInsert       = 331, // Insert key
  kPageDown     = 338, // Page Down
  kPageUp       = 339, // Page Up
};

} // namespace curs

#endif // WCURSES_KEY_H_