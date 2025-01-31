#include "wcurses/InputManager.h"

#include <conio.h>

void __internal::InputManager::clear()
{
	for (; _kbhit() ;) { (void)_getch(); }
}

int __internal::InputManager::getCh()
{
	if (_noDelay && !_kbhit()) {
		return ERR;
	}

	int keyCode = _getch();

	if (keyCode >= static_cast<int>(waki::Key::SPACE) &&
		keyCode <= static_cast<int>(waki::Key::TILDE)) {
		return keyCode;
	}

	if (keyCode == _ENTER) {
		return static_cast<int>(waki::Key::ENTER);
	}

	if (keyCode == _BACKSPACE) {
		return static_cast<int>(waki::Key::BACKSPACE);
	}

	if (keyCode == static_cast<int>(waki::Key::ESCAPE))
	{
		return keyCode;
	}

	if (keyCode == _FN_KEY || keyCode == _F_KEY)
	{
		keyCode = _getch();

		auto fnKey = _fnKeyMap.find(keyCode);
		if (fnKey != _fnKeyMap.end()) {
			return static_cast<int>(fnKey->second);
		}
	}

	return ERR;
}

waki::Key __internal::InputManager::getKey()
{
    return static_cast<waki::Key>(getCh());
}

__internal::InputManager& __internal::InputManager::operator>>(int& keyCode)
{
	keyCode = getCh();
	return *this;
}

__internal::InputManager& __internal::InputManager::operator>>(waki::Key& keyCode)
{
	keyCode = static_cast<waki::Key>(getCh());
	return *this;
}