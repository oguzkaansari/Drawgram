// Drawgram.cpp : Uygulamanın giriş noktasını tanımlar.
//
#include "framework.h"
#include "Drawgram.h"
#include "OpenGL.h"
#include "WindowsX.h"
#include "Commdlg.h"
#include <commctrl.h>
#include <string>


#define MAX_LOADSTRING 100

// Genel Değişkenler:
HINSTANCE hInst;                                // geçerli örnek
WCHAR szTitle[MAX_LOADSTRING];                  // Başlık çubuğu metni
WCHAR szWindowClass[MAX_LOADSTRING];            // ana pencere sınıfı adı
OpenGL gl;
HGLRC hRC;
HDC hDC;
RECT WINDOW_SIZE;
const int buttonWidth = 50;
const int buttonHeight = 50;
HWND glWin;
HWND lineWidthTrack;
HBITMAP hBlack, hBlue, hBrown, hGray, hGreen, hLightBlue, hOrange, hPurple, hRed, hWhite, hYellow, hLightGreen;
bool didMouseMoved; // mouse un basılı tutulduktan veya tek tıklamadan sonra bırakıldığını belirtir.
const int scrollLength = 50; // Birim kaydırma başına kayan pixel sayısı
int COLOR_TARGET = 0;

// Bu kod modülündeki işlevlerin bildirimlerini ilet:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                LoadButtonImages();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Kodu buraya yerleştirin.

    // Genel dizeleri başlat
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DRAWGRAM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Uygulama başlatması gerçekleştir:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAWGRAM));

    MSG msg;

    // Ana ileti döngüsü:
    while (1)

    {

        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (GetMessage(&msg, NULL, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }else
            {
                return msg.wParam;
            }
        }
        //gl.DrawGLScene();
        glFlush();
        SwapBuffers(hDC);
    }

}



