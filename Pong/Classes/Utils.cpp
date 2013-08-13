#include "Utils.h"

namespace utils {
    
    void positionInWindow(HWND hWnd, int *x, int *y) {
        HWND parent = GetParent(hWnd);
        POINT p = {0};

        MapWindowPoints(hWnd, parent, &p, 1);
        (*x) = p.x;
        (*y) = p.y;
    }
}