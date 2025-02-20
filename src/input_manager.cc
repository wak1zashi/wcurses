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