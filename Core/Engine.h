#pragma once

#include "incdef.h"
#include "Types.h"
#include "Input.h"
#include "Timer.h"

class CRender;

class CEngine
{
public:
	CEngine(uint32 render);
	~CEngine(void);

	enum 
	{
		OPENGL_RENDER = 1,
		DIRECT3D9_RENDER = 2
	};

	enum 
	{
		OS_WINDOWS = 1,
		OS_LINUX = 2,
		OS_MAC = 3
	};

	LRESULT EngineWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	int32 EngineRun(HINSTANCE hInst, HINSTANCE, LPSTR, INT);
	Bool InitEngine();
	Bool SetRenderMode(int32 render_mode);
	Bool SetVideoMode(int32 screenwidth,int32 screenheight,Bool fullscreen,int32 bpp,Bool vsync);
	void DeInitEngine();
	void CreateMainWindow();
	void DestroyMainWindow();
	void RenderEngine();
	Float32 CalculateFrameRate();
	
	__forceinline uint32 GetRenderMode(){return m_iRenderMode;}
	__forceinline void SetMsgProc(WNDPROC msg_proc){MsgProc = msg_proc;}

	virtual void Init(){}
	virtual void DeInit(){}
	virtual void Render(){};
	virtual void onKeyPressed(int32 key){}
	virtual void onKeyUnPressed(int32 key){}
	virtual void onButtonPressed(int32 key){}
	virtual void onButtonUnPressed(int32 key){}
	virtual void onMouseMove(){}

	__forceinline Float32 GetFramesPerSecond(){return m_fFramesPerSecond;}
	__forceinline uint32 GetMouseX(){return m_pInput->GetMouseX();}
	__forceinline uint32 GetMouseY(){return m_pInput->GetMouseY();}
	__forceinline void SetMouseCursorVisible(Bool visible){m_bShowCursor = visible;}

private:

	uint32 m_iRenderMode;
	WNDPROC MsgProc;
	Bool m_bShowCursor;
	int32 m_iScreenWidth;
	int32 m_iScreenHeight;
	int32 m_iScreenDepth;
	Float32	m_fTimeMovement;
	Float32	m_fFramesPerSecond;
	Bool m_bVSync;
	Bool m_bFullScreen;
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	Bool m_bDeviceObjectsRestored;
protected:

	CRender *m_pRender;
	CInput *m_pInput;	
};