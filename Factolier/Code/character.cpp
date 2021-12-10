#include "character.h"

void Character::updatetransfrom()
{
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.z, scale.y, scale.z);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    DirectX::CXMMATRIX W = S * R * T;
    DirectX::XMStoreFloat4x4(&world, W);
}