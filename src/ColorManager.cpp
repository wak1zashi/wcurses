#include "wcurses/ColorManager.h"

#include <algorithm>

const __internal::ColorManager::PairIndex __internal::ColorManager::DEFAULT_PAIR = 0;

 std::string __internal::ColorManager::decToHex(short value)
 {
    if (value == 0) {
        return "0";
    }

    std::string hex;

    for(; value != 0 ;)
    {
        short temp = 0;

        temp = value % 16;

        if(temp < 10) {
            hex.push_back(static_cast<char>(temp + static_cast <short>('0')));
        }
        else {
            hex.push_back(static_cast<char>(temp + static_cast <short>('7')));
        }

        value /= 16;
    }

    std::reverse(hex.begin(), hex.end());
    return hex;
 }

__internal::ColorManager::ColorManager()
    : _currPair(0), _startColor(false)
{ }

bool __internal::ColorManager::startColor()
{
    _colorPairs[DEFAULT_PAIR] = { 7, 0 };
    _startColor = true;

    return true;
}

std::string __internal::ColorManager::initColor(PairIndex pair, short r, short g, short b)
{
    if (!_startColor) {
        return "";
    }

    auto isInRange = [](short color) -> bool {
        return color >= 0 && color <= 255;
    };

    if (!isInRange(r) || !isInRange(g) || !isInRange(b)) {
        return "";
    }

    std::string color = "\033]4;" + std::to_string(pair) + ";rgb:";
    color += decToHex(r) += "/";
    color += decToHex(g) += "/";
    color += decToHex(b) += "\033\\";

    return color;
}

void __internal::ColorManager::initPair(PairIndex pairIndex, short foreground, short background)
{
    if (!_startColor) {
        return;
    }

    if (pairIndex > 0 && pairIndex < 256) {
        _colorPairs[pairIndex] = { foreground, background };
    }
}

void __internal::ColorManager::initDefaultPair(PairIndex pairIndex)
{
    if (!_startColor) {
        return;
    }

    if (pairIndex == DEFAULT_PAIR) {
        return;
    }

    auto colorPair = _colorPairs.find(pairIndex);
    if (colorPair != _colorPairs.end()) {
        _colorPairs[DEFAULT_PAIR] = colorPair->second;
    }
}

void __internal::ColorManager::setActivePair(PairIndex pairIndex)
{
    if (!_startColor) {
        return; 
    }

    if (_colorPairs.find(pairIndex) != _colorPairs.end()) {
        _currPair = pairIndex;
    }
}

void __internal::ColorManager::resetToDefault()
{
    if (!_startColor) {
        return; 
    }

    _currPair = DEFAULT_PAIR;
}

std::string __internal::ColorManager::getColorCode(PairIndex pairIndex) const
{
    if (!_startColor) {
        return ""; 
    }

    std::string colorCode = "";

    const auto colorPair = _colorPairs.find(pairIndex);
    if (colorPair != _colorPairs.end()) 
    {
        colorCode += "\033[38;5;" + std::to_string(colorPair->second.foreground) + "m";
        colorCode += "\033[48;5;" + std::to_string(colorPair->second.background) + "m";
    }

    return colorCode;
}

std::string __internal::ColorManager::getColorCode(PairIndex pairIndex0, PairIndex pairIndex1) const
{
    if (!_startColor) {
        return ""; 
    }

    std::string colorCode = "";

    const auto firstPair  = _colorPairs.find(pairIndex0);
    const auto secondPair = _colorPairs.find(pairIndex1);

    if (firstPair != _colorPairs.end() && secondPair != _colorPairs.end()) 
    {
        if (firstPair->second.foreground != secondPair->second.foreground) {
            colorCode += "\033[38;5;" + std::to_string(secondPair->second.foreground) + "m";
        }

        if (firstPair->second.background != secondPair->second.background) {
            colorCode += "\033[48;5;" + std::to_string(secondPair->second.background) + "m";
        }
    }

    return colorCode;
}