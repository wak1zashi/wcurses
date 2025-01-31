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
            {73, waki::Key::PAGE_UP},
            {75, waki::Key::ARROW_RIGHT},
            {77, waki::Key::ARROW_LEFT},
            {80, waki::Key::ARROW_DOWN},
            {81, waki::Key::PAGE_DOWN},
            {82, waki::Key::INSERT},
            {83, waki::Key::DELETE},
            {59, waki::Key::F1},
            {60, waki::Key::F2},
            {61, waki::Key::F3},
            {62, waki::Key::F4},
            {63, waki::Key::F5},
            {64, waki::Key::F6},
            {65, waki::Key::F7},
            {66, waki::Key::F8},
            {67, waki::Key::F9},
            {68, waki::Key::F10},
            {133, waki::Key::F11},
            {134, waki::Key::F12},
        };

		static constexpr short _FN_KEY = 224;
		static constexpr short _F_KEY  = 0;

		static constexpr short _BACKSPACE = 8;
		static constexpr short _ENTER = 13;
        static constexpr short ESCAPE = 27;

		bool _noDelay = false;
	public:
		static constexpr short ERR = -1;

		void clear();

		void noDelay(bool enable) { _noDelay = enable; }

		int getCh();
        waki::Key getKey();

		InputManager& operator>>(int& keyCode);
		InputManager& operator>>(waki::Key& keyCode);
	};
} // namespace __internal