//
//  İŞLEV: MyRegisterClass()
//
//  AMAÇ: Pencere sınıfını kaydeder.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAWGRAM));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DRAWGRAM);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   İŞLEV: InitInstance(HINSTANCE, int)
//
//   AMAÇ: Örnek tanıtıcısını kaydeder ve ana pencereyi oluşturur
//
//   AÇIKLAMALAR:
//
//        Bu işlevde, örnek tanıtıcısını genel değişkene kaydederiz ve
//        ana program penceresini oluşturur ve görüntüleriz.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Örnek tanıtıcısını genel değişkenimizde depola
   //int WINDOW_WIDTH = GetSystemMetrics(SM_CXVIRTUALSCREEN);
   //int WINDOW_HEIGHT = GetSystemMetrics(SM_CYVIRTUALSCREEN);
   LoadButtonImages();
   SystemParametersInfo(SPI_GETWORKAREA, 0, &WINDOW_SIZE, 0);
   HWND mainWin = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
      0, 0, WINDOW_SIZE.right, WINDOW_SIZE.bottom, nullptr, nullptr, hInstance, nullptr);

   if (!mainWin)
   {
      return FALSE;
   }  
   
   HWND penButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"PEN",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
       0,         // x position 
       10,         // y position 
       buttonWidth,        // Button width
       buttonHeight,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_PEN,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.
   
   if (!penButton)
   {
       return FALSE;
   }

   HWND lineButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"LINE",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
       buttonWidth + 10,         // x position 
       10,         // y position 
       buttonWidth,        // Button width
       buttonHeight,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_LINE,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   if (!lineButton)
   {
       return FALSE;
   }
   HWND rectButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"RECT",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
       buttonWidth*2 + 10,         // x position 
       10,         // y position 
       buttonWidth,        // Button width
       buttonHeight,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_RECT,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   if (!rectButton)
   {
       return FALSE;
   }

   HWND triButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"TRI",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
       buttonWidth * 3 + 10,         // x position 
       10,         // y position 
       buttonWidth,        // Button width
       buttonHeight,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_TRI,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   if (!triButton)
   {
       return FALSE;
   }

   HWND pentaButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"PENTA",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
       buttonWidth * 4 + 10,         // x position 
       10,         // y position 
       buttonWidth,        // Button width
       buttonHeight,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_PENTA,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   if (!pentaButton)
   {
       return FALSE;
   }

   HWND hexaButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"HEXA",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
       buttonWidth * 5 + 10,         // x position 
       10,         // y position 
       buttonWidth,        // Button width
       buttonHeight,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_HEXA,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   if (!hexaButton)
   {
       return FALSE;
   }

   HWND circleButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"CHMBR",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
       buttonWidth * 6 + 10,         // x position 
       10,         // y position 
       buttonWidth,        // Button width
       buttonHeight,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_CHMBR,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   if (!circleButton)
   {
       return FALSE;
   }

   HWND ellipseButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"ELPS",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
       buttonWidth * 7 + 10,         // x position 
       10,         // y position 
       buttonWidth,        // Button width
       buttonHeight,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_ELLIPSE,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   if (!ellipseButton)
   {
       return FALSE;
   }

   HWND eraserButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"ERSR",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
       buttonWidth * 8 + 10,         // x position 
       10,         // y position 
       buttonWidth,        // Button width
       buttonHeight,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_ERSR,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   if (!eraserButton)
   {
       return FALSE;
   }

   HWND fillButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"FILL",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
       buttonWidth * 11 + 10,         // x position 
       5,         // y position 
       100,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_FILL,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   if (!fillButton)
   {
       return FALSE;
   }

   HWND drawFilledButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"FILL(DRAW)",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
       buttonWidth * 11 + 10,         // x position 
       30,         // y position 
       100,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_DRAW_FILLED,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   if (!drawFilledButton)
   {
       return FALSE;
   }
   
   HWND saveButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"SAVE",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
       buttonWidth * 14 + 10,         // x position 
       10,         // y position 
       buttonWidth,        // Button width
       buttonHeight,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_SAVE,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   if (!saveButton)
   {
       return FALSE;
   }

   lineWidthTrack = CreateWindow(
       TRACKBAR_CLASS,
       L"",
       WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_HORZ |
       TBS_TOOLTIPS | TBS_FIXEDLENGTH,
       buttonWidth * 9 + 350, //x position  
       25, //y position
       120, //width
       30,  //height
       mainWin,
       (HMENU)ID_TRACKBAR_LINE_WIDTH,
       hInstance,
       NULL);

   HWND trackBarTopLabel = CreateWindow(
       L"Static", 
       L"Line Width",
       WS_CHILD | WS_VISIBLE, 
       buttonWidth * 9 + 375, 
       5, 
       70, 
       15, 
       mainWin, 
       (HMENU)1, 
       NULL, 
       NULL);

   SendMessage(lineWidthTrack, TBM_SETRANGE, true, MAKELONG(0/*MIN RANGE*/, 10/*MAX RANGE*/)); 
   SendMessage(lineWidthTrack, TBM_SETPOS, TRUE, 2);

   
   if (!lineWidthTrack)
   {
       return FALSE;
   }
  
   HWND extendRightButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L">",      // Button text 
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
       WINDOW_SIZE.right - 36,         // x position 
       0 + buttonWidth + 10,         // y position 
       20,        // Button width
       WINDOW_SIZE.bottom - 140,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_EXTND_RIGHT,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   if (!extendRightButton)
   {
       return FALSE;
   }
   
   HWND extendDownButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L">",      // Button text 
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
       0,         // x position 
       WINDOW_SIZE.bottom - 80,         // y position 
       WINDOW_SIZE.right - 35,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_EXTND_DOWN,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   if (!extendDownButton)
   {
       return FALSE;
   }

   HWND blackButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       NULL,      // Button text 
       WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT,  // Styles 
       buttonWidth * 12 + 350,         // x position 
       5,         // y position 
       20,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_BLACK,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   SendMessageW(blackButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBlack);

   if (!blackButton)
   {
       return FALSE;
   }
   HWND whiteButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       NULL,      // Button text 
       WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT,  // Styles 
       buttonWidth * 12 + 350,         // x position 
       30,         // y position 
       20,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_WHITE,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   SendMessageW(whiteButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hWhite);

   if (!whiteButton)
   {
       return FALSE;
   }
   HWND grayButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       NULL,      // Button text 
       WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT,  // Styles 
       buttonWidth * 12 + 375,         // x position 
       5,         // y position 
       20,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_GRAY,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   SendMessageW(grayButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hGray);

   if (!grayButton)
   {
       return FALSE;
   }

   HWND brownButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       NULL,      // Button text 
       WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT,  // Styles 
       buttonWidth * 12 + 375,         // x position 
       30,         // y position 
       20,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_BROWN,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   SendMessageW(brownButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBrown);

   if (!brownButton)
   {
       return FALSE;
   }

   HWND blueButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       NULL,      // Button text 
       WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT,  // Styles 
       buttonWidth * 12 + 400,         // x position 
       5,         // y position 
       20,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_BLUE,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   SendMessageW(blueButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBlue);

   if (!blueButton)
   {
       return FALSE;
   }

   HWND lightBlueButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       NULL,      // Button text 
       WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT,  // Styles 
       buttonWidth * 12 + 400,         // x position 
       30,         // y position 
       20,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_LIGHTBLUE,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   SendMessageW(lightBlueButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLightBlue);

   if (!lightBlueButton)
   {
       return FALSE;
   }

   HWND redButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       NULL,      // Button text 
       WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT,  // Styles 
       buttonWidth * 12 + 425,         // x position 
       5,         // y position 
       20,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_RED,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   SendMessageW(redButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hRed);

   if (!redButton)
   {
       return FALSE;
   }

   HWND purpleButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       NULL,      // Button text 
       WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT,  // Styles 
       buttonWidth * 12 + 425,         // x position 
       30,         // y position 
       20,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_PURPLE,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   SendMessageW(purpleButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hPurple);

   if (!purpleButton)
   {
       return FALSE;
   }

   HWND greenButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       NULL,      // Button text 
       WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT,  // Styles 
       buttonWidth * 12 + 450,         // x position 
       5,         // y position 
       20,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_GREEN,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   SendMessageW(greenButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hGreen);

   if (!greenButton)
   {
       return FALSE;
   }

   HWND lightGreenButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       NULL,      // Button text 
       WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT,  // Styles 
       buttonWidth * 12 + 450,         // x position 
       30,         // y position 
       20,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_LIGHTGREEN,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   SendMessageW(lightGreenButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLightGreen);

   if (!lightGreenButton)
   {
       return FALSE;
   }

   HWND orangeButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       NULL,      // Button text 
       WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT,  // Styles 
       buttonWidth * 12 + 475,         // x position 
       5,         // y position 
       20,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_ORANGE,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   SendMessageW(orangeButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hOrange);

   if (!orangeButton)
   {
       return FALSE;
   }

   HWND yellowButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       NULL,      // Button text 
       WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT,  // Styles 
       buttonWidth * 12 + 475,         // x position 
       30,         // y position 
       20,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_YELLOW,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   SendMessageW(yellowButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hYellow);

   if (!yellowButton)
   {
       return FALSE;
   }

   HWND changeDrawColorButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"DRAW",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
       buttonWidth * 12 + 500,         // x position 
       5,         // y position 
       100,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_CHANGE_DRAW_COLOR,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   if (!changeDrawColorButton)
   {
       return FALSE;
   }

   HWND changeBgColorButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"BACKGROUND",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
       buttonWidth * 12 + 500,         // x position 
       30,         // y position 
       100,        // Button width
       20,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_CHANGE_BG_COLOR,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   if (!changeBgColorButton)
   {
       return FALSE;
   }

   HWND undoButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"UNDO",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
       WINDOW_SIZE.right - 100,         // x position 
       10,         // y position 
       buttonWidth,        // Button width
       buttonHeight,        // Button height
       mainWin,     // Parent window
       (HMENU)ID_BUTTON_UNDO,       // No menu.
       (HINSTANCE)GetWindowLongPtr(mainWin, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   if (!undoButton)
   {
       return FALSE;
   }
   
   ShowWindow(mainWin, SW_SHOWMAXIMIZED);
   UpdateWindow(mainWin);

   return TRUE;
}

