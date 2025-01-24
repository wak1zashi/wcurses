#pragma once

#include <string>
#include <vector>
#include "ColorManager.h"
#include "Cursor.h"

namespace waki
{
    struct SizeT
	{
		short rows;
		short cols;
	};
} // namespace waki

namespace __internal 
{
	struct Chtype
	{
		char symbol = ' ';
		ColorManager::PairIndex colorPair = ColorManager::DEFAULT_PAIR;
	};

    class Buffer
	{
	public:
		using ScreenBufferT = std::string;
    private:
        using BufferT = std::vector<std::vector<Chtype>>;
        using BufferChT = std::vector<std::vector<char>>;

        const int _MIN_SIZE = 1;

        ScreenBufferT _screenBuffer;
        BufferT _buffer;
        BufferChT _bufferCh;
        Cursor _cursor;
        waki::SizeT _size;
        const ColorManager* const* _color;

        // =========================================================================
        // ============================ Initialization =======+=====================
        // =========================================================================

        void initializeBuffer(BufferT& buffer, waki::SizeT size);
        void initializeBuffer(BufferChT& buffer, waki::SizeT size, char fillChar = ' ');

        void initialize(const ColorManager* const* const color, waki::SizeT size);

        // =========================================================================
	    // ============================ Support Methods ============================
        // =========================================================================

        void swap(BufferChT& sourceBuffer, BufferT& destinationBuffer);
        void swap(BufferT& sourceBuffer, BufferChT& destinationBuffer);
    public:

        // =========================================================================
	    // ============================= Constructors ==============================
        // =========================================================================

        Buffer(const ColorManager* const* const color, waki::SizeT size);
        Buffer(const ColorManager* const* const color, short rows, short cols);

        // =========================================================================
        // =============================== Operators ===============================
        // =========================================================================

        Buffer& operator<<(char ch);
        Buffer& operator<<(const char* str);
        Buffer& operator<<(const std::string& str);

        template <typename _Ty>
		Buffer& operator<<(_Ty val);

        // =========================================================================
        // =============================== Resizing ================================
        // =========================================================================

        void resize(waki::SizeT size);
        void resize(short rows, short cols);

        // =========================================================================
        // =========================== Buffer Management =========================== 
        // =========================================================================

        void refreshScreenBuffer();

        void clear();

        // =========================================================================
        // =========================== Movement Methods ============================
        // =========================================================================

        void endl();

        void move(short y, short x);
        void move(waki::Point& cursor);

        void moveBy(short y, short x);
        void moveBy(waki::Point& cursor);

        // =========================================================================
	    // ============================ Getter Methods =============================
        // =========================================================================

        const waki::Point& getXY() const { return _cursor.getYX(); } 
        const waki::SizeT& Size() const { return _size; } 
        const std::string& getScreenBuffer() const { return _screenBuffer; }
    };
    
    template<typename _Ty>
	inline Buffer& Buffer::operator<<(_Ty val) 
	{
		return *this << std::to_string(val);
	}
} // namespace internal