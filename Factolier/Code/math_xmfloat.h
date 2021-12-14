#pragma once

#include<DirectXMath.h>



//**********************************************************************************************************************************
// 
// XMFloat2ÇÃÉTÉ|Å[Ég
// 
//**********************************************************************************************************************************

//--------------------<ñﬂÇËílÇ™XMFloat2>--------------------//

// ë´ÇµéZ
inline DirectX::XMFLOAT2 operator+ (const DirectX::XMFLOAT2 a, const DirectX::XMFLOAT2 b) 
{
    using namespace DirectX;
    
    const XMVECTOR Ans = XMLoadFloat2(&a) + XMLoadFloat2(&b);

    XMFLOAT2 ret{};
    XMStoreFloat2(&ret, Ans);
    return ret;
}

// à¯Ç´éZ
inline DirectX::XMFLOAT2 operator- (const DirectX::XMFLOAT2 a, const DirectX::XMFLOAT2 b)
{
    using namespace DirectX;
    
    const XMVECTOR Ans = XMLoadFloat2(&a) - XMLoadFloat2(&b);

    XMFLOAT2 ret;
    XMStoreFloat2(&ret, Ans);
    return ret;
}

// ä|ÇØéZ
inline DirectX::XMFLOAT2 operator * (const DirectX::XMFLOAT2 a, const float b)
{
    using namespace DirectX;

    const DirectX::XMVECTOR RET = DirectX::XMVectorScale(XMLoadFloat2(&a), b);

    DirectX::XMFLOAT2 ret{};
    XMStoreFloat2(&ret, RET);
    return ret;
}

// ä|ÇØéZ
inline DirectX::XMFLOAT2 operator * (const float& b, const DirectX::XMFLOAT2& a)
{
    using namespace DirectX;

    const DirectX::XMVECTOR RET = DirectX::XMVectorScale(XMLoadFloat2(&a), b);

    DirectX::XMFLOAT2 ret{};
    XMStoreFloat2(&ret, RET);
    return ret;
}

// Å{ÅÅ
inline void operator += (DirectX::XMFLOAT2& xmf2_, const DirectX::XMFLOAT2& add_xmf2_)
{
    DirectX::XMStoreFloat2(&xmf2_, DirectX::XMVectorAdd(DirectX::XMLoadFloat2(&xmf2_), DirectX::XMLoadFloat2(&add_xmf2_)));
}

// -=
inline void operator -= (DirectX::XMFLOAT2& xmf2_, const DirectX::XMFLOAT2& subtract_xmf2_)
{
    DirectX::XMStoreFloat2(&xmf2_, DirectX::XMVectorSubtract(DirectX::XMLoadFloat2(&xmf2_), DirectX::XMLoadFloat2(&subtract_xmf2_)));
}

// ÅñÅÅ
inline void operator *= (DirectX::XMFLOAT2& a, const float& b)
{
    using namespace DirectX;

    const DirectX::XMVECTOR A = DirectX::XMVectorScale(XMLoadFloat2(&a), b);
    
    XMStoreFloat2(&a, A);
}
//**********************************************************************************************************************************
// 
// XMFloat3ÇÃÉTÉ|Å[Ég
// 
//**********************************************************************************************************************************

// ë´ÇµéZ
inline DirectX::XMFLOAT3 operator + (const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
    using namespace DirectX;
    
    const XMVECTOR Ans = XMLoadFloat3(&a) + XMLoadFloat3(&b);

    XMFLOAT3 ret;
    XMStoreFloat3(&ret, Ans);
    return ret;
}

// à¯Ç´éZ
inline DirectX::XMFLOAT3 operator - (const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
    using namespace DirectX;
    
    const XMVECTOR Ans = XMLoadFloat3(&a) - XMLoadFloat3(&b);

    XMFLOAT3 ret;
    XMStoreFloat3(&ret, Ans);
    return ret;
}

// ä|ÇØéZ
inline DirectX::XMFLOAT3 operator * (const DirectX::XMFLOAT3& a, const float& b)
{
    using namespace DirectX;

    const DirectX::XMVECTOR RET = DirectX::XMVectorScale(XMLoadFloat3(&a), b);

    DirectX::XMFLOAT3 ret{};
    XMStoreFloat3(&ret, RET);
    return ret;
}

