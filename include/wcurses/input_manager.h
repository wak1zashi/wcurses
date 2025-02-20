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

#ifndef INPUT_MANAGER_H_
#define INPUT_MANAGER_H_

#include <unordered_map>
#include "key.h"

namespace curs {
namespace internal {

// Class for managing input data and converting key codes to the corresponding ncurses values.
class InputManager {
  public:
    // Returns the error value.
    static int Err() { return kErr; }

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