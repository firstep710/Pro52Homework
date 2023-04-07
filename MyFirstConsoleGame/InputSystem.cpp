#include "InputSystem.h"
#include <Windows.h>
#include <cassert>

namespace global
{
	namespace input
	{
		bool inputKeyTable[MAX_KEY];

		void Set(const int keyIdx, bool bOn)
		{
			assert(keyIdx >= 0 && keyIdx < MAX_KEY);

			inputKeyTable[keyIdx] = bOn;
		}

		bool IsEscapeCmdOn()
		{
			return inputKeyTable[ESCAPE_KEY_INDEX];
		}
		bool IsLeftCmdOn()
		{
			return inputKeyTable[USER_CMD_LEFT];
		}
		bool IsRightCmdOn()
		{
			return inputKeyTable[USER_CMD_RIGHT];
		}
		bool IsUpCmdOn()
		{
			return inputKeyTable[USER_CMD_UP];
		}
		bool IsDownCmdOn()
		{
			return inputKeyTable[USER_CMD_DOWN];
		}
		bool IsSpaceCmdOn()
		{
			return inputKeyTable[USER_CMD_SPACE];
		}

		void UpdateInput()
		{
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			{
				global::input::Set(ESCAPE_KEY_INDEX, true);
			}

			if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			{
				global::input::Set(USER_CMD_LEFT, true);
			}

			if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			{
				global::input::Set(USER_CMD_RIGHT, true);
			}

			if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				global::input::Set(USER_CMD_UP, true);
			}

			if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				global::input::Set(USER_CMD_DOWN, true);
			}
			if (GetAsyncKeyState(VK_SPACE))
			{
				global::input::Set(USER_CMD_SPACE, true);
			}
		}
	}
}