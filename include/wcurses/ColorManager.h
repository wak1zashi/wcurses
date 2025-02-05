#pragma once

#include <unordered_map>
#include <string>

namespace __internal
{
    struct ColorPair
	{
		short foreground = 0;
		short background = 0;
	};

    struct RGB
	{
		short r;
		short g;
        short b;
	};


    class ColorManager
    {
    public:
        using PairIndex = short;
        using ColorIndex = short;

        using ColorMap = std::unordered_map<PairIndex, ColorPair>;
        using CustomColorMap = std::unordered_map<ColorIndex, RGB>;

    private:
        ColorMap _colorPairs;
        CustomColorMap _customColor;
        
        PairIndex _currPair;
        
        bool _startColor;
        
    public:
        static const PairIndex DEFAULT_PAIR;

        // ====================================================================
	    // =========================== Constructors ===========================
        // ====================================================================

        ColorManager();

        // ====================================================================
	    // ========================= Color Management =========================
        // ====================================================================

        bool startColor();
        
        void initColor(PairIndex pair, short r, short g, short b);
        void initPair(PairIndex pairIndex, short foreground, short background);

        void initDefaultPair(PairIndex pairIndex);

        void setActivePair(PairIndex pairIndex);
        void resetToDefault();

        // ====================================================================
	    // ========================== Getter Methods ==========================
        // ====================================================================
       
        std::string getResetCode() { return "\033[0m"; }
        std::string getColorCode(PairIndex pairIndex) const;
        std::string getColorCode(PairIndex pairIndex0, PairIndex pairIndex1) const;

        bool isStartedColor() const {return _startColor; }

        PairIndex getActivePair() const { return _currPair; }
    };
} // namespace __internal