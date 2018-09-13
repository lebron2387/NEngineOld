#pragma once

#include "incdef.h"
#include "Types.h"

class CInput
{
public:
	CInput(void);
	~CInput(void);

	Bool Init(){return 1;}
	void DeInit(){}

	int32 TranslateKey(int32 key);

	__forceinline void SetKeyPressed(int32 key){Keys[key]=1;}
	__forceinline void SetKeyUnPressed(int32 key){Keys[key]=0;}

	__forceinline int32 GetMouseX(){return m_iMouseX;}
	__forceinline void SetMouseX(int32 x){m_iMouseX=x;}
	__forceinline int32 GetMouseY(){return m_iMouseY;}
	__forceinline void SetMouseY(int32 y){m_iMouseY=y;}
	__forceinline int32 GetMouseButton(){return m_iMouseButton;}
	__forceinline void SetMouseButton(int32 button){m_iMouseButton=button;}
	__forceinline int32 GetKeysState(){return m_iKeysState;}
	__forceinline void SetKeysState(int32 state){m_iKeysState=state;}

	
	enum 
	{
		KEY_LMOUSEBUTTON,
		KEY_MMOUSEBUTTON,
		KEY_RMOUSEBUTTON,
		KEY_DOWNMOUSEBUTTON,
		KEY_UPMOUSEBUTTON,
		KEY_CANCEL,
		KEY_BACK,
		KEY_TAB,
		KEY_CLEAR,
		KEY_RETURN,
		KEY_SHIFT,
		KEY_CONTROL,
		KEY_MENU,
		KEY_PAUSE,
		KEY_CAPITAL,
		KEY_ESCAPE,
		KEY_SPACE,
		KEY_PRIOR,
		KEY_NEXT,
		KEY_END,
		KEY_HOME,
		KEY_LEFT,
		KEY_UP,
		KEY_RIGHT,
		KEY_DOWN,
		KEY_SELECT,
		KEY_PRINT,
		KEY_SNAPSHOT,
		KEY_INSERT,
		KEY_DELETE,
		KEY_HELP,
		KEY_NUMPAD0,
		KEY_NUMPAD1,
		KEY_NUMPAD2,
		KEY_NUMPAD3,
		KEY_NUMPAD4,
		KEY_NUMPAD5,
		KEY_NUMPAD6,
		KEY_NUMPAD7,
		KEY_NUMPAD8,
		KEY_NUMPAD9,
		KEY_MULTIPLY,
		KEY_ADD,
		KEY_SEPARATOR,
		KEY_SUBTRACT,
		KEY_DECIMAL,
		KEY_DIVIDE,
		KEY_F1,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_F5,
		KEY_F6,
		KEY_F7,
		KEY_F8,
		KEY_F9,
		KEY_F10,
		KEY_F11,
		KEY_F12,
		KEY_F13,
		KEY_F14,
		KEY_F15,
		KEY_F16,
		KEY_F17,
		KEY_F18,
		KEY_F19,
		KEY_F20,
		KEY_F21,
		KEY_F22,
		KEY_F23,
		KEY_F24,
		KEY_NUMLOCK,
		KEY_SCROLL,
		KEY_LSHIFT,
		KEY_RSHIFT,
		KEY_LCONTROL,
		KEY_RCONTROL,
		KEY_LMENU,
		KEY_RMENU

	};



private:
	Bool Keys[512];
	int32 m_iMouseX;
	int32 m_iMouseY;
	int32 m_iMouseButton;
	int32 m_iKeysState;
};
