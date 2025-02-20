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

#include "wcurses/color_manager.h"

#include <unordered_map>
#include <string>
#include <sstream>

curs::internal::ColorManager::ColorManager()
  : current_pair_(0), start_color_(false) { }

void curs::internal::ColorManager::StartColor() {
  if(start_color_) {
    return;
  }

  // Initializing a standard color pair (white text on a black background)
  color_pairs_map_[kDefaultPair] = { 7, 0 };
  start_color_ = true;
}

void curs::internal::ColorManager::InitColor(ColorIndex color_index, const RGB& rgb) {
  if (!start_color_) {
    return; 
  }

  // Lambda function to check if the value is within the valid range (0-255)
  auto isInRange = [](short color) -> bool { 
    return color >= 0 && color <= 255; 
  };

  // Check if the color values are within the acceptable range
  if (!isInRange(rgb.red) || !isInRange(rgb.green) || !isInRange(rgb.blue)) {
    return;
  }

  // Save the custom color to the map
  custom_colors_map_[color_index] = {rgb.red, rgb.green, rgb.blue};
}

void curs::internal::ColorManager::InitColor(
    ColorIndex color_index,
    short r,
    short g,
    short b) {
  InitColor(color_index, {r, g, b});
}

void curs::internal::ColorManager::InitPair(PairIndex pair_index,
                                            const ColorPair& color_pair) {
  if (!start_color_) {
    return;
  }
  
  if (pair_index > 0 && pair_index <= 255) {
    color_pairs_map_[pair_index] = color_pair;
  }
}

void curs::internal::ColorManager::InitPair(PairIndex pair_index, short foreground,
                                            short background) {
  InitPair(pair_index, {foreground, background});
}

void curs::internal::ColorManager::InitDefaultPair(PairIndex pair_index) {
  if (!start_color_) {
    return;
  }

  if (pair_index == kDefaultPair) {
    return;
  }

  auto color_pair = color_pairs_map_.find(pair_index);
  
  // If a color pair exists, set it as standard
  if (color_pair != color_pairs_map_.end()) {
    color_pairs_map_[kDefaultPair] = color_pair->second;
  }
}

void curs::internal::ColorManager::SetActivePair(PairIndex pair_index) {
  if (!start_color_) {
    return;
  }

  // If a color pair exists, set it as active
  if (color_pairs_map_.find(pair_index) != color_pairs_map_.end()) {
    current_pair_ = pair_index;
  }
}

void curs::internal::ColorManager::ResetToDefault() {
  if (!start_color_) {
    return;
  }

  current_pair_ = kDefaultPair;
}

std::string curs::internal::ColorManager::MakeColorCode (PairIndex pair_index) const {
  if (!start_color_) {
    return " ";
  }

  const auto color_pair = color_pairs_map_.find(pair_index);
  if (color_pair == color_pairs_map_.end()) {
    return ""; // If there is no color pair, return an empty string
  }

  std::ostringstream color_code;

  // Add an ESC sequence for the text color
  const auto color_index_fg = custom_colors_map_.find(color_pair->second.foreground);
  
  if (color_index_fg != custom_colors_map_.end()) {
    MakeEscapeSequenceRGB(color_code, ColorType::Text, color_index_fg->second);
  } else {
    MakeEscapeSequence256Color(color_code, ColorType::Text, color_pair->second.foreground);
  }

  // Add an ESC sequence for the background color
  const auto color_index_bg = custom_colors_map_.find(color_pair->second.background);

  if (color_index_bg != custom_colors_map_.end()) {
    MakeEscapeSequenceRGB(color_code, ColorType::Background, color_index_bg->second);
  } else {
    MakeEscapeSequence256Color(color_code, ColorType::Background, color_pair->second.background);
  }

  return color_code.str();
}

std::string curs::internal::ColorManager::MakeColorCode (
    PairIndex prev_pair_index,
    PairIndex new_pair_index) const {
  if (!start_color_) {
    return "";
  }

  const auto first_pair  = color_pairs_map_.find(prev_pair_index);
  const auto second_pair = color_pairs_map_.find(new_pair_index);

  // If at least one of the pairs is missing, return an empty string
  if (first_pair == color_pairs_map_.end() && second_pair == color_pairs_map_.end())  {
    return ""; 
  }

  std::ostringstream color_code;

  // If the text color is different, update it
  if (first_pair->second.foreground != second_pair->second.foreground) {
    const auto color_index_fg = custom_colors_map_.find(second_pair->second.foreground);

    if (color_index_fg != custom_colors_map_.end()) {
      MakeEscapeSequenceRGB(color_code, ColorType::Text, color_index_fg->second);
    } else {
      MakeEscapeSequence256Color(color_code, ColorType::Text, second_pair->second.foreground);
    }
  }

  // If the background has changed, update it
  if (first_pair->second.background != second_pair->second.background) {
    const auto color_index_bg = custom_colors_map_.find(second_pair->second.background);

    if (color_index_bg != custom_colors_map_.end()) {
      MakeEscapeSequenceRGB(color_code, ColorType::Background, color_index_bg->second);
    } else {
      MakeEscapeSequence256Color(color_code, ColorType::Background, second_pair->second.background);
    }
  }

  return color_code.str();
}

void curs::internal::ColorManager::MakeEscapeSequence256Color(
    std::ostringstream& color_code,
    ColorType color_type, 
    short color_index) const {
  // Generates an ANSI escape sequence for setting
  // text or background color using the 256-color palette.
  // Example format: "\033[<38/48>;5;<color_index>m"

  // Only process valid color types (Text or Background).
  if(color_type != ColorType::Text &&
     color_type != ColorType::Background) {
   return;
  }
 
  // Append ANSI escape sequence prefix and color code
  color_code << "\033["
  << static_cast<short>(color_type) // 38 sets text color, 48 sets background color 
  << ";5;"                          // '5' indicates the 256-color palette
  << color_index                    // Color index in the palette
  << "m";                           // 'm' marks the end of the sequence
}

void curs::internal::ColorManager::MakeEscapeSequenceRGB(
    std::ostringstream& color_code,
    ColorType color_type,
    const RGB& rbg) const {
  // Generates an ANSI escape sequence for setting 
  // text or background color using RGB values.
  // Example format: "\033[<38/48>;2;<red>;<green>;<blue>m"

  // Only process valid color types (Text or Background)..
  if(color_type != ColorType::Text &&
     color_type != ColorType::Background) {
    return;
  }

  // Append ANSI escape sequence prefix and color code
  color_code << "\033[" 
  << static_cast<short>(color_type) // 38 for text color, 48 for background color
  << ";2;"                          // '2' indicates RGB color format
  << rbg.red << ";"                 // Red component
  << rbg.green << ";"               // Green component
  << rbg.blue                       // Blue component
  << "m";                           // 'm' marks the end of the sequence
}