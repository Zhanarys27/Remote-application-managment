#include <windows.h>


DWORD SendScanCode(WORD scan, BOOL up)
{
	INPUT inp = { 0 };
	inp.type = INPUT_KEYBOARD;
	inp.ki.wScan = scan;
	inp.ki.dwFlags = KEYEVENTF_SCANCODE | (up ? KEYEVENTF_KEYUP : 0);
	return SendInput(1, &inp, sizeof(inp)) ? NO_ERROR : GetLastError();
}
DWORD SendVirtualKey(UINT vk, BOOL up)
{
	UINT scan = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
	return scan ? SendScanCode(scan, up) : ERROR_NO_UNICODE_TRANSLATION;
}
DWORD SendChar(TCHAR ch, BOOL up)
{
	SHORT vk = VkKeyScan(ch);
	DWORD Result;
	if (0xFFFF == (USHORT)vk)
		Result = ERROR_NO_UNICODE_TRANSLATION;
	else
		if (up)
			Result = SendVirtualKey(LOBYTE(vk), TRUE);
		else {
			Result = NO_ERROR;
			if (HIBYTE(vk) & 1) Result = SendVirtualKey(VK_LSHIFT, FALSE);

			if (NO_ERROR == Result) {
				if (HIBYTE(vk) & 2) Result = SendVirtualKey(VK_LCONTROL, FALSE);

				if (NO_ERROR == Result) {
					if (HIBYTE(vk) & 4) Result = SendVirtualKey(VK_LMENU, FALSE);

					if (NO_ERROR == Result) {
						Result = SendVirtualKey(LOBYTE(vk), FALSE);

						if (NO_ERROR == Result) {
							if (HIBYTE(vk) & 4) Result = SendVirtualKey(VK_LMENU, TRUE);

							if (NO_ERROR == Result) {
								if (HIBYTE(vk) & 2) Result = SendVirtualKey(VK_LCONTROL, TRUE);

								if (NO_ERROR == Result) {
									if (HIBYTE(vk) & 1) Result = SendVirtualKey(VK_LSHIFT, TRUE);
								}
							}
						}
					}
				}
			}
		}
		return Result;
}
DWORD SendString(LPCTSTR s)
{
	DWORD Result = NO_ERROR;
	for (; *s && !Result; ++s) {
		Result = SendChar(s[0], FALSE);
		if (NO_ERROR == Result) Result = SendChar(s[0], TRUE);
	}
	return Result;
}

void fub_q() {
	SendVirtualKey(VK_TAB, FALSE);			
}