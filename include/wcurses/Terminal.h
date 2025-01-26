#pragma once

#ifdef _WIN32
    #include <string>
    #include <windows.h>

    namespace __internal
    {
        class Terminal
        {
        private:
            HWND _tWindow;
            HANDLE _tHandle;
            DWORD _tMode;
        public:
            // ================================================================
            // ========================= Constructors =========================
            // ================================================================

            Terminal();

            // ================================================================
            // ========================== Operators ===========================
            // ================================================================

            Terminal& operator<<(const std::string& str);
            Terminal& operator<<(char ch);

            // ================================================================
            // ====================== Terminal Managemen ======================
            // ================================================================

            void clearScreen();

            void stopFor(unsigned milliseconds);

            void resetCursor();
            void moveCursor(short y, short x);

            bool setTerminalSize(SHORT height, SHORT width);
            void setFontSettings(SHORT fontSize, const std::wstring& fontName);
            void setTitle(const std::string& title);

            void setMaximizeButton(bool enable);
            void setWindowResizing(bool enable);
            void setCursorVisible(bool visible);

            void restoreTerminalMode();
            void enableVirtualMode();

            // ================================================================
            // ======================== Getter Methods ========================
            // ================================================================

            COORD getMaxSizeTerminal() const;
        };    
    } // namespace __internal
#endif  