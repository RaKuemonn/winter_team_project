#pragma once

#include<DirectXMath.h>



//**********************************************************************************************************************************
// 
// XMFloat2�̃T�|�[�g
// 
//**********************************************************************************************************************************

//--------------------<�߂�l��XMFloat2>--------------------//

// �����Z
inline DirectX::XMFLOAT2 operator+ (const DirectX::XMFLOAT2 a, const DirectX::XMFLOAT2 b) 
{
    using namespace DirectX;

    const XMVECTOR A = XMLoadFloat2(&a);
    const XMVECTOR B = XMLoadFloat2(&b);
    const XMVECTOR Ans = A + B;
    XMFLOAT2 ret{};

    XMStoreFloat2(&ret, Ans);
    return ret;
}

// �����Z
inline DirectX::XMFLOAT2 operator- (const DirectX::XMFLOAT2 a, const DirectX::XMFLOAT2 b)
{
    using namespace DirectX;

    const XMVECTOR A = XMLoadFloat2(&a);
    const XMVECTOR B = XMLoadFloat2(&b);
    const XMVECTOR Ans = A - B;
    XMFLOAT2 ret;

    XMStoreFloat2(&ret, Ans);
    return ret;
}

// �|���Z
inline DirectX::XMFLOAT2 operator* (const DirectX::XMFLOAT2 a, const DirectX::XMFLOAT2 b)
{
    using namespace DirectX;

    const XMVECTOR A = XMLoadFloat2(&a);
    const XMVECTOR B = XMLoadFloat2(&b);
    const XMVECTOR Ans = A * B;
    XMFLOAT2 ret;

    XMStoreFloat2(&ret, Ans);
    return ret;
}

// ����Z
inline DirectX::XMFLOAT2 operator/ (const DirectX::XMFLOAT2 a, const DirectX::XMFLOAT2 b)
{
    using namespace DirectX;

    const XMVECTOR A = XMLoadFloat2(&a);
    const XMVECTOR B = XMLoadFloat2(&b);
    const XMVECTOR Ans = A / B;
    XMFLOAT2 ret;

    XMStoreFloat2(&ret, Ans);
    return ret;
}


// �|���Z
inline DirectX::XMFLOAT2 operator * (const DirectX::XMFLOAT2 a, const float b)
{
    using namespace DirectX;

    const DirectX::XMFLOAT2 ret = {
        a.x * b,
        a.y * b
    };

    return ret;
}

// �{��
inline DirectX::XMFLOAT2 operator += (DirectX::XMFLOAT2 xmf2_, const DirectX::XMFLOAT2& add_xmf2_)
{
    DirectX::XMFLOAT2 added_xmf2 = {};
    DirectX::XMStoreFloat2(&added_xmf2, DirectX::XMVectorAdd(DirectX::XMLoadFloat2(&xmf2_), DirectX::XMLoadFloat2(&add_xmf2_)));
    return added_xmf2;
}

// ����
inline void operator *= (DirectX::XMFLOAT2& a, const float& b)
{
    using namespace DirectX;
    a.x *= b;
    a.y *= b;
}
//**********************************************************************************************************************************
// 
// XMFloat3�̃T�|�[�g
// 
//**********************************************************************************************************************************

// �����Z
inline DirectX::XMFLOAT3 operator + (const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
    using namespace DirectX;

    const XMVECTOR A = XMLoadFloat3(&a);
    const XMVECTOR B = XMLoadFloat3(&b);
    const XMVECTOR Ans = A + B;
    XMFLOAT3 ret;

    XMStoreFloat3(&ret, Ans);
    return ret;
}

// �����Z
inline DirectX::XMFLOAT3 operator - (const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
    using namespace DirectX;

    const XMVECTOR A = XMLoadFloat3(&a);
    const XMVECTOR B = XMLoadFloat3(&b);
    const XMVECTOR Ans = A - B;
    XMFLOAT3 ret;

    XMStoreFloat3(&ret, Ans);
    return ret;
}

// �|���Z
inline DirectX::XMFLOAT3 operator * (const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
    using namespace DirectX;

    const XMVECTOR A = XMLoadFloat3(&a);
    const XMVECTOR B = XMLoadFloat3(&b);
    const XMVECTOR Ans = A * B;
    XMFLOAT3 ret;

    XMStoreFloat3(&ret, Ans);
    return ret;
}

// �|���Z
inline DirectX::XMFLOAT3 operator * (const DirectX::XMFLOAT3& a, const float& b)
{
    using namespace DirectX;

    const DirectX::XMFLOAT3 ret = {
        a.x * b,
        a.y * b,
        a.z * b
    };

    return ret;
}

// �|���Z
inline DirectX::XMFLOAT3 operator * (const float& b, const DirectX::XMFLOAT3& a)
{
    using namespace DirectX;

    const DirectX::XMFLOAT3 ret = {
        a.x * b,
        a.y * b,
        a.z * b
    };

    return ret;
}

// �{��
inline DirectX::XMFLOAT3 operator += (DirectX::XMFLOAT3 xmf3_, const DirectX::XMFLOAT3& add_xmf3_)
{
    DirectX::XMFLOAT3 added_xmf3 = {};
    DirectX::XMStoreFloat3(&added_xmf3, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&xmf3_), DirectX::XMLoadFloat3(&add_xmf3_)));
    return added_xmf3;
}


// *=
inline void operator *= (DirectX::XMFLOAT3& a, const float b)
{
    using namespace DirectX;


    a.x *= b;
    a.y *= b;
    a.z *= b;
}

// *=
inline void operator *= (const float b, DirectX::XMFLOAT3& a)
{
    using namespace DirectX;


    a.x *= b;
    a.y *= b;
    a.z *= b;
}

// -=
inline void operator -= (DirectX::XMFLOAT3& a,DirectX::XMFLOAT3& b)
{
    using namespace DirectX;


    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
}


//**********************************************************************************************************************************
// 
//  XMFloat4�̃T�|�[�g
// 
//**********************************************************************************************************************************

// �����Z
inline DirectX::XMFLOAT4 operator + (const DirectX::XMFLOAT4 a, const DirectX::XMFLOAT4 b)
{
    using namespace DirectX;

    const XMVECTOR A = XMLoadFloat4(&a);
    const XMVECTOR B = XMLoadFloat4(&b);
    const XMVECTOR Ans = A + B;
    XMFLOAT4 ret;

    XMStoreFloat4(&ret, Ans);
    return ret;
}

// �����Z
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


    a.x *= b;
    a.y *= b;
    a.z *= b;
    a.w *= b;
}


// *=
inline void operator *= (const float b, DirectX::XMFLOAT4& a)
{
    using namespace DirectX;


    a.x *= b;
    a.y *= b;
    a.z *= b;
    a.w *= b;
}

