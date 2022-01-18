

#include "timer.h"

#include <string>

//#include "SpriteBatch.h"



void Timer::Initialize(ID3D11Device* Device, COUNT count, const float default_start_time_, const wchar_t* filename, int max_digit)
{
    timer = std::make_unique<Digits<float>>(default_start_time_, max_digit);

    // �O�����Z�q
    pCount = static_cast<int>(count) ? /* DOWN = true */&Timer::CountDown : /* Up = false */&Timer::CountUp;


    stop = false;


    // �`�悷��ꍇ�̂݃X�v���C�g�̎��̂𐶐�
    if (!filename) return;

    // �O�����Z�q
    //pRender = static_cast<int>(max_digit) ? /* (max_digit > 0) == true */&Timer::ClampSizeRender : /* (max_digits == 0) == false */&Timer::FreeSizeRender;

    if (max_digit <= 0) max_digit = 4 /*7���͂P000�̈�*/;
    //spr_number = std::make_unique<Sprite_Batch>(Device,filename,max_digit);
    //
    //number_size.x = static_cast<float>(spr_number->GetTextureWidth() / 10);
    //number_size.y = static_cast<float>(spr_number->GetTextureHeight());

}


void Timer::Update(float elapsedTime)
{
    if (stop) return;

    // ���Ԃ̍X�V
    (this->*pCount)(elapsedTime);
}


void Timer::SpriteRender(ID3D11DeviceContext* Immediate_Context, DirectX::XMFLOAT2 Pos)
{
    //if (spr_number == nullptr) return;
    //
    //
    ///* 2D�X�v���C�g�̕`�� */
    //(this->*pRender)(Immediate_Context, Pos);
}


void Timer::CountUp(float elapsedTime)
{
    // �^�C�}�[�����E�l�ɂȂ�����X�V���Ȃ�
    if (timer->GetDigit() == max_time) return;


    timer->Add(elapsedTime);


    //TODO: ���E�l 999.0f�@�܂�
    // ���E�n�𒴂�����
    if (timer->GetDigit() < max_time) return;
    timer->Set(max_time);

}


void Timer::CountDown(float elapsedTime)
{
    // �^�C�}�[���O�ɂȂ�����X�V���Ȃ�
    if (timer->GetDigit() == zero_time) return;


    timer->Subtract(-elapsedTime);


    // �[����艺�������
    if (timer->GetDigit() > zero_time) return;
    timer->Set(zero_time);
}


//void Timer::ClampSizeRender(ID3D11DeviceContext* Immediate_Context, const DirectX::XMFLOAT2& Pos)
//{
//    constexpr DirectX::XMFLOAT2 Scale = { 1.0f,1.0f };
//
//
//    std::vector<float> digit_place;
//    digit_place.resize(timer->max_digits_place);
//    timer->SliceDigits(digit_place);
//
//    spr_number->Begin(Immediate_Context);
//    for (size_t i = digit_place.size(); i > 0; --i)
//    {
//        spr_number->Render(Immediate_Context,
//            { Pos.x + number_size.x * (i - 1), Pos.y },
//            Scale,
//            { number_size.x * digit_place.at(digit_place.size() - i),0.0f },
//            number_size,
//            { 0.5f,0.5f }
//        );
//    }
//    spr_number->End(Immediate_Context);
//}
//
//
//void Timer::FreeSizeRender(ID3D11DeviceContext* Immediate_Context, const DirectX::XMFLOAT2& Pos)
//{
//    constexpr DirectX::XMFLOAT2 Scale = { 1.0f,1.0f };
//
//
//    // ���l�𕶎��ɕϊ�
//    std::string str_number = std::to_string(timer->GetDigit());
//
//    // ���݂̕����ʒu(���Έʒu)
//    float carriage = 0;
//
//    // �ꕶ���Â�Render()����
//    spr_number->Begin(Immediate_Context);
//    for (const char c : str_number)
//    {
//        int number = (c & 0x0F);
//        //������\���B�A�X�L�[�R�[�h�̈ʒu�ɂ��镶���ʒu��؂蔲���ĕ\��
//        spr_number->Render(Immediate_Context, { Pos.x + carriage,Pos.y }, Scale, { number_size.x * number, 0.0f }, number_size, { 0.5f,0.5f });
//
//        //�����ʒu�𕝕����炷
//        carriage += number_size.x * Scale.x;
//    }
//    spr_number->End(Immediate_Context);
//
//}