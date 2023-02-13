//
// Copyright (c) 2016-2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "bgfx/platform.h"

#include "FWCore.h"
#include "GameCore.h"
#include "EventSystem/Events.h"
#include "EventSystem/EventManager.h"
#include "Utility/Utility.h"

namespace fw {

// Initialize window on windows, huge chunks taken from nehe
//    http://nehe.gamedev.net/tutorial/creating_an_opengl_window_%28win32%29/13001/

// Public methods

FWCore::FWCore(uint32 width, uint32 height, bgfx::RendererType::Enum renderType)
{
    Init( width, height );

    // Initialize bgfx.
    bgfx::Init init;
    
    init.platformData.nwh = GetWindowHandle();
    init.type = renderType;

    init.resolution.width = width;
    init.resolution.height = height;
    init.resolution.reset = BGFX_RESET_VSYNC;

    bool succeeded = bgfx::init( init );
    assert( succeeded );

    // Show the window.
    ShowWindow( m_hWnd, SW_SHOW );   // Show the window.
    SetForegroundWindow( m_hWnd );   // Slightly higher priority.
    SetFocus( m_hWnd );              // Sets keyboard focus to the window.
    ResizeWindow( width, height );   // Set up our screen.
}

FWCore::~FWCore()
{
    bgfx::shutdown();
}

bool FWCore::Init(uint32 width, uint32 height)
{
    m_WindowClientWidth = width;
    m_WindowClientHeight = height;

    // Create our render window.
    if( !FWCore::CreateRenderWindow( "My Window", width, height, 32, m_FullscreenMode ) )
    {
        PostQuitMessage( 0 );
        return false;
    }

    return true;
}

uint32 FWCore::Run(GameCore& game)
{
    m_pGame = &game;

    // Main loop.
    MSG message;
    bool done = false;

    double lastTime = GetSystemTimeSinceGameStart();

    while( !done )
    {
        if( PeekMessage( &message, nullptr, 0, 0, PM_REMOVE ) )
        {
            if( message.message == WM_QUIT )
            {
                done = true;
            }
            else
            {
                TranslateMessage( &message );
                DispatchMessage( &message );
            }
        }
        else
        {
            double currentTime = GetSystemTimeSinceGameStart();
            float deltaTime = static_cast<float>( currentTime - lastTime );
            lastTime = currentTime;

            game.StartFrame( deltaTime );
            game.Update( deltaTime );
            game.Draw();
            game.EndFrame();

            // Swap buffers.
            m_FrameCount = bgfx::frame();

            // Backup the state of the keyboard and mouse.
            for( int i=0; i<256; i++ )
                m_OldKeyStates[i] = m_KeyStates[i];

            for( int i=0; i<3; i++ )
                m_OldMouseButtonStates[i] = m_MouseButtonStates[i];

            m_MouseWheel = 0;
        }
    }

    // Truncate wParam in 64-bit mode to an int.
    return static_cast<uint32>( message.wParam );
}

void FWCore::Shutdown()
{
    m_pGame->OnShutdown();
    DestroyRenderWindow( true );
    PostQuitMessage(0);
}

void FWCore::SetWindowSize(uint32 width, uint32 height)
{
    SetWindowPositionAndSize( 0, 0, width, height, false );
}

void FWCore::SetWindowPositionAndSize(int32 x, int32 y, uint32 width, uint32 height, bool maximized)
{
    SetWindowPos( m_hWnd, nullptr, x, y, width, height, 0 );
    if( maximized )
    {
        ShowWindow( m_hWnd, SW_MAXIMIZE );
    }
    RECT clientRect;
    GetClientRect( m_hWnd, &clientRect );
    ResizeWindow( clientRect.right - clientRect.left, clientRect.bottom - clientRect.top );
}

void FWCore::SetClientPositionAndSize(int32 x, int32 y, uint32 width, uint32 height, bool maximized)
{
    uint32 maxWidth = GetSystemMetrics( SM_CXFULLSCREEN );
    uint32 maxHeight = GetSystemMetrics( SM_CYFULLSCREEN );

    float aspect = static_cast<float>( width ) / height;

    if( width > maxWidth )
    {
        width = maxWidth;
        height = static_cast<uint32>(maxWidth / aspect);
    }

    if( height > maxHeight )
    {
        width = static_cast<uint32>(maxHeight * aspect);
        height = maxHeight;
    }

    DWORD dwStyle = static_cast<DWORD>( GetWindowLongPtr( m_hWnd, GWL_STYLE ) );
    DWORD dwExStyle = static_cast<DWORD>( GetWindowLongPtr( m_hWnd, GWL_EXSTYLE ) );
    HMENU menu = GetMenu( m_hWnd );

    // Calculate the full size of the window needed to match our client area of width/height.
    RECT WindowRect = { 0, 0, (int32)width, (int32)height };
    AdjustWindowRectEx( &WindowRect, dwStyle, menu ? TRUE : FALSE, dwExStyle );

    int windowWidth = WindowRect.right - WindowRect.left;
    int windowHeight = WindowRect.bottom - WindowRect.top;

    SetWindowPos( m_hWnd, 0, x, y, windowWidth, windowHeight, 0 );
    if( maximized )
    {
        ShowWindow( m_hWnd, SW_MAXIMIZE );
    }
    ResizeWindow( width, height );
}

bool FWCore::IsKeyDown(uint32 value)
{
    assert( value < 256 );
    return m_KeyStates[value];
}

bool FWCore::IsMouseButtonDown(uint32 id)
{
    assert( id < 3 );
    return m_MouseButtonStates[id];
}

void FWCore::GetMouseCoordinates(int32* mx, int32* my)
{
    POINT p;
    if( GetCursorPos( &p ) )
    {
        if( ScreenToClient( m_hWnd, &p ) )
        {
            *mx = p.x;
            *my = p.y;
        }
    }
}

float FWCore::GetMouseWheel()
{
    return m_MouseWheel;
}

// Protected methods.

void FWCore::ResizeWindow(uint32 width, uint32 height)
{
    if( height <= 0 ) height = 1;
    if( width <= 0 ) width = 1;

    m_WindowClientWidth = width;
    m_WindowClientHeight = height;

    bgfx::reset( width, height, BGFX_RESET_VSYNC );
    //bgfx::setViewRect( 0, 0, 0, width, height );

    if( m_pGame )
    {
        WindowResizeEvent* pEvent = new WindowResizeEvent( width, height );
        m_pGame->GetEventManager()->AddEvent( pEvent );
    }
}

bool FWCore::CreateRenderWindow(char* title, uint32 width, uint32 height, uint8 colorBits, bool fullscreenflag)
{
    DWORD dwExStyle;
    DWORD dwStyle;

    RECT WindowRect;
    WindowRect.left = (long)0;
    WindowRect.right = (long)width;
    WindowRect.top = (long)0;
    WindowRect.bottom = (long)height;

    m_FullscreenMode = fullscreenflag;

    m_hInstance = GetModuleHandle( nullptr );       // Grab an instance for our window.

    // Define and register the window class.
    {
        WNDCLASSEX wc;
        ZeroMemory( &wc, sizeof(wc) );
        wc.cbSize = sizeof( wc );

        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // Redraw on move, and own DC for window.
        wc.lpfnWndProc = (WNDPROC)FWCore::WndProc;      // WndProc handles messages.
        wc.cbClsExtra = 0;                              // No extra window data.
        wc.cbWndExtra = 0;                              // No extra window data.
        wc.hInstance = m_hInstance;                     // Set the instance.
        wc.hIcon = LoadIcon( 0, IDI_WINLOGO );          // Load the default icon.
        wc.hCursor = LoadCursor( 0, IDC_ARROW );        // Load the arrow pointer.
        wc.hbrBackground = 0;                           // No background required.
        wc.lpszMenuName = nullptr;                      // We don't want a menu.
        wc.lpszClassName = "MyWindowClass";             // Set the class name.

        // Attempt to register the Window Class.
        if( !RegisterClassEx( &wc ) )
        {
            return FailAndCleanup( "Failed To Register The Window Class." );
        }
    }

    if( m_FullscreenMode )
    {
        DEVMODE dmScreenSettings;                                   // Device mode.
        memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) ); // Makes sure memory's cleared.
        dmScreenSettings.dmSize = sizeof( dmScreenSettings );       // Size of the devmode structure.
        dmScreenSettings.dmPelsWidth  = width;                      // Selected screen width.
        dmScreenSettings.dmPelsHeight = height;                     // Selected screen height.
        dmScreenSettings.dmBitsPerPel = colorBits;                  // Selected bits per pixel.
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Try to set selected mode and get results.  NOTE: CDS_FULLSCREEN gets rid of start bar.
        if( ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
        {
            // If the mode fails, offer two options.  Quit or run in a window.
            if( MessageBox( 0, "The requested fullscreen mode is not supported by\nyour video card.\nTry a different resolution.\nUse Windowed Mode Instead?", "", MB_YESNO|MB_ICONEXCLAMATION ) == IDYES )
            {
                m_FullscreenMode = false;
            }
            else
            {
                //return FailAndCleanup( "Program Will Now Close." );
                return false;
            }
        }
    }

