#pragma once
#include <DirectXMath.h>

class Character
{
public:
    Character() {}
    virtual ~Character() {}


private:
    void updateTransfrom();

protected:
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 angle;
    DirectX::XMFLOAT3 scale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4 transforn = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
};