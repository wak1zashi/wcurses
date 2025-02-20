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

#include "wcurses/input_manager.h"

#include <conio.h>

#include <unordered_map>

#include "wcurses/key.h"

namespace curs {
namespace internal {
const InputManager::KeyMap InputManager::fn_key_map_ {
  {72,  Key::kArrowUp},
  {73,  Key::kPageUp},
  {75,  Key::kArrowRight},
  {77,  Key::kArrowLeft},
  {80,  Key::kArrowDown},
  {81,  Key::kPageDown},
  {82,  Key::kInsert},
  {83,  Key::kDelete},
  {59,  Key::kF1},
  {60,  Key::kF2},
  {61,  Key::kF3},
  {62,  Key::kF4},
  {63,  Key::kF5},
  {64,  Key::kF6},
  {65,  Key::kF7},
  {66,  Key::kF8},
  {67,  Key::kF9},
  {68,  Key::kF10},
  {133, Key::kF11},
  {134, Key::kF12},
};

const InputManager::KeyMap InputManager::key_map_ {
  {8,  Key::kBackspace},
  {9,  Key::kTab},
  {13, Key::kEnter},
  {27, Key::kEscape},
};

} // namespace internal
} // namespace curs


void curs::internal::InputManager::Clear() {
  // Keep checking if any key has been pressed, and discard all key codes
  // to make sure that there are no pending keys in the buffer.
  // This ensures that no unwanted input is carried over to the next operation.
  for (; _kbhit() ;) { (void)_getch(); }
}

// The GetCh() method reads a key press using `_getch()` and translates it into 
// an ncurses-compatible key code. This allows cross-platform compatibility between 
// Windows and Unix-like systems.
int curs::internal::InputManager::GetCh() {
  // If `no_delay_` mode is enabled and no key is pressed, return an error code.
  // `_kbhit()` checks if a key is available to read. If no key is pressed, return `kErr`.
  if (no_delay_ && !_kbhit()) {
    return kErr;
  }

  // Retrieve the raw key code.
  int key_code = _getch();

  // If the key is a printable ASCII character (from ' ' to '~'),
  // return it directly
  if (key_code >= static_cast<int>(Key::kSpace) &&
      key_code <= static_cast<int>(Key::kTilde)) {
    return key_code;
  }

  // Check if the key exists in `key_map_`, which stores mappings 
  // for common keys like Enter, Backspace, and Escape
  auto key_it = key_map_.find(key_code);
  if(key_it != key_map_.end()) {
    return static_cast<int>(key_it->second);
  }

  // Handle function keys (arrows, Insert, Delete, F1–F12).
  // Function keys are typically a two-step sequence:
  // 1. The first `_getch()` reads an escape sequence (e.g., `kFnKey == 224` or `kFKey == 0`),
  // 2. The second `_getch()` retrieves the actual key code.
  // On Windows, `kFnKey == 224` is often used for arrows and some function keys,
  // and `kFKey == 0` is used for other function keys
  if(key_code == kFnKey || key_code == kFKey) {
    key_code = _getch();

    auto fn_key_it = fn_key_map_.find(key_code);
    if (fn_key_it != fn_key_map_.end()) {
      return static_cast<int>(fn_key_it->second);
    }
  }

  return kErr;
}

curs::Key curs::internal::InputManager::GetKey() {
  return static_cast<Key>(GetCh());
}

curs::internal::InputManager& curs::internal::InputManager::operator>>(int& key_code) {
  key_code = GetCh();
  return *this;
}

curs::internal::InputManager& curs::internal::InputManager::operator>>(Key& key_code) {
  key_code = GetKey();
  return *this;
}