#pragma once
#include "Buffer.h"
#include "ColorManager.h"
#include "InputManager.h"
#include "Terminal.h"
#include <sstream>

namespace waki
{
    class Wcurses
    {
    private:
        __internal::Buffer* _buffer;
        __internal::ColorManager* _color;
        __internal::InputManager* _cin;
        __internal::Terminal* _term;

        std::streambuf* _originalCoutBuffer;
        std::streambuf* _originalCinBuffer;

        std::stringstream _dummyStream;

        bool _init;

        Wcurses();
    public:
        // =========================================================================
	    // ============================= Constructors ==============================
        // =========================================================================

        ~Wcurses();

        static Wcurses& createWcurses();

        void initscr(SizeT size);

        void endwin();
        
        // =========================================================================
        // =============================== Operators ===============================
        // =========================================================================

        Wcurses& operator<<(char ch);
        Wcurses& operator<<(const char* str);
        Wcurses& operator<<(const std::string& str);

        Wcurses& operator<<(short val);
        Wcurses& operator<<(int val);
        Wcurses& operator<<(long val);
        Wcurses& operator<<(long long val);

        Wcurses& operator<<(float val);
        Wcurses& operator<<(double val);
        Wcurses& operator<<(long double val);

        Wcurses& operator>>(int val);
        Wcurses& operator>>(Key val);

        int getch();

        // =========================================================================
	    // ========================== Wcurses Managemen ============================
        // =========================================================================

        void refresh();

        void startColor();

        void initColor(short pair, short r, short g, short b);
        void initPair(short pair, short fg, short bg);
        void initDefaultPair(short pair);

        void setActivePair(short pair);
        void resetToDefaultPair();

        void setTitle(const std::string& title);
        void setFontSettings(SHORT fontSize, const std::wstring& fontName);
        
        // =========================================================================
        // =========================== Movement Methods ============================
        // =========================================================================

        void move(short y, short x);
        void moveBy(short y, short x);
        
        Point getYX() const;

        void clear();

        void cursSet(int visibility);
    };    

    extern Wcurses& curses;
} // namespace waki