#include "wcurses/ColorManager.h"

#include <sstream>

const __internal::ColorManager::PairIndex __internal::ColorManager::DEFAULT_PAIR = 0;

__internal::ColorManager::ColorManager()
    : _currPair(0), _startColor(false)
{ }

bool __internal::ColorManager::startColor()
{
    _colorPairs[DEFAULT_PAIR] = { 7, 0 };
    _startColor = true;

    return true;
}

void __internal::ColorManager::initColor(PairIndex pair, short r, short g, short b)
{
    if (!_startColor) {
        return;
    }

    auto isInRange = [](short color) -> bool {
        return color >= 0 && color <= 255;
    };

    if (!isInRange(r) || !isInRange(g) || !isInRange(b)) {
        return;
    }

    _customColor[pair] = {r, g, b};
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

    const auto colorPair = _colorPairs.find(pairIndex);
    if (colorPair == _colorPairs.end()) {
        return "";
    }
    
    std::ostringstream colorCode;

    const auto colorIndex0 = _customColor.find(colorPair->second.foreground);
    if (colorIndex0 != _customColor.end()) 
    {
        colorCode << "\033[38;2;"
        << colorIndex0->second.r << ";"
        << colorIndex0->second.g << ";"
        << colorIndex0->second.b << "m";
    }
    else {
        colorCode << "\033[38;5;" << colorPair->second.foreground << "m";
    }

    const auto colorIndex1 = _customColor.find(colorPair->second.background);
    if (colorIndex1 != _customColor.end()) 
    {
        colorCode << "\033[48;2;"
        << colorIndex1->second.r << ";"
        << colorIndex1->second.g << ";"
        << colorIndex1->second.b << "m";
    }
    else {
        colorCode << "\033[48;5;" << colorPair->second.background << "m";
    }

    return colorCode.str();
}

std::string __internal::ColorManager::getColorCode(PairIndex pairIndex0, PairIndex pairIndex1) const
{
    if (!_startColor) {
        return ""; 
    }

    const auto firstPair  = _colorPairs.find(pairIndex0);
    const auto secondPair = _colorPairs.find(pairIndex1);

    if (firstPair == _colorPairs.end() && secondPair == _colorPairs.end())  {
        return ""; 
    }

    std::ostringstream colorCode;
    
    if (firstPair->second.foreground != secondPair->second.foreground) 
    {
        const auto colorIndex0 = _customColor.find(secondPair->second.foreground);
        if (colorIndex0 != _customColor.end()) 
        {
            colorCode << "\033[38;2;"
            << colorIndex0->second.r << ";"
            << colorIndex0->second.g << ";"
            << colorIndex0->second.b << "m";
        }
        else {
            colorCode << "\033[38;5;" << secondPair->second.foreground << "m";
        }
    }

    if (firstPair->second.background != secondPair->second.background) 
    {
        const auto colorIndex1 = _customColor.find(secondPair->second.background);
        if (colorIndex1 != _customColor.end()) 
        {
            colorCode << "\033[48;2;"
            << colorIndex1->second.r << ";"
            << colorIndex1->second.g << ";"
            << colorIndex1->second.b << "m";
        }
        else {
            colorCode << "\033[48;5;" << secondPair->second.background << "m";
        }

    }

    return colorCode.str();
}