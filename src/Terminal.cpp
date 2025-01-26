#include "wcurses/Terminal.h"

#ifdef _WIN32
    #include <stdexcept>

    __internal::Terminal::Terminal()
    {
        _tHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        _tWindow = GetConsoleWindow();
        
        GetConsoleMode(_tHandle, &_tMode);

        enableVirtualMode();
    }

    __internal::Terminal& __internal::Terminal::operator<<(const std::string& str)
    {
        WriteConsoleA(_tHandle, str.c_str(), str.length(), NULL, NULL);
        return *this;
    }

    __internal::Terminal& __internal::Terminal::operator<<(char ch)
    {
        WriteConsoleA(_tHandle, &ch, 1, NULL, NULL);
        return *this;
    }

    void __internal::Terminal::clearScreen()
    {
    system("cls");
    }

    void __internal::Terminal::stopFor(unsigned milliseconds)
    {
        Sleep(milliseconds);
    }

    void __internal::Terminal::resetCursor()
    {
        moveCursor(0, 0);
    }

    void __internal::Terminal::moveCursor(short y, short x)
    {   
        COORD newPosition {x, y};
        SetConsoleCursorPosition(_tHandle, newPosition);
    }

    bool __internal::Terminal::setTerminalSize(SHORT height, SHORT width)
    {
        COORD maxSize = getMaxSizeTerminal();

        if(height <= 0 || height > maxSize.Y || 
        width  <= 0 || width  > maxSize.X)  {
            return false;
        }

        SMALL_RECT terminalSize = { 0, 0, 1, 1 };
        SetConsoleWindowInfo(_tHandle, TRUE, &terminalSize);

        COORD bufferSize = { width, height };
        SetConsoleScreenBufferSize(_tHandle, bufferSize);

        terminalSize.Right = width - 1;
        terminalSize.Bottom = height - 1;
        SetConsoleWindowInfo(_tHandle, TRUE, &terminalSize);

        return true;
    }

    void __internal::Terminal::setFontSettings(SHORT fontSize, const std::wstring& fontName)
    {
        CONSOLE_FONT_INFOEX cfi;

        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 0;
        cfi.dwFontSize.Y = fontSize;
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        wcscpy_s(cfi.FaceName, sizeof(L"Consolas"), fontName.c_str());

        SetCurrentConsoleFontEx(_tHandle, FALSE, &cfi);
    }

    void __internal::Terminal::setTitle(const std::string& title)
    {
        SetConsoleTitleA(title.c_str());
    }

    void __internal::Terminal::setMaximizeButton(bool enable)
    {
        DWORD windowStyle = GetWindowLong(_tWindow, GWL_STYLE);

        if(enable) {
            windowStyle |= WS_MAXIMIZEBOX;
        }
        else {
            windowStyle &= ~WS_MAXIMIZEBOX;
        }

        SetWindowLong(_tWindow, GWL_STYLE, windowStyle);
        SetWindowPos(_tWindow, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
    }

    void __internal::Terminal::setWindowResizing(bool enable)
    {
        DWORD windowStyle = GetWindowLong(_tWindow, GWL_STYLE);

        if(enable) {
            windowStyle |= (WS_MAXIMIZEBOX | WS_SIZEBOX);
        }
        else {
            windowStyle &= ~(WS_MAXIMIZEBOX | WS_SIZEBOX);
        }

        SetWindowLong(_tWindow, GWL_STYLE, windowStyle);
    }

    void __internal::Terminal::setCursorVisible(bool visible)
    {
        CONSOLE_CURSOR_INFO cursorInfo;
        cursorInfo.dwSize = 100;

        if(visible) {
            cursorInfo.bVisible = TRUE;
        }
        else {
            cursorInfo.bVisible = FALSE;
        }

        SetConsoleCursorInfo(_tHandle, &cursorInfo);
    }

    void __internal::Terminal::restoreTerminalMode()
    {
        SetConsoleMode(_tHandle, _tMode);
    }

    void __internal::Terminal::enableVirtualMode()
    {
        SetConsoleMode(_tHandle, _tMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    COORD __internal::Terminal::getMaxSizeTerminal() const
    {
        return GetLargestConsoleWindowSize(_tHandle);
    }
#endif  