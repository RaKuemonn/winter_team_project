#pragma once


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
