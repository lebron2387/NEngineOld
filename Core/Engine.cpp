/*
*/

#include "Engine.h"
#include "../Render/D3DRender.h"
#include "../Render/OGLRender.h"

/*
*/

CEngine::CEngine(uint32 render)
{
	m_iScreenWidth = 800;
	m_iScreenHeight = 600;
	m_iScreenDepth = 32;
	m_fTimeMovement = 0.0f;
	m_bShowCursor = true;
	m_bVSync = false;
	m_bFullScreen = false;
	m_bDeviceObjectsRestored = false;
	m_iRenderMode = render;

	if(render == OPENGL_RENDER)
	{
		m_pRender = new COGLRender();
	}
	else if(render == DIRECT3D9_RENDER)
	{
		m_pRender = new CD3D9Render();
	}
	else
	{
		m_pRender = new CD3D9Render();
	}
	
	m_pInput = new CInput();
}

CEngine::~CEngine(void)
{
	SAFE_DELETE(m_pRender);
	SAFE_DELETE(m_pInput);
	m_hWnd = NULL;
	m_hInstance = NULL;
}

/*
*/

LRESULT CEngine::EngineWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT    ps;
	switch( msg )
    {
		/*
		case WM_SIZE:										
			m_iScreenWidth = LOWORD(lParam);
			m_iScreenHeight = HIWORD(lParam);
			m_pRender->SetViewport(0,0,m_iScreenWidth,m_iScreenHeight);
			break; 
		*/
		case WM_PAINT:				
			ShowCursor(m_bShowCursor);
			BeginPaint(hWnd, &ps);								
			EndPaint(hWnd, &ps);							
			break;

		//case WM_SETCURSOR:
			//SetCursor(m_bShowCursor);
			//return 1;
		//	break;

		case WM_KEYDOWN:
			{
				int32 k = m_pInput->TranslateKey((int32)wParam);
				m_pInput->SetKeyPressed(k);
				onKeyPressed(k);
			}
			break;

		case WM_KEYUP:
			{
				int32 k = m_pInput->TranslateKey((int32)wParam);
				m_pInput->SetKeyUnPressed(k);
				onKeyUnPressed(k);
			}
			break;

		case WM_LBUTTONDOWN:
			m_pInput->SetMouseButton(CInput::KEY_LMOUSEBUTTON);
			m_pInput->SetKeyPressed(CInput::KEY_LMOUSEBUTTON);
			onButtonPressed(CInput::KEY_LMOUSEBUTTON);
			break;
		case WM_LBUTTONUP:
			m_pInput->SetMouseButton(0);
			m_pInput->SetKeyUnPressed(CInput::KEY_LMOUSEBUTTON);
			onButtonUnPressed(CInput::KEY_LMOUSEBUTTON);
			break;
		case WM_MBUTTONDOWN:
			m_pInput->SetMouseButton(CInput::KEY_MMOUSEBUTTON);
			m_pInput->SetKeyPressed(CInput::KEY_MMOUSEBUTTON);
			onButtonPressed(CInput::KEY_MMOUSEBUTTON);
			break;
		case WM_MBUTTONUP:
			m_pInput->SetMouseButton(0);
			m_pInput->SetKeyUnPressed(CInput::KEY_MMOUSEBUTTON);
			onButtonUnPressed(CInput::KEY_MMOUSEBUTTON);
			break;
		case WM_RBUTTONDOWN:
			m_pInput->SetMouseButton(CInput::KEY_RMOUSEBUTTON);
			m_pInput->SetKeyPressed(CInput::KEY_RMOUSEBUTTON);
			onButtonPressed(CInput::KEY_RMOUSEBUTTON);
			break;
		case WM_RBUTTONUP:
			m_pInput->SetMouseButton(0);
			m_pInput->SetKeyUnPressed(CInput::KEY_RMOUSEBUTTON);
			onButtonUnPressed(CInput::KEY_RMOUSEBUTTON);
			break;
		case 0x020A: //WM_MOUSEWHEEL:
			if((short)HIWORD(wParam) > 0) 
			{
				m_pInput->SetMouseButton(CInput::KEY_UPMOUSEBUTTON);
				m_pInput->SetKeyPressed(CInput::KEY_UPMOUSEBUTTON);
				m_pInput->SetKeyUnPressed(CInput::KEY_UPMOUSEBUTTON);
				onButtonPressed(CInput::KEY_UPMOUSEBUTTON);
				onButtonUnPressed(CInput::KEY_UPMOUSEBUTTON);
			}
			else 
			{
				m_pInput->SetMouseButton(CInput::KEY_DOWNMOUSEBUTTON);
				m_pInput->SetKeyPressed(CInput::KEY_DOWNMOUSEBUTTON);
				m_pInput->SetKeyUnPressed(CInput::KEY_DOWNMOUSEBUTTON);
				onButtonPressed(CInput::KEY_DOWNMOUSEBUTTON);
				onButtonUnPressed(CInput::KEY_DOWNMOUSEBUTTON);
			}
			break;
		case WM_MOUSEMOVE:
			onMouseMove();
			m_pInput->SetMouseX(LOWORD(lParam));
			m_pInput->SetMouseY(HIWORD(lParam));
			break;

		case WM_CLOSE:
			DeInitEngine();
            PostQuitMessage( 0 );
			break;
    }
    return DefWindowProc( hWnd, msg, wParam, lParam );
}

