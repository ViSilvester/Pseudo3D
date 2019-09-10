# include <windows.h>
# include <math.h>

int GetMousePosX(HWND *janela){
    POINT mouseP;
    RECT windP;
    GetCursorPos(&mouseP);
    SetCursorPos(mouseP.x, mouseP.y);
    GetWindowRect(*janela, &windP);
    return mouseP.x - windP.left-6;
}

int GetMousePosY(HWND *janela){
    POINT mouseP;
    RECT windP;
    GetCursorPos(&mouseP);
    GetWindowRect(*janela, &windP);
    return mouseP.y - windP.top-31;
}
