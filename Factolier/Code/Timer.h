#pragma once

#include <memory>
#include <DirectXMath.h>
#include "d3d11.h"
#include "digits.h"

enum class COUNT
{
    UP,
    DOWN,
};



// �^�C�}�[ ���ۑ� : ���l�\���̏��������,���Œ肾���łȂ�������悤�ɂ���

class Timer
{
public:
    Timer() = default;
    ~Timer() = default;

public:
    void Initialize(ID3D11Device* Device, COUNT count, const float default_start_time_, const wchar_t* filename = nullptr, int max_digit = 0);   // �ő包����ݒ肷�邩�ǂ����ŕ\�����@���ω�����
    void Update(float elapsedTime);
    void SpriteRender(ID3D11DeviceContext* Immediate_Context, DirectX::XMFLOAT2 Pos = {});
    void Finalize();

public: // Get�֐�
    float NowTime() { return timer->GetDigit(); }

public: // Set�֐�
    void Reset() { timer->Reset(); }
    void Add(const float add) { timer->Add(add); }
    void Subtract(const float subtract) { timer->Subtract(subtract); }
    void Stop() { stop = true; }
    void UnlockStop() { stop = false; }

private:
    //�@�֐��|�C���^ //
    using pFunc = void(Timer::*)(float);
    pFunc pCount = nullptr;

    // �֐��|�C���^�p�֐�
    void CountUp(float elapsedTime);
    void CountDown(float elapsedTime);


    //�@�֐��|�C���^ //
    using pFuncRender = void(Timer::*)(ID3D11DeviceContext*, const DirectX::XMFLOAT2&);
    pFuncRender pRender = nullptr;

    //// �����Œ�
    //void ClampSizeRender(ID3D11DeviceContext* Immediate_Context, const DirectX::XMFLOAT2& Pos);
    //// �����ϓ�����
    //void FreeSizeRender(ID3D11DeviceContext* Immediate_Context, const DirectX::XMFLOAT2& Pos);

private: // �萔
    static constexpr float zero_time = 0.0f;        // �[��
    static constexpr float max_time = 9999.0f;      // �ő�l

private: // �ϐ�
    std::unique_ptr<Digits<float>> timer;           // ���Ԑ��l
    //std::unique_ptr<Sprite_Batch> spr_number;       // �X�v���C�g
    DirectX::XMFLOAT2 number_size;                  // �ꕶ���̑傫��
    bool stop = false;                              // �ꎞ��~
};