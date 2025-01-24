#include "wcurses/Buffer.h"

#include <cstring>

void __internal::Buffer::initializeBuffer(BufferT& buffer, waki::SizeT size) {
    buffer.resize(size.rows, std::vector<Chtype>(size.cols)); 
}

void __internal::Buffer::initializeBuffer(BufferChT& buffer, waki::SizeT size, char fillChar) {
    buffer.resize(size.rows, std::vector<char>(size.cols, fillChar));
}

void __internal::Buffer::initialize(const ColorManager* const* const color, waki::SizeT size)
{
    if(size.rows < _MIN_SIZE || size.cols < _MIN_SIZE)
    {
        size.rows = 1;
        size.cols = 1;
    }

    _screenBuffer.clear();
    _screenBuffer.shrink_to_fit();
    
    _buffer.clear();
    _buffer.shrink_to_fit();

    _bufferCh.clear();
    _bufferCh.shrink_to_fit();

    _size = size;
    _cursor.setRange(_size.rows, _size.cols);

    _color = color;

    if(_color != nullptr && (*_color) != nullptr && (*_color)->isStartedColor()) {
        initializeBuffer(_buffer, _size);
    }
    else {
        initializeBuffer(_bufferCh, _size);
    }
}

void __internal::Buffer::swap(BufferChT& sourceBuffer, BufferT& destinationBuffer)
{
    if(sourceBuffer.empty()) {
        return;
    }

    if(destinationBuffer.empty()) 
    {
        initializeBuffer(destinationBuffer, _size);
        return;
    }

    destinationBuffer.clear();
    destinationBuffer.resize(_size.rows);

    for (int i = 0; i < _size.rows; ++i)
    {
        destinationBuffer[i].resize(_size.cols);

        for (int j = 0; j < _size.cols; ++j) {
            destinationBuffer[i][j].symbol = sourceBuffer[i][j];
        }
    }

    sourceBuffer.clear();
    sourceBuffer.shrink_to_fit();
}


void __internal::Buffer::swap(BufferT& sourceBuffer, BufferChT& destinationBuffer)
{
    if(sourceBuffer.empty()) {
        return;
    }

    destinationBuffer.clear();
    destinationBuffer.resize(_size.rows);

    for (int i = 0; i < _size.rows; ++i)
    {
        destinationBuffer[i].resize(_size.cols);

        for (int j = 0; j < _size.cols; ++j) {
            destinationBuffer[i][j] = sourceBuffer[i][j].symbol;
        }
    }

    sourceBuffer.clear();
    sourceBuffer.shrink_to_fit();
}

__internal::Buffer::Buffer(const ColorManager* const* const color, waki::SizeT size)
{
    initialize(color, size);
}

__internal::Buffer::Buffer(const ColorManager* const* const, short rows, short cols)
{
    initialize(_color, {rows, cols});
}

__internal::Buffer& __internal::Buffer::operator<<(char ch)
{
    if (_cursor.getX() >= _size.cols) {
        endl();
	}

	if (ch == '\n')
	{
		endl();
		return *this;
	}

	if(_color != nullptr && (*_color) != nullptr && (*_color)->isStartedColor())
	{
        if (_buffer.empty()) {
		    swap(_bufferCh, _buffer);    
	    }

		_buffer[_cursor.getY()][_cursor.getX()].symbol = ch;
		_buffer[_cursor.getY()][_cursor.getX()].colorPair = (*_color)->getActivePair();
	}
	else 
    {
        if (_bufferCh.empty()) {
            swap(_buffer, _bufferCh);    
	    }

		_bufferCh[_cursor.getY()][_cursor.getX()] = ch;
	}

	if (_cursor.getX() >= _size.cols - 1) {
		endl();
	}
	else {
		_cursor.incrementX();
	}

	return *this;
}

__internal::Buffer& __internal::Buffer::operator<<(const char* str)
{
    size_t length = std::strlen(str);
	for (int i = 0; i < length; ++i) {
		this->operator<<(str[i]);
	}

	return *this;
}

__internal::Buffer& __internal::Buffer::operator<<(const std::string& str)
{
    for (size_t i = 0; i < str.size(); ++i) {
        *this << str[i];
	}

	return *this;
}

void __internal::Buffer::resize(waki::SizeT size)
{
    initialize(_color, size);
}

void __internal::Buffer::resize(short rows, short cols)
{
    resize({rows, cols});
}

void __internal::Buffer::refreshScreenBuffer()
{
    ColorManager::PairIndex currPair = 0;
    bool colorStart = false;

    if(_color != nullptr && (*_color) != nullptr && (*_color)->isStartedColor())
    {
        colorStart = true;

        if (_buffer.empty()) {
		    swap(_bufferCh, _buffer);    
	    }

        currPair = _buffer[0][0].colorPair;
        _screenBuffer = (*_color)->getColorCode(currPair);
    }

    for(int i = 0; i < _size.rows; ++i)
    {
        for (int j = 0; j < _size.cols; ++j)
        {
            if (colorStart)
            {
                if (currPair != _buffer[i][j].colorPair)
                {
                    ColorManager::PairIndex newCurrPair = _buffer[i][j].colorPair;
                    _screenBuffer += (*_color)->getColorCode(currPair, newCurrPair);

                    currPair = newCurrPair;
                }

                _screenBuffer += _buffer[i][j].symbol;
            }
            else {
                _screenBuffer += _bufferCh[i][j];
            }
        }

        if (i < _size.rows - 1) {
            _screenBuffer += '\n';
        }
    }
}

void __internal::Buffer::clear()
{
    if(_color != nullptr && (*_color) != nullptr && (*_color)->isStartedColor())
    {
        for(auto& rows : _buffer)
        {
            for(auto& ch : rows)
            {
                ch.symbol = ' ';
                ch.colorPair = ColorManager::DEFAULT_PAIR;
            }
        }
    }
    else
    {
        for(auto& rows : _bufferCh)
        {
            for(auto& ch : rows) {
                ch = ' ';
            }
        }
    }

    _cursor.reset();
}

void __internal::Buffer::endl()
{
	if (!_cursor.isAtBottom())
	{
		_cursor.incrementY();
		_cursor.resetX();
	}
}

void __internal::Buffer::move(short y, short x)
{
    _cursor.move(y, x);
}

void __internal::Buffer::move(waki::Point& cursor)
{
    _cursor.move(cursor);
}

void __internal::Buffer::moveBy(short y, short x)
{
    _cursor.moveBy(y, x);
}

void __internal::Buffer::moveBy(waki::Point& cursor)
{
    _cursor.moveBy(cursor);
}