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

    class ColorManager
    {
    public:
        using PairIndex = short;
        using ColorMap = std::unordered_map<PairIndex, ColorPair>;
    private:
        ColorMap _colorPairs;
        PairIndex _currPair;
        bool _startColor;

        // ====================================================================
	    // ========================= Support Methods ==========================
        // ====================================================================

        std::string decToHex(short value);
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
        
        std::string initColor(PairIndex pair, short r, short g, short b);
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