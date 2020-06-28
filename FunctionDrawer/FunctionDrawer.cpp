// FunctionDrawer.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
//#include "windows.h"
#include "math.h"
#include <iostream>
//#include "Parser.h"
#include "BusinessLogic.h"
#include "FunctionDrawer.h"

#define MAX_LOADSTRING 100
#define BUTTON_ID      1001
#define CANVAS_WIDTH 650
#define CANVAS_HEIGHT 500
#define ID_LIST   1

// ���������� ����������:
HINSTANCE hInst;                                // ������� ���������
WCHAR szTitle[MAX_LOADSTRING];                  // ����� ������ ���������
WCHAR szWindowClass[MAX_LOADSTRING];            // ��� ������ �������� ����
HWND hWndCanvas, hWndPoints, hWndFormula;
bool paint = false;
double(*ptFunc)(double) = sinusFun;
int NUM = 70;
// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���������� ��� �����.

    // ������������� ���������� �����
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FUNCTIONDRAWER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ��������� ������������� ����������:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FUNCTIONDRAWER));

    MSG msg;

    // ���� ��������� ���������:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  �������: MyRegisterClass()
//
//  ����������: ������������ ����� ����.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FUNCTIONDRAWER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FUNCTIONDRAWER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �������: InitInstance(HINSTANCE, int)
//
//   ����������: ��������� ��������� ���������� � ������� ������� ����.
//
//   �����������:
//
//        � ������ ������� ���������� ���������� ����������� � ���������� ����������, � �����
//        ��������� � ��������� �� ����� ������� ���� ���������.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��������� ���������� ���������� � ���������� ����������

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
		
   // ���� ��� ��������� �������� Canvas
	hWndCanvas = CreateWindowEx(WS_EX_CLIENTEDGE, L"Static", L"", WS_CHILD | WS_VISIBLE,
	   400, 0, CANVAS_WIDTH, CANVAS_HEIGHT, hWnd, (HMENU)10000, hInstance, NULL);


	hWndPoints = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"70", WS_CHILD | WS_VISIBLE | ES_RIGHT,
											230, 20, 80, 30, hWnd, (HMENU)10000, hInstance, NULL);

	//hWndFormula = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_RIGHT,
		//230, 250, 80, 30, hWnd, (HMENU)10000, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �������: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����������:  ������������ ��������� � ������� ����.
