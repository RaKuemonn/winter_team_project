#pragma once
#include <DirectXMath.h>

struct Hit_Result
{
    DirectX::XMFLOAT3   position        = { 0,0,0 };    // ���Ă����C�L���X�g�Ɠ��������|���S���̌�_
    DirectX::XMFLOAT3   normal          = { 0,0,0 };    // ���������|���S���̖@���x�N�g��
    float               distance        = 0.0f;                   // ���C�L���X�g�̎n�_~�I�_�܂ł̋���
    int                 material_index  = -1;                     // ���������|���S���̃}�e���A���ԍ�
    DirectX::XMFLOAT3   rotation        = { 0,0,0 };    // ��]��
};