#pragma once

#include <d3d11.h>
#include "memory"
#include "sprite.h"
#include "input_manager.h"



class Option_Manager
{
public:
    Option_Manager(ID3D11Device* device, ID3D11DeviceContext* context);
    ~Option_Manager() = default;

    void update(float elapsedTime, Input_Manager* input_manager);
    void setvolume(float elapsedTime, Input_Manager* input_manager);
    // �f�o�b�O�pGUI�`��
    void DrawDebugGUI();
    void render();


public:
    float bgm_vo = 0;      // BGM�̃{�����[��
    float se_vo = 0;       // SE�̃{�����[��
    float camera_set = 0;  // �J�������x�̐ݒ�

    float icon_eas = 0;     // �A�C�R���̈ړ�����
    float icon_pos = 0;     //�@�A�C�R���̈ʒu�C��

    float time = 0; // �o�ߎ���

    bool up_flag = false;
    bool down_flag = false;

    

private:
    ID3D11DeviceContext* immediate_context;

    std::unique_ptr<Sprite> back = nullptr;
    std::unique_ptr<Sprite> meter_back = nullptr;
    std::unique_ptr<Sprite> bgm = nullptr;
    std::unique_ptr<Sprite> se = nullptr;
    std::unique_ptr<Sprite> camera = nullptr;
    std::unique_ptr<Sprite> option = nullptr;
    std::unique_ptr<Sprite> bar = nullptr;
    std::unique_ptr<Sprite> icon = nullptr;
};
