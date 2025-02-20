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

#ifndef INPUT_MANAGER_H_
#define INPUT_MANAGER_H_

#include <unordered_map>
#include "key.h"

namespace curs {
namespace internal {

// Class for managing input data and converting key codes to the corresponding ncurses values.
class InputManager {
  public:
    // Returns the error value (-1).
    static int ERR() { return kErr; }

    // Clears the current state of the input.
    void Clear();

    // Sets whether the input will be processed without delay.
    // If true, the input is processed without delay.
    void NoDelay(bool enable) { no_delay_ = enable; }

    // Gets the input character.
    int GetCh();
    Key GetKey();

    InputManager& operator>>(int& key_code);
    InputManager& operator>>(Key& key_code);

  private:
    using KeyMap = std::unordered_map<short, curs::Key>;

    // Map for converting key codes to corresponding ncurses values.
    // Each element contains a key code and the corresponding Key.
    static const KeyMap fn_key_map_;

    static const KeyMap key_map_;

    // Code for function keys.
    static constexpr short kFnKey = 224;  
    static constexpr short kFKey  = 0;  

    // Error code
    static constexpr short kErr = -1;

    bool no_delay_ = false;
};

} // namespace internal
} // namespace curs

#endif // INPUT_MANAGER_H_