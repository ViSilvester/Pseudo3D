# include <windows.h>
# include "drawFunctions.h"



LRESULT CALLBACK WindowProc(HWND,UINT,WPARAM,LPARAM);
void EnableOpenGL(HWND,HDC*,HGLRC*);
typedef struct { HWND hwnd; HDC hDC; HGLRC hRC; int largura; int altura; LPSTR *Title; } Janela;
int CriarJanela(WNDCLASSEX *wcex, LPSTR titulo);


#ifdef _MSC_VER
#    pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif





int StartWindow(LPSTR lpCmdLine, int nCmdShow, Janela *wind)
{
    WNDCLASSEX wcex;
    CriarJanela(&wcex, wind->Title);


	////////////////////////////////////////

	HINSTANCE hInstance = GetModuleHandle(NULL);
	HINSTANCE hPrevInstance = NULL;






    ///////////////////////////////////////////////////////////////////////

    if (!RegisterClassEx(&wcex))
        return -1;

    wind->hwnd = CreateWindowEx(0,wind->Title,wind->Title,WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,CW_USEDEFAULT,wind->largura,wind->altura,
                          NULL,NULL,hInstance,NULL);

    ShowWindow(wind->hwnd, nCmdShow);



    ////////////////////////////////////////////////////////////////////////

    EnableOpenGL(wind->hwnd, &(wind->hDC), &(wind->hRC));



	


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, wind->largura, wind->altura, 0.0f, -1, 1);
	glMatrixMode(GL_MODELVIEW);



    return 0;

}


int CriarJanela(WNDCLASSEX *wcex, LPSTR titulo){


    HINSTANCE hInstance=NULL;

    wcex->cbSize = sizeof(WNDCLASSEX);
    wcex->style = CS_OWNDC;
    wcex->lpfnWndProc = WindowProc;
    wcex->cbClsExtra = 0;
    wcex->cbWndExtra = 0;
    wcex->hInstance = hInstance;
    wcex->hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex->hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex->hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex->lpszMenuName = NULL;
    wcex->lpszClassName = titulo;
    wcex->hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

