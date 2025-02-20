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

#ifndef WCURSES_COLOR_MANAGER_H_
#define WCURSES_COLOR_MANAGER_H_

#include <unordered_map>
#include <string>
#include <sstream>

namespace curs {

struct ColorPair {
  short foreground = 0;
  short background = 0;
};

struct RGB {
  short red;
  short green;
  short blue;
};

namespace internal {

enum class ColorType {
  Text = 38,      // ANSI code for text color
  Background = 48 // ANSI code for background color
};

// The ColorManager class is responsible for color management: storing color pairs and custom colors,
// as well as converting them into escape sequences for changing colors in the terminal.
class ColorManager {
  public:
    // Types for color indices and color pairs 
    using PairIndex = short;
    using ColorIndex = short;

    // Maps for storing color pairs and custom colors.
    using ColorMap = std::unordered_map<PairIndex, ColorPair>;
    using CustomColorMap = std::unordered_map<ColorIndex, RGB>;

    ColorManager();

    // Initializes the color system
    void StartColor();
    
    // Initializes a custom color by adding a new value to custom_color_.
    // color_index must be in the range [0, 255].
    void InitColor(ColorIndex color_index, const RGB& rgb);
    void InitColor(ColorIndex color_index, short r, short g, short b);

    // Initializes a color pair in the range [1, 255] and adds it to color_pairs_.
    void InitPair(PairIndex pair_index, const ColorPair& color_pair);
    void InitPair(PairIndex pair_index, short foreground, short background);

    // Initializes the default color pair with index 0.
    void InitDefaultPair(PairIndex pair_index);

    // Sets the current active color pair.
    void SetActivePair(PairIndex pair_index);

    // Resets the current color pair to the default (index 0)..
    void ResetToDefault();

    // Returns the escape code to reset all colors
    std::string GetResetCode() { return "\033[0m"; }

    // Returns the escape code to change the color for the given pair index.
    // It uses the 256 color palette or custom colors if available
    std::string MakeColorCode (PairIndex pair_index) const;

    // Returns the escape code for two color pairs:
    // if the pairs are the same, it returns an empty string;
    // if the pairs are different, it returns the escape sequence for what differs.
    std::string MakeColorCode (PairIndex prev_pair_index, PairIndex new_pair_index) const;

    // Getter methods
    bool IsStartedColor() const {return start_color_; }
    PairIndex GetActivePair() const { return current_pair_; }
    static PairIndex GetDefaultPair() { return kDefaultPair; }

  private:
    ColorMap color_pairs_map_;
    CustomColorMap custom_colors_map_;
    PairIndex current_pair_;
    bool start_color_;

    static constexpr PairIndex kDefaultPair = 0;

    // Adds a sequence to the ANSI string for a 256-color palette.
    void MakeEscapeSequence256Color(
        std::ostringstream& color_code,
        ColorType color_type, 
        short color_index) const;

    // Adds a sequence for an RGB color to an ANSI string.
    void MakeEscapeSequenceRGB(
        std::ostringstream& color_code,
        ColorType color_type,
        const RGB& rbg) const;
};

} // namespace internal
} // namespace curs

#endif // WCURSES_COLOR_MANAGER_H_