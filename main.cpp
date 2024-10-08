#include "cGameSettings.h"

#include "RPG_Engine.h"

//// Function to snap the console window to the primary screen
void SnapConsoleToPrimaryScreen()
{
    // Get the dimensions of the primary screen
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Set the console window size to match the screen resolution
    int consoleWidth = screenWidth-50;
    int consoleHeight = screenHeight-50;

    // Get the console window handle
    HWND hWnd = GetConsoleWindow();

    // Set the console window size
    SetWindowPos(hWnd, HWND_TOP, 0, 0, consoleWidth, consoleHeight, SWP_SHOWWINDOW);
}

// Function to handle window activation event
void HandleWindowActivation(HWND hWnd, WPARAM wParam)
{
    // Check if the console window is being activated
    if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)
    {
        // Reapply mouse snap functionality
        RECT windowRect;
        GetWindowRect(hWnd, &windowRect);
        ClipCursor(&windowRect);
    }
}

// Window procedure for the console window
LRESULT CALLBACK ConsoleWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_ACTIVATE:
        // Handle window activation event
        HandleWindowActivation(hWnd, wParam);
        break;
    default:
        // Call default window procedure for unhandled messages
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

int main()
{

    // Инициализация генератора случайных чисел на основе текущего времени
    srand(static_cast<unsigned int>(time(0)));
	// Load the settings singleton
	cGameSettings config;
	if (!config.LoadConfigFile("assets/config.lua"))
	{
		std::cout << "Failed to load '/assets/config.lua'" << std::endl;
		std::cout << "  -> Using default configuration" << std::endl;
	}
    // Snap console window to the primary screen
   SnapConsoleToPrimaryScreen();

    // Subclass the console window to intercept messages
    HWND hWnd = GetConsoleWindow();
    if (hWnd != nullptr)
    {
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)ConsoleWindowProc);
    }
	ShowCursor(0);
	//game.GetWindowMouse();
	//AllocConsole();

    srand(static_cast<unsigned int>(time(nullptr))); // Seed random number generator
	RPG_Engine game;

    game.fscale = 1.0f / config.nPixelWidth;
	ShowWindow(GetConsoleWindow(),1);
	if (game.Construct(config.nScreenWidth, config.nScreenHeight, config.nPixelWidth, config.nPixelHeight, true,false))
		game.Start();
  
  

	return 0;
}