#pragma once

#include <d3d11.h>
#include "memory"
#include "sprite.h"
#include "binary_loader.h"
#include "input_manager.h"



#define CAMERA_MAX 2.0f



enum class Stage_Select
{
    STAGE_1 = 0,
    STAGE_2,
    STAGE_3,
    STAGE_4,
    STAGE_BOSS
};


class Option_Manager
{
public:
    Option_Manager(ID3D11Device* device, ID3D11DeviceContext* context);
    ~Option_Manager() { File_IO::write("./Data/Binary/save.dat", binary_data); }

    void update(float elapsedTime, Input_Manager* input_manager);
    void title_update(float elapsedTime, Input_Manager* input_manager);
    void setvolume(float elapsedTime, Input_Manager* input_manager);
    // �f�o�b�O�pGUI�`��
    void DrawDebugGUI();
    void title_render();
    void slideshow(float elapsedTime);
    void game_render();

    void set_next_stage(Stage_Select next_stage) { now_stage = next_stage; }
    Stage_Select get_now_stage() { return now_stage; }
    Binary_Data& get_binary() { return binary_data; }

public:
    float bgm_vo = 0;         // BGM�̃{�����[�� 1~0�̊Ԃőւ������Ă���
    //float bgm_bar = 126;      // BGM�̃{�����[���o�[�̒��g���ǂꂭ�炢�\�������邩
    float bgm_move = 0;       // BGM�̃{�����[���o�[�̒��g�̕`�悪�S�̂̉��p�[�Z���g��
    
    float se_vo = 0;          // SE�̃{�����[�� 1~0�̊Ԃőւ������Ă���
    //float se_bar = 126;       // SE�̃{�����[���o�[�̒��g���ǂꂭ�炢�\�������邩
    float se_move = 0;        // SE�̃{�����[���o�[�̒��g�̕`�悪�S�̂̉��p�[�Z���g��
    
    //float camera_bar = 252;     // �J�������x�o�[�̒��g���ǂꂭ�炢�\�������邩
    float camera_move = 0;    // �J�������x�o�[�̒��g�̕`�悪�S�̂̉��p�[�Z���g��

    float icon_eas = 0;     // �A�C�R���̈ړ�����
    float icon_eas_x = 0;     // �A�C�R���̈ړ����� x
    float icon_pos = 0;     //�@�A�C�R���̈ʒu�C��
    float icon_pos_x = 0;     // �A�C�R���̈ړ����� x
    float icon_count = 0;

    float time = 0; // �o�ߎ���

    float arrow_x = 0; // ����x�̈ʒu
    float arrow_y = 0; // ����y�̈ʒu
    float arrow_move = 0; // ���̃g�[�^���̈ړ���
    float arrow_bgm = 0; //���̈ړ��� BGM
    float arrow_se = 0; //���̈ړ��� SE
    float arrow_camera = 0; //���̈ړ��� �J����

    bool up_flag = false;
    bool down_flag = false;

    int icon_select = 0;
    float icon_size = 1.0f;

    float spring_alpha = 1;
    float summer_alpha = 1;
    float autam_alpha = 1; 
    float winter_alpha = 0;

    float slidetimer = 0.0f; // �X���C�h�V���[�p�̃^�C�}�[
    int slide = 0; //�X���C�h�V���[�̐؂�ւ��^�C�~���O�̍��v
    float integer = 0;  // slidetimer�̐�������
    float decimals = 0;// slidetimer�̏�������

    bool return_flag = false; //�Z���N�g�̖߂�


private:
    ID3D11DeviceContext* immediate_context;

    std::unique_ptr<Sprite> title_back = nullptr;
    std::unique_ptr<Sprite> game_back = nullptr;
    std::unique_ptr<Sprite> option_bakc = nullptr;

    Stage_Select now_stage = Stage_Select::STAGE_1;
    Binary_Data binary_data {};

    std::unique_ptr<Sprite> bar_back = nullptr;
    std::unique_ptr<Sprite> bgm = nullptr;
    std::unique_ptr<Sprite> se = nullptr;
    std::unique_ptr<Sprite> camera = nullptr;
    std::unique_ptr<Sprite> bar = nullptr;

    std::unique_ptr<Sprite> icon_spring = nullptr;
    std::unique_ptr<Sprite> icon_summer = nullptr;
    std::unique_ptr<Sprite> icon_autam = nullptr;
    std::unique_ptr<Sprite> icon_winter = nullptr;

    std::unique_ptr<Sprite> arrow = nullptr;

};
