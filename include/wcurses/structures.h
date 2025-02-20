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

#ifndef WCURSES_STRUCTURES_H_
#define WCURSES_STRUCTURES_H_

namespace curs {
  struct Size {
    short rows;
    short cols;
  };
  
  struct ColorPair {
    short foreground = 0;
    short background = 0;
  };
  
  struct RGB {
    short red;
    short green;
    short blue;
  };
} // namespace curs

#endif // WCURSES_STRUCTURES_H_