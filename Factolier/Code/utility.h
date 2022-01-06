#pragma once

#include <windows.h>


CONST LONG SCREEN_WIDTH{ 1280 };
CONST LONG SCREEN_HEIGHT{ 720 };
CONST BOOL FULLSCREEN{ FALSE };
CONST LPWSTR APPLICATION_NAME{ L"X3DGP" };


#define CAST_I(f) static_cast<int>(f)
#define CAST_F(i) static_cast<float>(i)


template <typename T>
inline void safe_delete(T*& p)
{
    if (p != nullptr)
    {
        delete (p);
        (p) = nullptr;
    }
}
