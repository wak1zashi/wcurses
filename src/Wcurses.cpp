#include "wcurses/Wcurses.h"

#ifdef _WIN32
    #include "iostream"
#else
    #include "ncurses/ncurses.h"    
#endif

#ifdef _WIN32
    waki::Wcurses::Wcurses()
        : _buffer(nullptr), _color(nullptr),
        _cin(nullptr), _term(nullptr), _init(false),
        _originalCoutBuffer(nullptr), _originalCinBuffer(nullptr)
    { }
#endif

waki::Wcurses::~Wcurses()
{ 
    end();
}

waki::Wcurses& waki::Wcurses::createWcurses()
{
    static Wcurses _wcurses;
    return _wcurses;
}

#ifdef _WIN32
    void waki::Wcurses::start(SizeT size)
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
#else
    void waki::Wcurses::start()
    {
        initscr();
        keypad(stdscr, TRUE);
        noecho();
    }
#endif

void waki::Wcurses::end()
{  
#ifdef _WIN32
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
#else
    endwin();
#endif
}

waki::Wcurses& waki::Wcurses::operator<<(char ch)
{
#ifdef _WIN32
    if(!_init) {
        return *this;
    }

    *_buffer << ch;
#else
    addch(ch);
#endif

    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(const char* str)
{
#ifdef _WIN32
    if(!_init) {
        return *this;
    }

    *_buffer << str;
#else
    printw(str);
#endif

    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(const std::string& str)
{
#ifdef _WIN32
    if(!_init) {
        return *this;
    }

    *_buffer << str;
#else
    printw(str.c_str());
#endif

    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(short val)
{
#ifdef _WIN32
    if(!_init) {
        return *this;
    }

    *_buffer << val;
#else
    printw("%d", val);
#endif
    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(int val)
{
#ifdef _WIN32
    if(!_init) {
        return *this;
    }

    *_buffer << val;
#else
    printw("%d", val);
#endif
    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(long val)
{
#ifdef _WIN32
    if(!_init) {
        return *this;
    }

    *_buffer << val;
#else
    printw("%ld", val);
#endif
    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(long long val)
{
#ifdef _WIN32
    if(!_init) {
        return *this;
    }

    *_buffer << val;
#else
    printw("%lld", val);
#endif
    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(float val)
{
#ifdef _WIN32
    if(!_init) {
        return *this;
    }

    *_buffer << val;
#else
    printw("%f", val);
#endif
    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(double val)
{
#ifdef _WIN32
    if(!_init) {
        return *this;
    }

    *_buffer << val;
#else
    printw("%f", val);
#endif
    return *this;
}

waki::Wcurses& waki::Wcurses::operator<<(long double val)
{
#ifdef _WIN32
    if(!_init) {
        return *this;
    }

    *_buffer << val;
#else
    printw("%f", val);
#endif
    return *this;
}

waki::Wcurses& waki::Wcurses::operator>>(int& val)
{
#ifdef _WIN32
    if(!_init) {
        return *this;
    }

    *_cin >> val;
#else
    val = getch();
#endif
    return *this;
}

waki::Wcurses& waki::Wcurses::operator>>(Key& val)
{
#ifdef _WIN32
    if(!_init) {
        return *this;
    }

    *_cin >> val;
#else
    val = static_cast<Key>(getch());
#endif
    return *this;
}

int waki::Wcurses::getKey()
{
#ifdef _WIN32
    if(!_init) {
        return *this;
    }

    return _cin->getCh();
#else
    return getch();
#endif
}

void waki::Wcurses::refreshScreen()
{
#ifdef _WIN32
    if(!_init) {
        return;
    }

    _term->moveCursor(0, 0);

    _buffer->refreshScreenBuffer();
    *_term << _buffer->getScreenBuffer();

    Point cursor = _buffer->getXY(); 
    _term->moveCursor(cursor.y, cursor.x);
#else   
    refresh();
#endif
}

void waki::Wcurses::startColor()
{
#ifdef _WIN32
    if(!_init) {
        return;
    }

    _color->startColor();
#else  
    start_color();
#endif
}

void waki::Wcurses::initColor(short pair, short r, short g, short b) 
{
#ifdef _WIN32
    if(!_init) {
        return;
    }

    *_term << _color->initColor(pair, r, g, b);
#else 
    init_color(pair, r * 1000 / 255, g * 1000 / 255, b * 1000 / 255);
#endif
}

void waki::Wcurses::initPair(short pair, short fg, short bg) 
{
#ifdef _WIN32
    if(!_init) {
        return;
    }

    _color->initPair(pair, fg, bg);
#else 
    init_pair(pair, fg, bg);
#endif
}

void waki::Wcurses::initDefaultPair(short pair) 
{
#ifdef _WIN32
    if(!_init) {
        return;
    }

    _color->initDefaultPair(pair);
#else 
    bkgd(COLOR_PAIR(pair));
#endif
}

void waki::Wcurses::setActivePair(short pair) 
{
#ifdef _WIN32
    if(!_init) {
        return;
    }

    _color->setActivePair(pair);
#else 
    attron(COLOR_PAIR(pair));
#endif  
}

void waki::Wcurses::resetToDefaultPair() 
{
#ifdef _WIN32
    if(!_init) {
        return;
    }

    _color->resetToDefault();
#else 
    attroff(A_COLOR);
#endif
}

#ifdef _WIN32
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
#endif

void waki::Wcurses::moveTo(short y, short x)
{
#ifdef _WIN32
    if(!_init) {
        return;
    }

    _buffer->move(y, x);
#else 
    move(y, x);
#endif
}

void waki::Wcurses::moveBy(short y, short x)
{
#ifdef _WIN32
    if(!_init) {
        return;
    }

    _buffer->moveBy(y, x);
#else 
    Point cursorPosition = getPos();
    move(cursorPosition.y + y, cursorPosition.x + x);
#endif
}

waki::Point waki::Wcurses::getPos() const
{
#ifdef _WIN32
    if(!_init) {
        return {-1, -1};
    }

    return _buffer->getXY();
#else 
    Point cursorPosition{0, 0};

	getyx(stdscr, cursorPosition.y, cursorPosition.x);

	return cursorPosition; 
#endif
}

void waki::Wcurses::clearScreen()
{   
#ifdef _WIN32
    if(!_init) {
        return;
    }

    return _buffer->clear();
    refreshScreen();
#else   
    clear();
    refreshScreen();
#endif
}

void waki::Wcurses::cursSet(int visibility)
{
#ifdef _WIN32
    _term->setCursorVisible(visibility);
#else 
    curs_set(visibility);
#endif
}

namespace waki
{
	Wcurses& wcurses = Wcurses::createWcurses();
}