    if( m_FullscreenMode )
    {
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle = WS_POPUP;
        ShowCursor( false );
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle = WS_OVERLAPPEDWINDOW;
    }

    AdjustWindowRectEx( &WindowRect, dwStyle, false, dwExStyle );   // Adjust window to true requested size.

    // Create our window.
    {
        m_hWnd = CreateWindowEx( dwExStyle,           // Extended style for the window.
                 "MyWindowClass",                     // Class name.
                 title,                               // Window title.
                 WS_CLIPSIBLINGS | WS_CLIPCHILDREN |  // Required window style.
                 dwStyle,                             // Selected window style.
                 0, 0,                                // Window position.
                 WindowRect.right-WindowRect.left,    // Calculate adjusted window width.
                 WindowRect.bottom-WindowRect.top,    // Calculate adjusted window height.
                 nullptr,                             // No parent window.
                 nullptr,                             // No menu.
                 m_hInstance,                         // Instance.
                 this );                              // Pass a pointer to this FWCore object to WM_NCCREATE.
        
        if( m_hWnd == nullptr )
        {
            return FailAndCleanup( "Window Creation Error." );
        }
    }

    return true;
}

bool FWCore::FailAndCleanup(const char* pMessage)
{
    DestroyRenderWindow( true );
    MessageBox( 0, pMessage, "ERROR", MB_OK|MB_ICONEXCLAMATION );
    return false;
}