//
//  İŞLEV: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  AMAÇ: Ana pencere için iletileri işler.
//
//  WM_COMMAND  - uygulama menüsünü işle
//  WM_PAINT    - Ana pencereyi boya
//  WM_DESTROY  - bir çıkış iletisi gönder ve dön
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    POINTS pt;
    switch (message)
    {
    case WM_CREATE:
    {   
        //GetClientRect(hWnd, &gl.MainWinRect);
        HWND glWin = CreateWindow(
            szWindowClass,  // Predefined class; Unicode assumed 
            L"GLWindow",      
            WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CHILD,  // Styles 
            0,         // x position 
            buttonWidth + 10,         // y position 
            WINDOW_SIZE.right - 36,        // Window width
            WINDOW_SIZE.bottom - 140,        // Window height
            hWnd,     // Parent window
            NULL,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HWNDPARENT ),
            NULL);      // Pointer not needed.

        if (!glWin)
        {
            return FALSE;
        }
        if (gl.CreateGLContexts(glWin, hDC, hRC))
        {
            GetClientRect(glWin, &gl.GLRect); 
            gl.InitGL();
            
        }
        else {
            PostQuitMessage(0);
        }


        break;
    }
    case WM_SIZE:
    {
        /*
        GetClientRect(hWnd, &gl.ClientRect);
        HWND childWindow = GetWindow(hWnd, GW_CHILD);
        SetWindowPos(
            childWindow,
            hWnd,
            gl.ClientRect.left + buttonWidth,
            gl.ClientRect.top,
            gl.ClientRect.right - buttonWidth,
            gl.ClientRect.bottom,
            SWP_NOZORDER);
        GetClientRect(glWin, &gl.ClientRect);
        gl.ResizeGLScene();
        */
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Menü seçimlerini ayrıştır:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_BUTTON_PEN:
                glColor3f(gl.DrawColor[0], gl.DrawColor[1], gl.DrawColor[2]);
                gl.DRAW_STATE = 1;                
                break;
            case ID_BUTTON_LINE:
                gl.DRAW_STATE = 2;
                break;            
            case ID_BUTTON_TRI:
                gl.DRAW_STATE = 3;
                break;
            case ID_BUTTON_RECT:
                gl.DRAW_STATE = 4;
                break;
            case ID_BUTTON_PENTA:
                gl.DRAW_STATE = 5;
                break;
            case ID_BUTTON_HEXA:
                gl.DRAW_STATE = 6;
                break;
            case ID_BUTTON_CHMBR:
                gl.DRAW_STATE = 7;
                break;
            case ID_BUTTON_ELLIPSE:
                gl.DRAW_STATE = 8;
                break;
            case ID_BUTTON_ERSR:
                gl.DRAW_STATE = 9;
                break;
            case ID_BUTTON_FILL:
                gl.DRAW_STATE = 0;  
                break;
            case ID_BUTTON_SAVE:               
                OPENFILENAMEA ofn;
                char file_path[100];
                ZeroMemory(&ofn, sizeof(OPENFILENAME));
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFile = file_path;
                ofn.lpstrFile[0] = '\0';
                ofn.nMaxFile = 100;
                ofn.lpstrFilter = 
                    "Image files (*.jpg;*.png;*.bmp)\0*.jpg;*.png;*.bmp\0"
                    "All files\0*.*\0";
                ofn.nFilterIndex = 1;

                GetSaveFileNameA(&ofn);
                gl.SaveGLWindow(ofn.lpstrFile);
                break;            
            case ID_BUTTON_DRAW_FILLED:
                if (gl.filled == 0)
                {
                    gl.filled = 1;
                }
                else
                {
                    gl.filled = 0;
                }
                break;
            case ID_BUTTON_BLACK:
                gl.ChangeGLColor(0, 0, 0, COLOR_TARGET);
                break;
            case ID_BUTTON_BLUE:
                gl.ChangeGLColor(0, 0, 255, COLOR_TARGET);
                break;
            case ID_BUTTON_BROWN:
                gl.ChangeGLColor(139, 69, 19, COLOR_TARGET);
                break;
            case ID_BUTTON_GRAY:
                gl.ChangeGLColor(128, 128, 128, COLOR_TARGET);
                break;
            case ID_BUTTON_GREEN:
                gl.ChangeGLColor(0, 100, 0, COLOR_TARGET);
                break;
            case ID_BUTTON_LIGHTBLUE:
                gl.ChangeGLColor(173, 216, 230, COLOR_TARGET);
                break;
            case ID_BUTTON_LIGHTGREEN:
                gl.ChangeGLColor(127, 255, 0, COLOR_TARGET);
                break;
            case ID_BUTTON_ORANGE:
                gl.ChangeGLColor(255, 165, 0, COLOR_TARGET);
                break;
            case ID_BUTTON_PURPLE:
                gl.ChangeGLColor(128, 0, 128, COLOR_TARGET);
                break;
            case ID_BUTTON_RED:
                gl.ChangeGLColor(255, 0, 0, COLOR_TARGET);
                break;
            case ID_BUTTON_WHITE:
                gl.ChangeGLColor(255, 255, 255, COLOR_TARGET);
                break;
            case ID_BUTTON_YELLOW:
                gl.ChangeGLColor(255, 255, 0, COLOR_TARGET);
                break;
            case ID_BUTTON_CHANGE_DRAW_COLOR:
                COLOR_TARGET = 0;
                break;
            case ID_BUTTON_CHANGE_BG_COLOR:
                COLOR_TARGET = 1;
                break;
            case ID_BUTTON_EXTND_RIGHT:                  
                gl.ResizeGLWindow(0);                
                break;
            case ID_BUTTON_EXTND_DOWN:
                gl.ResizeGLWindow(1);
                break;
            case ID_BUTTON_UNDO:
                if (!gl.objects.empty())
                {
                    gl.objects.pop_back();
                    glClear(GL_COLOR_BUFFER_BIT);
                    gl.RedrawGLScene();
                }                
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    
    case WM_LBUTTONDOWN:
    {
        didMouseMoved = false;
        //mouse un sol tıkına basıldı. DrawGLScene fonskiyonu mesaj döngüsü yerine buradan da çağırılabiliyor.  
        pt = MAKEPOINTS(lParam);
        gl.pStartX = -1.0 + pt.x * (2.0 / gl.GLRect.right) + (1.0 / gl.GLRect.right);
        gl.pStartY = -1.0 + pt.y * (2.0 / gl.GLRect.bottom) + (1.0 / gl.GLRect.bottom);
        
        if (gl.DRAW_STATE == 1 || gl.DRAW_STATE == 9) 
        {
            gl.penInnerPoints.push_back(gl.pStartX);
            gl.penInnerPoints.push_back(gl.pStartY);
        }
        
        if (gl.DRAW_STATE == 0)
        {
            gl.FillGLObject();
        }
        SetCapture(hWnd);

        return 0;
    }
    case WM_LBUTTONUP:    
    {
        //mouse un sol tıkı bırakıldı
        ReleaseCapture();
        if(didMouseMoved)
        {
            gl.SavePoints(gl.DRAW_STATE);
            gl.RedrawGLScene();        
        }
       
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        // mouse hareket ettirliyor. Burada sol tıka basılı olup olmadığı da kontrol edilecek ve basılıysa çizim yapılacak.
        DWORD flags = (DWORD)wParam;
        if ((flags & MK_LBUTTON) && gl.DRAW_STATE != 0) {
            didMouseMoved = true;
            pt = MAKEPOINTS(lParam);            
            gl.pEndX = -1.0 + pt.x * (2.0 / gl.GLRect.right) + (1.0 / gl.GLRect.right);
            gl.pEndY = -1.0 + pt.y * (2.0 / gl.GLRect.bottom) + (1.0 / gl.GLRect.bottom);
            gl.DrawGLScene();
            
        }        
        return 0;
    }    
    case WM_VSCROLL:    //dikey kaydırma
    {
        
       /* HWND childWindow = GetWindow(hWnd, GW_CHILD);
        SetWindowPos(
            childWindow,
            hWnd,
            0,
            0,
            gl.GLRect.right,
            gl.GLRect.bottom + scrollLength,
            SWP_NOZORDER | SWP_NOMOVE
        );
        gl.ScrollGLWindow(0, scrollLength);*/
        break;
    }
    case WM_HSCROLL:    //yatay kaydırma
    {
        // Trackbar kontrolü

        int position = SendMessage(lineWidthTrack, TBM_GETPOS, 0, 0);
        glLineWidth(position);
        gl.lineWidth = position;
        //gl.ScrollGLWindow(1, 50);

        break;
    }   
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Hakkında kutusu için ileti işleyicisi.
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

void LoadButtonImages()
{
    hBlack = (HBITMAP)LoadImageW(NULL, L"Colors\\black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBlue = (HBITMAP)LoadImageW(NULL, L"Colors\\blue.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBrown = (HBITMAP)LoadImageW(NULL, L"Colors\\brown.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hGray = (HBITMAP)LoadImageW(NULL, L"Colors\\gray.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hGreen = (HBITMAP)LoadImageW(NULL, L"Colors\\green.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hLightBlue = (HBITMAP)LoadImageW(NULL, L"Colors\\lightblue.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hOrange = (HBITMAP)LoadImageW(NULL, L"Colors\\orange.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hPurple = (HBITMAP)LoadImageW(NULL, L"Colors\\purple.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hRed = (HBITMAP)LoadImageW(NULL, L"Colors\\red.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hWhite = (HBITMAP)LoadImageW(NULL, L"Colors\\white.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hYellow = (HBITMAP)LoadImageW(NULL, L"Colors\\yellow.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hLightGreen = (HBITMAP)LoadImageW(NULL, L"Colors\\lightgreen.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    
}


