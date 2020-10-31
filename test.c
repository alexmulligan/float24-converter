#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <windows.h>

#define APPNAME "FLOAT24_CONVERTER"
#define ID_HOTKEY_C 0xABCD
#define ID_HOTKEY_V 0xABCE

char szAppName[] = APPNAME;
char szTitle[]   = "FLOAT24 Converter";
const char *pWindowText;
const char *pWindowText2;
const char *pWindowText3;
bool inputValid;

void PositionWindow(HWND hWnd);

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hwndEdit;
    static HWND hwndStat;
	
	switch (message) {
        case WM_CREATE:
			//RegisterHotKey(hwnd, ID_HOTKEY_C, MOD_CONTROL, 0x43);
			//RegisterHotKey(hwnd, ID_HOTKEY_V, MOD_CONTROL, 0x56);
            PositionWindow(hwnd);
			
			hwndEdit = CreateWindowW(L"EDIT", NULL,
				WS_CHILD|WS_VISIBLE|WS_BORDER,
				70, 70, 150, 20, hwnd, (HMENU)2, NULL, NULL);
			
			CreateWindowW(L"BUTTON", L"Convert",
				WS_CHILD|WS_VISIBLE,
				260, 120, 80, 25,
				hwnd, (HMENU)1, NULL, NULL);
            
             hwndStat = CreateWindowW(L"STATIC", L"", 
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                373, 70, 150, 20, 
                hwnd, (HMENU) 3, NULL, NULL);
			
            break;

        case WM_DESTROY:
			//UnregisterHotKey(hwnd, ID_HOTKEY_C);
			//UnregisterHotKey(hwnd, ID_HOTKEY_V);
            PostQuitMessage(0);
            break;

		case WM_COMMAND:
		{
             if (LOWORD(wParam) == 1) {
                int len = GetWindowTextLengthW(hwndEdit) + 1; // 1 = empty; 2 = single char; # > 1 = # chars - 1
                wchar_t text[len];
                GetWindowTextW(hwndEdit, text, len);

                if (len == 1) { // check if input is blank
                    SetWindowTextW(hwndStat, L"");
                    MessageBoxW(NULL, L"Error: Decimal number cannot be blank.", L"Error", MB_OK);

                } else {
                    // check if input is valid - !!!Implement this!!!
                    inputValid = true;
                    // function to convert wchar_t to float
                    // function to calculate FLOAT24 from float

                    if (inputValid) {
                        SetWindowTextW(hwndStat, text);

                    } else { // default when input is not blank but is not valid - NOTE: Possibly implement more tests to give details about what is invalid
                        SetWindowTextW(hwndStat, L"");
                        MessageBoxW(NULL, L"Error: Input is invalid.", L"Error", MB_OK);

                    }
                }
            }

			break;
		}

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC         hdc;
            RECT        rc;
            hdc = BeginPaint(hwnd, &ps);

            GetClientRect(hwnd, &rc);
            rc.left = rc.left + 40;
            rc.top = rc.top + 20;
            SetTextColor(hdc, RGB(0, 0, 0));
            SetBkMode(hdc, TRANSPARENT);
            DrawText(hdc, pWindowText, -1, &rc, DT_LEFT);

			GetClientRect(hwnd, &rc);
			rc.left = 290;
			rc.top = 70;
			DrawText(hdc, pWindowText2, -1, &rc, DT_LEFT);

			GetClientRect(hwnd, &rc);
			rc.left = 380;
			rc.top = rc.top + 20;
			DrawText(hdc, pWindowText3, -1, &rc, DT_LEFT);

            EndPaint(hwnd, &ps);
            break;
        }

		/*case WM_HOTKEY:
		{
			if ((wParam) == ID_HOTKEY_C) {
				MessageBoxW(NULL, L"Ctrl+C Pressed", L"Hotkey", MB_OK);
			}
			if ((wParam) == ID_HOTKEY_V) {
				MessageBoxW(NULL, L"Ctrl+V Pressed", L"Hotkey", MB_OK);
			}
			break;
		}*/

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MSG msg;
    WNDCLASS wc;
    HWND hwnd;

    pWindowText = "Please enter a decimal number:";
	pWindowText2 = "->";
	pWindowText3 = "FLOAT24 Hex Value:";

    ZeroMemory(&wc, sizeof wc);
    wc.hInstance     = hInstance;
    wc.lpszClassName = szAppName;
    wc.lpfnWndProc   = (WNDPROC)WndProc;
    wc.style         = CS_DBLCLKS|CS_VREDRAW|CS_HREDRAW;
    wc.hbrBackground = (HBRUSH)GetStockObject(DC_BRUSH);
    wc.hIcon         = LoadIcon(NULL, IDC_ARROW);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

    if (FALSE == RegisterClass(&wc))
        return 0;

    hwnd = CreateWindow(
        szAppName,
        szTitle,
        WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        600,
        200,
        0,
        0,
        hInstance,
        0);

    if (NULL == hwnd)
        return 0;

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

void PositionWindow(HWND hwnd_self) {
    HWND hwnd_parent;
    RECT rw_self, rc_parent, rw_parent;
    int xpos, ypos;

    hwnd_parent = GetParent(hwnd_self);
    if (NULL == hwnd_parent)
        hwnd_parent = GetDesktopWindow();

    GetWindowRect(hwnd_parent, &rw_parent);
    GetClientRect(hwnd_parent, &rc_parent);
    GetWindowRect(hwnd_self, &rw_self);

    xpos = rw_parent.right - 650;
    ypos = 50;

    SetWindowPos(hwnd_self, NULL, xpos, ypos, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
}