Bool CEngine::SetVideoMode(int32 screenwidth,int32 screenheight,Bool fullscreen,int32 bpp,Bool vsync)
{
	m_iScreenWidth = screenwidth;
	m_iScreenHeight = screenheight;
	m_iScreenDepth = bpp;
	m_bVSync = vsync;
	m_bFullScreen = fullscreen;

	m_pRender->DeInit(m_bFullScreen,m_hWnd);
	DestroyMainWindow();
	CreateMainWindow();
	m_pRender->Init(m_iScreenWidth,m_iScreenHeight,m_bFullScreen,m_iScreenDepth,m_bVSync,m_hWnd);
	ShowWindow(m_hWnd,SW_SHOW);						
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
	return 1;
}

Bool CEngine::SetRenderMode(int32 render_mode)
{
	if(render_mode == CEngine::DIRECT3D9_RENDER)
	{
		m_iRenderMode = render_mode;
		m_pRender->DeInit(m_bFullScreen,m_hWnd);
		SAFE_DELETE(m_pRender);
		DestroyMainWindow();
		CMemoryManager::GetNotDeletedMemory();
		CreateMainWindow();
		m_pRender = new CD3D9Render();
		m_pRender->Init(m_iScreenWidth,m_iScreenHeight,m_bFullScreen,m_iScreenDepth,m_bVSync,m_hWnd);
		ShowWindow(m_hWnd,SW_SHOW);						
		SetForegroundWindow(m_hWnd);
		SetFocus(m_hWnd);
		return 1;
	}
	else if(render_mode == CEngine::OPENGL_RENDER)
	{
		m_iRenderMode = render_mode;
		m_pRender->DeInit(m_bFullScreen,m_hWnd);
		SAFE_DELETE(m_pRender);
		DestroyMainWindow();
		CMemoryManager::GetNotDeletedMemory();
		CreateMainWindow();
		m_pRender = new COGLRender();
		m_pRender->Init(m_iScreenWidth,m_iScreenHeight,m_bFullScreen,m_iScreenDepth,m_bVSync,m_hWnd);
		ShowWindow(m_hWnd,SW_SHOW);						
		SetForegroundWindow(m_hWnd);
		SetFocus(m_hWnd);
		return 1;
	}
	return 0;
}

void CEngine::CreateMainWindow()
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,GetModuleHandle(NULL), NULL, NULL, NULL, NULL,"Window", NULL };
    RegisterClassEx( &wc );
	m_hInstance = wc.hInstance;
    m_hWnd = CreateWindow( "Window", "",WS_POPUP, 0, 0, m_iScreenWidth, m_iScreenHeight,GetDesktopWindow(), NULL, m_hInstance, NULL );
}

/*
*/

Bool CEngine::InitEngine()
{
	if(!m_pRender->Init(m_iScreenWidth,m_iScreenHeight,m_bFullScreen,m_iScreenDepth,m_bVSync,m_hWnd))
	{
		return false;
	}

	if(!m_pInput->Init())
	{
		return false;
	}

	ShowWindow(m_hWnd,SW_SHOW);						
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	Init();

	return true;
}

/*
*/

void CEngine::DeInitEngine()
{
	DeInit();
	m_pRender->DeInit(m_bFullScreen,m_hWnd);
	m_pInput->DeInit();
}

/*
*/

int CEngine::EngineRun(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	CreateMainWindow();
    if(InitEngine())
    {
        MSG msg;
        ZeroMemory( &msg, sizeof(msg) );
        while( msg.message!=WM_QUIT )
        {

            if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
            else
			{
                RenderEngine();
			}
        }
    }
    
    DestroyMainWindow();

	return 0;
}

/*
*/

void CEngine::DestroyMainWindow()
{
	if (m_hWnd && !DestroyWindow(m_hWnd))					// Are We Able To Destroy The Window?
	{
		//MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("Window",m_hInstance))			// Are We Able To Unregister Class
	{
		//MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_hInstance=NULL;									// Set hInstance To NULL
	}
}

/*
*/

void CEngine::RenderEngine()
{
	m_pRender->RenderStart();
	Render();
	m_pRender->Render();
	m_fFramesPerSecond = CalculateFrameRate();
	m_pRender->RenderEnd();
}

/*
*/

Float32 CEngine::CalculateFrameRate()
{
	static Float32 framesPerSecond    = 0.0f;							
    static Float32 lastTime			= 0.0f;						
	static Char strFrameRate[50] = {0};	
	static Float32 frametime=0.0f;

    Float32 currentTime=(Float32)GetTickCount();

	m_fTimeMovement = currentTime - frametime;

	frametime = currentTime;
    ++framesPerSecond;

    if(currentTime-lastTime>1000.0f )
    { 
	    lastTime = currentTime;
		m_fFramesPerSecond = framesPerSecond;
        framesPerSecond = 0;
		currentTime=0;
    };

	return m_fFramesPerSecond;

}

/*
*/