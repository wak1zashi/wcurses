#pragma once

#include <unordered_map>
#include "Key.h"

namespace __internal
{
	class InputManager
	{
	private:
		const std::unordered_map<short, waki::Key> _fnKeyMap
        {
            {72, waki::Key::ARROW_UP},
            {73, waki::Key::PAGE_DOWN},
            {75, waki::Key::ARROW_RIGHT},
            {77, waki::Key::ARROW_LEFT},
            {80, waki::Key::ARROW_DOWN},
            {81, waki::Key::PAGE_UP},
            {82, waki::Key::INSERT},
            {83, waki::Key::DELETE},
        };

		const short _BACKSPACE = 8;
		const short _ENTER = 13;
        const short ESCAPE = 27;
		const short _FN_KEY = 224;
	public:
		void clear();

		int getCh();
        waki::Key getKey();

		InputManager& operator>>(int& keyCode);
		InputManager& operator>>(waki::Key& keyCode);
	};
} // namespace __internal