//
//  WM_COMMAND � ���������� ���� ����������
//  WM_PAINT � ���������� ������� ����
//  WM_DESTROY � ��������� ��������� � ������ � ���������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hbtnGenerate, hbtnGetNum, hWndList;
//	double(*ptFunc)(double) = NULL; //&sinusFun;


    switch (message)
    {
    case WM_COMMAND:
        {
			//if(lParam == )
			if (lParam == (LPARAM)hbtnGetNum) {
				TCHAR buf[256];
				GetWindowText(hWndPoints, buf, lstrlen(buf));
				if (buf[0] != 0) {
					int numOfPoints = _tstoi(buf); 
					if(numOfPoints > 10 && numOfPoints < 1000) {
						NUM = numOfPoints;
					}
					else {
						MessageBox(
							NULL,
							L"����� ����� ������� ������ ���� � ��������� [10, 999]",
							L"������ �����!",
							MB_OK | //�� |���
							MB_DEFBUTTON1 | //��������� ������
							MB_ICONEXCLAMATION | //��� ������
							MB_DEFAULT_DESKTOP_ONLY
							);
					}
					
				}
				else {
					MessageBox(
						NULL,
						L"�� �� ����� ���������� ����� �������",
						L"������ �����!",
						MB_OK | //�� |���
						MB_DEFBUTTON1 | //��������� ������
						MB_ICONEXCLAMATION | //��� ������
						MB_DEFAULT_DESKTOP_ONLY
						);
				}
			}

			else if (lParam == (LPARAM)hbtnGenerate) {
				paint = true;

				int currentIndex = SendMessage(hWndList, LB_GETCARETINDEX, 0, wParam);
				switch (currentIndex)
				{
				case 0:
					ptFunc = &cosinusFun;
					break;
				case 1:
					ptFunc = &expFun;
					break;
				case 2:
					ptFunc = &sinusFun;
					break;
				case 3:
					ptFunc = &tangensFun;
					break;
				case 4:
					ptFunc = &parabolaFun;
					break;
				case 5:
					ptFunc = &giperbolaFun;
					break;
				default:
					MessageBox(
						NULL,
						L"������",
						L"������ �����!",
						MB_OK | //�� |���
						MB_DEFBUTTON1 | //��������� ������
						MB_ICONEXCLAMATION | //��� ������
						MB_DEFAULT_DESKTOP_ONLY
						);
				}
				
				//SendMessage(hWnd, WM_PAINT, 0, 0);

				InvalidateRect(hWnd, NULL, true);
				
			}
		
            int wmId = LOWORD(wParam);
            // ��������� ����� � ����:
            switch (wmId)
            {

			case 106:
				if (HIWORD(wParam) == LBN_DBLCLK) {
					MessageBox(
						NULL,
						L"��dssd����",
						L"dd!",
						MB_OK | //�� |���
						MB_DEFBUTTON1 | //��������� ������
						MB_ICONEXCLAMATION | //��� ������
						MB_DEFAULT_DESKTOP_ONLY
						);
				}
				
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_CREATE:
		{
			hbtnGenerate = CreateWindow(L"button", L"��������� ������",
				WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
				20, 200,
				150, 50,
				hWnd, (HMENU)BUTTON_ID,
				hInst, NULL);
			hbtnGetNum = CreateWindow(L"button", L"OK",
				WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
				20, 60,
				150, 50,
				hWnd, (HMENU)BUTTON_ID,
				hInst, NULL);
			hWndList = CreateWindowEx(WS_EX_CLIENTEDGE, L"listbox", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_STANDARD |
				LBS_WANTKEYBOARDINPUT, 200, 125, 100, 100, hWnd, (HMENU)ID_LIST, NULL, hInst);
			SendMessage(hWndList, LB_ADDSTRING, 0,	(LPARAM)L"y=cos(x)");
			SendMessage(hWndList, LB_ADDSTRING, 0, (LPARAM)L"y=sin(x)");
			SendMessage(hWndList, LB_ADDSTRING, 0, (LPARAM)L"y=tg(x)");
			SendMessage(hWndList, LB_ADDSTRING, 0, (LPARAM)L"y=x^2");
			SendMessage(hWndList, LB_ADDSTRING, 0, (LPARAM)L"y=x^3");
			SendMessage(hWndList, LB_ADDSTRING, 0, (LPARAM)L"y=exp(x)");
		}
    case WM_PAINT:
        {
            PAINTSTRUCT ps, psC;
            HDC hdc = BeginPaint(hWnd, &ps);
			HDC hdcC = BeginPaint(hWndCanvas, &psC);

			if (paint) {
				//HDC hdcC = GetDC(hWnd);
			//	HDC hdcC = BeginPaint(hWndCanvas, &ps);
			//	HDC hdc = GetDC(hWndCanvas);
				double ** x = getData(NUM, ptFunc);
				DrawGraph(hdcC, psC.rcPaint, x, NUM); // ���������� �������
			//	ReleaseDC(hWndCanvas, hdc);
				ValidateRect(hWndCanvas, NULL);
				/*
				PAINTSTRUCT ps1;
				HDC hdcC = BeginPaint(hWndCanvas, &ps1);
				HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
				SelectObject(hdcC, hBrush);
				Ellipse(hdcC, 100 + 1, 100 + 1, 30, 50);
				EndPaint(hWndCanvas, &ps1);*/
			}

			TextOut(hdc, 20, 25, L"������� ����� ����� �������", 28); // ����� ��������� ���������
			TextOut(hdc, 20, 125, L"�������� �������", 17);
			
			EndPaint(hWndCanvas, &ps);
		   EndPaint(hWnd, &ps);

        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���������� ��������� ��� ���� "� ���������".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
