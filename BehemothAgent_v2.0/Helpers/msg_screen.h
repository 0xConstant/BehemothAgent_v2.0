#pragma once

#include <windows.h>

// Function to create and show the full screen window
int ShowFullScreenWindow(HINSTANCE hInstance, int nCmdShow);

// Window Procedure
LRESULT CALLBACK FullScreenWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
