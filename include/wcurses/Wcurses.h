#pragma once

#ifdef _WIN32
    #include "Buffer.h"
    #include "ColorManager.h"
    #include "InputManager.h"
    #include "Terminal.h"
    #include <sstream>
#else
    #include <string>
    #include "ncurses/ncurses.h"    
    #include <wcurses/Key.h>
    #include <wcurses/Point.h>
#endif
namespace waki
{
    class Wcurses
    {
    private:

    #ifdef _WIN32
        __internal::Buffer* _buffer;
        __internal::ColorManager* _color;
        __internal::InputManager* _cin;
        __internal::Terminal* _term;

        std::streambuf* _originalCoutBuffer;
        std::streambuf* _originalCinBuffer;

        std::stringstream _dummyStream;

        bool _init;
    #endif

    #ifdef _WIN32
        Wcurses();
    #else
        Wcurses() = default;
    #endif

        Wcurses(Wcurses&) = delete;
        Wcurses(Wcurses&&) = delete;

        Wcurses& operator=(Wcurses&) = delete;
        Wcurses& operator=(Wcurses&&) = delete;

    public:
        // ====================================================================
	    // =========================== Constructors ===========================
        // ====================================================================

        ~Wcurses();

        static Wcurses& createWcurses();

    #ifdef _WIN32
        void start(SizeT size);
    #else
        void start();
    #endif

        void end();
        
        // ====================================================================
        // ============================ Operators =============================
        // ====================================================================

        Wcurses& operator<<(char ch);
        Wcurses& operator<<(const char* str);
        Wcurses& operator<<(const std::string& str);

        Wcurses& operator<<(short val);
        Wcurses& operator<<(int val);
        Wcurses& operator<<(long val);
        Wcurses& operator<<(long long val);

        Wcurses& operator<<(unsigned short val);
        Wcurses& operator<<(unsigned val);
        Wcurses& operator<<(unsigned long val);
        Wcurses& operator<<(unsigned long long val);

        Wcurses& operator<<(float val);
        Wcurses& operator<<(double val);
        Wcurses& operator<<(long double val);

        Wcurses& operator<<(Wcurses& (*pf)(Wcurses&));

        Wcurses& operator>>(int& val);
        Wcurses& operator>>(Key& val);

        int getCh();
        Key getKey();

        void noDelay(bool enable);
        
        void flushInput();

        // ====================================================================
	    // ======================== Wcurses Managemen =========================
        // ====================================================================

        void refreshScreen();

        void startColor();

        void initColor(short pair, short r, short g, short b);
        void initPair(short pair, short fg, short bg);
        void initDefaultPair(short pair);

        void setActivePair(short pair);
        void resetToDefaultPair();

        void sleep(unsigned milliseconds);

    #ifdef _WIN32
        void setTitle(const std::string& title);
        void setFontSettings(SHORT fontSize, const std::wstring& fontName);
    #endif

        // ====================================================================
        // ========================= Movement Methods =========================
        // ====================================================================

        void moveTo(short y, short x);
        void moveBy(short y, short x);
        
        Point getPos() const;

        void clearScreen();

        void cursSet(int visibility);
    };    

    Wcurses& endl(Wcurses& wcurses);

    extern Wcurses& wcurses;
} // namespace waki>