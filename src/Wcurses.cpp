#include "wcurses/Wcurses.h"
#include "iostream"

waki::Wcurses::Wcurses()
    : _buffer(nullptr), _color(nullptr),
     _cin(nullptr), _term(nullptr), _init(false),
     _originalCoutBuffer(nullptr), _originalCinBuffer(nullptr)
{ }

waki::Wcurses::~Wcurses()
{ 
    endwin();
}

waki::Wcurses& waki::Wcurses::createWcurses()
{
    static Wcurses _wcurses;
    return _wcurses;
}

void waki::Wcurses::initscr(SizeT size)
{
    if(_init) {
        return;
    }

    _term = new __internal::Terminal; 

    _color = new  __internal::ColorManager; 

    _buffer = new  __internal::Buffer(&_color, size); 
    
    _cin = new  __internal::InputManager; 

    _term->setTerminalSize(size.rows, size.cols);
    _term->setMaximizeButton(false);
    _term->setWindowResizing(false);

    _originalCoutBuffer = std::cout.rdbuf();
    _originalCinBuffer = std::cin.rdbuf();

    std::cout.rdbuf(_dummyStream.rdbuf());
    std::cin.rdbuf(_dummyStream.rdbuf());

    _init = true;
}

 void waki::Wcurses::endwin()
 {  
    if(!_init) {
        return;
    }

    *_term << _color->getResetCode();
    
    _term->restoreTerminalMode();
    _term->setMaximizeButton(true);
    _term->setWindowResizing(true);
    _term->setCursorVisible(true);
    
    _term->clearScreen();

    delete _term;
    _term = nullptr;

    delete _color;
    _color = nullptr;

    delete _buffer;
    _buffer = nullptr;

    delete _cin;
    _cin = nullptr;

    std::cout.rdbuf(_originalCoutBuffer);
    std::cin.rdbuf(_originalCinBuffer);

    _init = false;
 }

waki::Wcurses& waki::Wcurses::operator<<(char ch)
{
    if(!_init) {
        return *this;
    }

    *_buffer << ch;

    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(const char* str)
{
    if(!_init) {
        return *this;
    }

    *_buffer << str;

    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(const std::string& str)
{
    if(!_init) {
        return *this;
    }

    *_buffer << str;

    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(short val)
{
    if(!_init) {
        return *this;
    }

    *_buffer << val;

    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(int val)
{
    if(!_init) {
        return *this;
    }

    *_buffer << val;

    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(long val)
{
    if(!_init) {
        return *this;
    }

    *_buffer << val;

    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(long long val)
{
    if(!_init) {
        return *this;
    }

    *_buffer << val;

    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(float val)
{
    if(!_init) {
        return *this;
    }

    *_buffer << val;

    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(double val)
{
    if(!_init) {
        return *this;
    }

    *_buffer << val;

    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(long double val)
{
    if(!_init) {
        return *this;
    }

    *_buffer << val;

    return *this;
}

waki::Wcurses& waki::Wcurses::operator>>(int val)
{
    if(!_init) {
        return *this;
    }

    *_cin >> val;

    return *this;
}

waki::Wcurses& waki::Wcurses::operator>>(Key val)
{
    if(!_init) {
        return *this;
    }

    *_cin >> val;

    return *this;
}

int waki::Wcurses::getch()
{
    if(!_init) {
        return -1;
    }

    return _cin->getCh();
}

void waki::Wcurses::refresh()
{
    if(!_init) {
        return;
    }

    _term->moveCursor(0, 0);

    _buffer->refreshScreenBuffer();
    *_term << _buffer->getScreenBuffer();

    Point cursor = _buffer->getXY(); 
    _term->moveCursor(cursor.y, cursor.x);
}

void waki::Wcurses::startColor()
{
    if(!_init) {
        return;
    }

    _color->startColor();
}

void waki::Wcurses::initColor(short pair, short r, short g, short b) 
{
    if(!_init) {
        return;
    }

    *_term << _color->initColor(pair, r, g, b);
}

void waki::Wcurses::initPair(short pair, short fg, short bg) 
{
    if(!_init) {
        return;
    }

    _color->initPair(pair, fg, bg);
}

void waki::Wcurses::initDefaultPair(short pair) 
{
    if(!_init) {
        return;
    }

    _color->initDefaultPair(pair);
}

void waki::Wcurses::setActivePair(short pair) 
{
    if(!_init) {
        return;
    }

    _color->setActivePair(pair);
}

void waki::Wcurses::resetToDefaultPair() 
{
    if(!_init) {
        return;
    }

    _color->resetToDefault();
}

void waki::Wcurses::setTitle(const std::string& title)
{
    if(!_init) {
        return;
    }

    _term->setTitle(title);
}

void waki::Wcurses::setFontSettings(SHORT fontSize, const std::wstring& fontName)
{
    if(!_init) {
        return;
    }

    _term->setFontSettings(fontSize, fontName);
}

void waki::Wcurses::move(short y, short x)
{
    if(!_init) {
        return;
    }

    _buffer->move(y, x);
}

void waki::Wcurses::moveBy(short y, short x)
{
    if(!_init) {
        return;
    }

    _buffer->moveBy(y, x);
}

waki::Point waki::Wcurses::getYX() const
{
    if(!_init) {
        return {-1, -1};
    }

    return _buffer->getXY();
}

void waki::Wcurses::clear()
{   
    if(!_init) {
        return;
    }

    return _buffer->clear();
    refresh();
}

void waki::Wcurses::cursSet(int visibility)
{
    _term->setCursorVisible(visibility);
}

namespace waki
{
	Wcurses& curses = Wcurses::createWcurses();
}