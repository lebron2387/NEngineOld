#include "Input.h"

CInput::CInput(void)
{
}

CInput::~CInput(void)
{
}

int32 CInput::TranslateKey(int32 key) 
{
	int32 keyID;
	switch(key) 
	{
		case VK_CANCEL : keyID = KEY_CANCEL; break;
		case VK_BACK : keyID = KEY_BACK; break;
		case VK_TAB : keyID = KEY_TAB; break;
		case VK_CLEAR : keyID = KEY_CLEAR; break;
		case VK_RETURN : keyID = KEY_RETURN; break;
		case VK_SHIFT : keyID = KEY_SHIFT; break;
		case VK_CONTROL : keyID = KEY_CONTROL; break;
		case VK_MENU : keyID = KEY_MENU; break;
		case VK_PAUSE : keyID = KEY_PAUSE; break;
		case VK_CAPITAL : keyID = KEY_CAPITAL; break;
		case VK_ESCAPE : keyID = KEY_ESCAPE; break;
		case VK_SPACE : keyID = KEY_SPACE; break;
		case VK_PRIOR : keyID = KEY_PRIOR; break;
		case VK_NEXT : keyID = KEY_NEXT; break;
		case VK_END : keyID = KEY_END; break;
		case VK_HOME : keyID = KEY_HOME; break;
		case VK_LEFT : keyID = KEY_LEFT; break;
		case VK_UP : keyID = KEY_UP; break;
		case VK_RIGHT : keyID = KEY_RIGHT; break;
		case VK_DOWN : keyID = KEY_DOWN; break;
		case VK_SELECT : keyID = KEY_SELECT; break;
		case VK_PRINT : keyID = KEY_PRINT; break;
		case VK_SNAPSHOT : keyID = KEY_SNAPSHOT; break;
		case VK_INSERT : keyID = KEY_INSERT; break;
		case VK_DELETE : keyID = KEY_DELETE; break;
		case VK_HELP : keyID = KEY_HELP; break;
		case VK_NUMPAD0 : keyID = KEY_NUMPAD0; break;
		case VK_NUMPAD1 : keyID = KEY_NUMPAD1; break;
		case VK_NUMPAD2 : keyID = KEY_NUMPAD2; break;
		case VK_NUMPAD3 : keyID = KEY_NUMPAD3; break;
		case VK_NUMPAD4 : keyID = KEY_NUMPAD4; break;
		case VK_NUMPAD5 : keyID = KEY_NUMPAD5; break;
		case VK_NUMPAD6 : keyID = KEY_NUMPAD6; break;
		case VK_NUMPAD7 : keyID = KEY_NUMPAD7; break;
		case VK_NUMPAD8 : keyID = KEY_NUMPAD8; break;
		case VK_NUMPAD9 : keyID = KEY_NUMPAD9; break;
		case VK_MULTIPLY : keyID = KEY_MULTIPLY; break;
		case VK_ADD : keyID = KEY_ADD; break;
		case VK_SEPARATOR : keyID = KEY_SEPARATOR; break;
		case VK_SUBTRACT : keyID = KEY_SUBTRACT; break;
		case VK_DECIMAL : keyID = KEY_DECIMAL; break;
		case VK_DIVIDE : keyID = KEY_DIVIDE; break;
		case VK_F1 : keyID = KEY_F1; break;
		case VK_F2 : keyID = KEY_F2; break;
		case VK_F3 : keyID = KEY_F3; break;
		case VK_F4 : keyID = KEY_F4; break;
		case VK_F5 : keyID = KEY_F5; break;
		case VK_F6 : keyID = KEY_F6; break;
		case VK_F7 : keyID = KEY_F7; break;
		case VK_F8 : keyID = KEY_F8; break;
		case VK_F9 : keyID = KEY_F9; break;
		case VK_F10 : keyID = KEY_F10; break;
		case VK_F11 : keyID = KEY_F11; break;
		case VK_F12 : keyID = KEY_F12; break;
		case VK_F13 : keyID = KEY_F13; break;
		case VK_F14 : keyID = KEY_F14; break;
		case VK_F15 : keyID = KEY_F15; break;
		case VK_F16 : keyID = KEY_F16; break;
		case VK_F17 : keyID = KEY_F17; break;
		case VK_F18 : keyID = KEY_F18; break;
		case VK_F19 : keyID = KEY_F19; break;
		case VK_F20 : keyID = KEY_F20; break;
		case VK_F21 : keyID = KEY_F21; break;
		case VK_F22 : keyID = KEY_F22; break;
		case VK_F23 : keyID = KEY_F23; break;
		case VK_F24 : keyID = KEY_F24; break;
		case VK_NUMLOCK : keyID = KEY_NUMLOCK; break;
		case VK_SCROLL : keyID = KEY_SCROLL; break;
		case VK_LSHIFT : keyID = KEY_LSHIFT; break;
		case VK_RSHIFT : keyID = KEY_RSHIFT; break;
		case VK_LCONTROL : keyID = KEY_LCONTROL; break;
		case VK_RCONTROL : keyID = KEY_RCONTROL; break;
		case VK_LMENU : keyID = KEY_LMENU; break;
		case VK_RMENU : keyID = KEY_RMENU; break;

			
		default:
			keyID = MapVirtualKey(key,2);
			if(strchr("1234567890-=",keyID))
			{
				if(Keys[KEY_SHIFT])
				{
					if(keyID == '1') keyID = '!';
					else if(keyID == '2') keyID = '@';
					else if(keyID == '3') keyID = '#';
					else if(keyID == '4') keyID = '$';
					else if(keyID == '5') keyID = '%';
					else if(keyID == '6') keyID = '^';
					else if(keyID == '7') keyID = '&';
					else if(keyID == '8') keyID = '*';
					else if(keyID == '9') keyID = '(';
					else if(keyID == '0') keyID = ')';
					else if(keyID == '-') keyID = '_';
					else if(keyID == '=') keyID = '+';
				}
			}
			else if(isascii(keyID)) 
			{
				if(Keys[KEY_SHIFT])
					keyID = toupper(keyID);
				else
					keyID = tolower(keyID);
			}
			else keyID = 0;

	}
	return keyID;
}