void FWCore::DestroyRenderWindow(bool destroyInstance)
{
    if( m_FullscreenMode )
    {
        ChangeDisplaySettings( nullptr, 0 );
        ShowCursor( true );
    }

    if( m_hWnd && !DestroyWindow( m_hWnd ) )
    {
        MessageBox( 0, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
    }

    if( destroyInstance && m_hInstance != nullptr )
    {
        if( !UnregisterClass( "MyWindowClass", m_hInstance ) )
        {
            MessageBox( 0, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        }
        m_hInstance = 0;
    }

    m_hWnd = 0;
}

// This is a static method.
LRESULT CALLBACK FWCore::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! READ THIS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // This is a static method, which means it's essentially a global function.
    // It has no "this".
    // That means you can't access variables and methods directly.
    // As a member of the FWCore class it has full access to private
    //     variables and methods of any instance of the class.
    // To access the single instance of FWCore that we created
    //     you have to use the pFWCore pointer initialized just below.
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // Get a pointer to the FWCore object associated with this window.
    FWCore* pFWCore = (FWCore*)GetWindowLongPtr( hWnd, GWLP_USERDATA );

    switch( uMsg )
    {
    case WM_NCCREATE:
        {
            // Set the user data for this hWnd to the FWCore* we passed in, used on first line of this method above.
            CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>( lParam );
            FWCore* pFWCore = static_cast<FWCore*>( pCreateStruct->lpCreateParams );
            SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pFWCore ) );

            if( pFWCore )
            {
                pFWCore->m_hWnd = hWnd;
            }
        }
        return 1;

    case WM_DESTROY:
        {
            if( pFWCore )
            {
                pFWCore->m_hWnd = nullptr;
            }
        }
        return 0;

    case WM_ACTIVATE:
        {
            if( !HIWORD(wParam) )
            {
                pFWCore->m_WindowIsActive = true;
            }
            else
            {
                pFWCore->m_WindowIsActive = false;
            } 
        }
        return 0;

    case WM_SYSCOMMAND:
        {
            switch( wParam )
            {
            // Don't let screensaver or monitor power save mode kick in.
            case SC_SCREENSAVE:
            case SC_MONITORPOWER:
                return 0;
            }
        }
        break;

    case WM_CLOSE:
        {
            PostQuitMessage( 0 );
        }
        return 0;

    case WM_CHAR:
        {
            // Send a char event to the event manager.
            OnCharEvent* pEvent = new OnCharEvent( (uint32)wParam );
            pFWCore->m_pGame->GetEventManager()->AddEvent( pEvent );
        }
        return 0;

    case WM_KEYDOWN:
        {
            bool keyWasPressedLastTimeMessageArrived = lParam & (1 << 30);

            if( keyWasPressedLastTimeMessageArrived == false )
            {
                if( wParam == VK_ESCAPE && pFWCore->m_EscapeKeyWillQuit )
                    PostQuitMessage( 0 );

                pFWCore->m_KeyStates[wParam] = true;

                // Send a input event to the event manager.
                InputEvent* pEvent = new InputEvent(
                    InputEvent::DeviceType::Keyboard,
                    InputEvent::DeviceState::Pressed,
                    (uint32)wParam );

                pFWCore->m_pGame->GetEventManager()->AddEvent( pEvent );
            }
        }
        return 0;

    case WM_KEYUP:
        {
            pFWCore->m_KeyStates[wParam] = false;
    
            // Send a input event to the event manager.
            InputEvent* pEvent = new InputEvent(
                InputEvent::DeviceType::Keyboard,
                InputEvent::DeviceState::Released,
                (uint32)wParam );

            pFWCore->m_pGame->GetEventManager()->AddEvent( pEvent );
        }
        return 0;

    case WM_MOUSEMOVE:
        {
            int x = GET_X_LPARAM( lParam );
            int y = GET_Y_LPARAM( lParam );
        }
        return 0;

    case WM_LBUTTONDOWN:
        {
            SetCapture( pFWCore->m_hWnd );

            pFWCore->m_MouseButtonStates[0] = true;

            int x = GET_X_LPARAM( lParam );
            int y = GET_Y_LPARAM( lParam );
        }
        return 0;

    case WM_LBUTTONUP:
        {
            ReleaseCapture();
        
            pFWCore->m_MouseButtonStates[0] = false;

            int x = GET_X_LPARAM( lParam );
            int y = GET_Y_LPARAM( lParam );
        }
        return 0;

    case WM_RBUTTONDOWN:
        {
            SetCapture( pFWCore->m_hWnd );

            pFWCore->m_MouseButtonStates[1] = true;
        }
        return 0;

    case WM_RBUTTONUP:
        {
            ReleaseCapture();

            pFWCore->m_MouseButtonStates[1] = false;
        }
        return 0;

    case WM_MBUTTONDOWN:
        {
            SetCapture( pFWCore->m_hWnd );

            pFWCore->m_MouseButtonStates[2] = true;
        }
        return 0;

    case WM_MBUTTONUP:
        {
            ReleaseCapture();

            pFWCore->m_MouseButtonStates[2] = false;
        }
        return 0;

    case WM_MOUSEWHEEL:
        {
            pFWCore->m_MouseWheel = GET_WHEEL_DELTA_WPARAM(wParam) / 120.0f;
        }
        return 0;

    case WM_SIZE:
        {
            pFWCore->ResizeWindow( LOWORD(lParam), HIWORD(lParam) );
        }
        return 0;
    }

    // Pass all unhandled messages to DefWindowProc.
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

} // namespace fw
