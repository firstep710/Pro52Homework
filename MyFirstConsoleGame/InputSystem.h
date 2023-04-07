#pragma once
namespace global
{
	namespace input
	{
		const int ESCAPE_KEY_INDEX = 0;
		const int USER_CMD_LEFT = 1;
		const int USER_CMD_RIGHT = 2;
		const int USER_CMD_UP = 3;
		const int USER_CMD_DOWN = 4;
		const int USER_CMD_SPACE = 5;

		const int MAX_KEY = 6;

		void Set(const int keyIdx, bool bOn);

		bool IsEscapeCmdOn();
		bool IsLeftCmdOn();
		bool IsRightCmdOn();
		bool IsUpCmdOn();
		bool IsDownCmdOn();
		bool IsSpaceCmdOn();

		void UpdateInput();

	}
}