// ä|ÇØéZ
inline DirectX::XMFLOAT3 operator * (const float& b, const DirectX::XMFLOAT3& a)
{
    using namespace DirectX;

    const DirectX::XMVECTOR RET = DirectX::XMVectorScale(XMLoadFloat3(&a), b);

    DirectX::XMFLOAT3 ret{};
    XMStoreFloat3(&ret, RET);
    return ret;
}

// äÑÇËéZ
inline DirectX::XMFLOAT3 operator / (const DirectX::XMFLOAT3& a, const float& b)
{
    using namespace DirectX;

    // 0Ç≈äÑÇÈÇ»
    assert(b != 0.0f);

#if 0
    // ïÅí 
    const DirectX::XMVECTOR RET = DirectX::XMVectorDivide(XMLoadFloat3(&a), DirectX::XMVectorReplicate(b));
#else
    // ê∏ìxóéÇøÇÈÇ™çÇë¨ÇÁÇµÇ¢
    const XMVECTOR R = DirectX::XMVectorReciprocalEst(DirectX::XMVectorReplicate(b));
    const DirectX::XMVECTOR RET = DirectX::XMVectorMultiply(DirectX::XMLoadFloat3(&a), R);
#endif


    DirectX::XMFLOAT3 ret{};
    XMStoreFloat3(&ret, RET);
    return ret;
}

// Å{ÅÅ
inline void operator += (DirectX::XMFLOAT3& xmf3_, const DirectX::XMFLOAT3& add_xmf3_)
{
    DirectX::XMStoreFloat3(&xmf3_, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&xmf3_), DirectX::XMLoadFloat3(&add_xmf3_)));
}

// *=
inline void operator *= (DirectX::XMFLOAT3& a, const float b)
{
    using namespace DirectX;

    const DirectX::XMVECTOR A = DirectX::XMVectorScale(XMLoadFloat3(&a), b);

    XMStoreFloat3(&a, A);
}

// *=
inline void operator *= (const float b, DirectX::XMFLOAT3& a)
{
    using namespace DirectX;

    const DirectX::XMVECTOR A = DirectX::XMVectorScale(XMLoadFloat3(&a), b);

    XMStoreFloat3(&a, A);
}

// -=
inline void operator -= (DirectX::XMFLOAT3& xmf3_, const DirectX::XMFLOAT3& subtract_xmf3_)
{
    DirectX::XMStoreFloat3(&xmf3_, DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&xmf3_), DirectX::XMLoadFloat3(&subtract_xmf3_)));
}


//**********************************************************************************************************************************
// 
//  XMFloat4ÇÃÉTÉ|Å[Ég
// 
//**********************************************************************************************************************************

// ë´ÇµéZ
inline DirectX::XMFLOAT4 operator + (const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b)
{
    using namespace DirectX;

    const XMVECTOR Ans = XMLoadFloat4(&a) + XMLoadFloat4(&b);

    XMFLOAT4 ret;
    XMStoreFloat4(&ret, Ans);
    return ret;
}

// à¯Ç´éZ
inline DirectX::XMFLOAT4 operator - (const DirectX::XMFLOAT4 a, const DirectX::XMFLOAT4 b)
{
    using namespace DirectX;

    const XMVECTOR A = XMLoadFloat4(&a);
    const XMVECTOR B = XMLoadFloat4(&b);
    const XMVECTOR Ans = A - B;
    XMFLOAT4 ret;

    XMStoreFloat4(&ret, Ans);
    return ret;
}


// *=
inline void operator *= (DirectX::XMFLOAT4& a, const float b)
{
    using namespace DirectX;

    const DirectX::XMVECTOR A = DirectX::XMVectorScale(XMLoadFloat4(&a), b);

    XMStoreFloat4(&a, A);
}


// *=
inline void operator *= (const float b, DirectX::XMFLOAT4& a)
{
    using namespace DirectX;

    const DirectX::XMVECTOR A = DirectX::XMVectorScale(XMLoadFloat4(&a), b);

    XMStoreFloat4(&a, A);
}