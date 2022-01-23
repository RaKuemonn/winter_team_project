#include <imgui.h>
#include "option_manager.h"
#include "imgui.h"
#include "easing.h"

#define BGM 0
#define SE 180
#define CAMERA 360
#define BAR_MAX 504
#define BAR_MIN 0


// �f�o�b�O�pGUI�`��
void Option_Manager::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Option", nullptr, ImGuiWindowFlags_None))
    {
        // �g�����X�t�H�[��
        if (ImGui::CollapsingHeader("transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::InputFloat("icon_eas", &icon_eas);
            ImGui::InputFloat("icon_pos", &icon_pos);
            ImGui::InputFloat("time", &time);
            ImGui::Checkbox("up_flag", &up_flag);
            ImGui::Checkbox("down_flag", &down_flag);
            ImGui::InputFloat("bgm_vo", &bgm_vo);
            ImGui::InputFloat("bgm_move", &bgm_move);
            ImGui::InputFloat("arrow_move", &arrow_move);
        }
    }
    ImGui::End();
}


Option_Manager::Option_Manager(ID3D11Device* device, ID3D11DeviceContext* context)
{
    immediate_context = context;
    back = make_unique<Sprite>(device, "Data/back.png");

    File_IO::open("save.dat", binary_data);

    bgm = make_unique<Sprite>(device, "Data/����_bgm��.png");
    se = make_unique<Sprite>(device, "Data/����_se��.png");
    camera = make_unique<Sprite>(device, "Data/�J����_��.png");
    bar_back = make_unique<Sprite>(device, "Data/option_1��.png");
    bar = make_unique<Sprite>(device, "Data/option_2��.png");
    option = make_unique<Sprite>(device, "Data/option_3��.png");
    icon = make_unique<Sprite>(device, "Data/team1_flower_90�~90.png");
    arrow = make_unique<Sprite>(device, "Data/�o�[���(38,164).png");
}


void Option_Manager::update(float elapsedTime, Input_Manager* input_manager)
{
    if (icon_pos > 0 && down_flag == false)
    {
        if (input_manager->TRG(0) & PAD_UP)
        {
            icon_eas -= 180;
            up_flag = true;
        }
        if (up_flag)
        {
            if (time <= 0.5f)
            {
                time += elapsedTime;
                icon_eas = -easing::out_quint(time, 0.5f, 180, 0.0f);
            }
            else
            {
                up_flag = false;

                time = 0.0f;
                icon_pos -= 180;
                icon_eas = 0.0f;
            }
        }
    }

    if (icon_pos < 360 && up_flag == false)
    {
        if (input_manager->TRG(0) & PAD_DOWN)
        {
            down_flag = true;
        }
        if (down_flag)
        {
            if (time <= 0.5f)
            {
                time += elapsedTime;
                icon_eas = +easing::out_quint(time, 0.5f, 180, 0.0f);
            }
            else
            {
                down_flag = false;

                time = 0.0f;
                icon_pos += 180;
                icon_eas = 0.0f;
            }
        }
    }

    setvolume(elapsedTime, input_manager);
   
    
}

void Option_Manager::setvolume(float elapsedTime, Input_Manager* input_manager)
{
    icon_select = static_cast<int>(icon_pos);
    // �A�C�R����BGM�̂Ƃ���ɂ�����

    switch (icon_select)
    {
    case BGM:
        if (bgm_bar >= BAR_MIN && bgm_bar <= BAR_MAX)
        {
            if (input_manager->STATE(0) & PAD_RIGHT)
            {
                bgm_bar += 4;
            }

            if (input_manager->STATE(0) & PAD_LEFT)
            {
                bgm_bar -= 4;
            }
        }
        if (bgm_bar <= BAR_MIN)
        {
            bgm_bar = BAR_MIN;
        }
        if (bgm_bar >= BAR_MAX)
        {
            bgm_bar = BAR_MAX;
        }
        break;

    case SE:
        if (se_bar >= BAR_MIN && se_bar <= BAR_MAX)
        {
            if (input_manager->STATE(0) & PAD_RIGHT)
            {
                se_bar += 4;
            }

            if (input_manager->STATE(0) & PAD_LEFT)
            {
                se_bar -= 4;
            }
        }
        if (se_bar <= BAR_MIN)
        {
            se_bar = BAR_MIN;
        }
        if (se_bar >= BAR_MAX)
        {
            se_bar = BAR_MAX;
        }
        break;

    case CAMERA:
        if (se_bar >= BAR_MIN && se_bar <= BAR_MAX)
        {
            if (input_manager->STATE(0) & PAD_RIGHT)
            {
                camera_bar += 4;
            }

            if (input_manager->STATE(0) & PAD_LEFT)
            {
                camera_bar -= 4;
            }
        }
        if (camera_bar <= BAR_MIN)
        {
            camera_bar = BAR_MIN;
        }
        if (camera_bar >= BAR_MAX)
        {
            camera_bar = BAR_MAX;
        }
        break;
    }
    // �������邩�p�[�Z���g�ŏo��
    // �`��͈́@��
    bgm_move = bgm_bar / BAR_MAX;
    // BGM �{�����[���@%
    bgm_vo = 1 * bgm_move;
    // SE
    // ����Ă邱�Ƃ�BGM�Ɠ���
    se_move = se_bar / BAR_MAX;
    se_vo = 1 * se_move;
    // �J�����̃o�[�𓮂�������
    // BGM�Ɠ���
    camera_move = camera_bar / BAR_MAX;
    // ���
    arrow_bgm = bgm_move * 504;
    arrow_se = se_move * 504;
    arrow_camera = camera_move * 504;
  
}
    



void Option_Manager::render()
{
    // �w�i
    back->render(
        immediate_context,
        300, 180,  //position
        1, 1,     // scal
        1280, 720,    // �ǂꂭ�炢�`�悷�邩
        1280, 720,   // size
        0, 0,         // pibot
        1, 1, 1, 0.5,   // rgba
        0); // angle

    //�I�v�V����
    option->render(
        immediate_context,
        650, 200,  //position
        1, 1,     // scal
        516, 128,    // �ǂꂭ�炢�`�悷�邩
        516, 128,   // size
        0, 0,         // pibot
        1, 1, 1, 1.0,   // rgba
        0); // angle

    // �A�C�R��
    icon->render(immediate_context,
        330, 340 + icon_eas + icon_pos,
        1.0f, 1.0f,
        128, 128,
        128, 128,
        0, 0,
        1, 1, 1, 1,
        0);



    // BGM�ݒ�
    {
        bgm->render(
            immediate_context,
            500, 340,  //position
            1, 1,     // scal
            256, 128,    // �ǂꂭ�炢�`�悷�邩
            256, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle

        bar_back->render(
            immediate_context,
            800, 340,  //position
            1, 1,     // scal
            516, 128,    // �ǂꂭ�炢�`�悷�邩
            516, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle

        bar->render(
            immediate_context,
            806, 346,  //position
            1, 1,     // scal
            504, 116,    // �ǂꂭ�炢�`�悷�邩
            504 * bgm_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }

    // SE�ݒ�
    {
        se->render(
            immediate_context,
            500, 520,  //position
            1, 1,     // scal
            256, 128,    // �ǂꂭ�炢�`�悷�邩
            256, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle

        bar_back->render(
            immediate_context,
            800, 520,  //position
            1, 1,     // scal
            516, 128,    // �ǂꂭ�炢�`�悷�邩
            516, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
        bar->render(
            immediate_context,
            806, 526,  //position
            1, 1,     // scal
            504, 116,    // �ǂꂭ�炢�`�悷�邩
            504 * se_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }

    // �J�������x�ݒ�
    {
        camera->render(
            immediate_context,
            500, 700,  //position
            1, 1,     // scal
            256, 128,    // �ǂꂭ�炢�`�悷�邩
            256, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle


        bar_back->render(
            immediate_context,
            800, 700,  //position
            1, 1,     // scal
            516, 128,    // �ǂꂭ�炢�`�悷�邩
            516, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
        bar->render(
            immediate_context,
            806, 706,  //position
            1, 1,     // scal
            504, 116,    // �ǂꂭ�炢�`�悷�邩
            504 * camera_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }


    // ���
    switch (icon_select)
    {
    case BGM:
        arrow_x = 790;
        arrow_y = 298;
        arrow_move = arrow_bgm;
        break;
    case SE:
        arrow_x = 790;
        arrow_y = 478;
        arrow_move = arrow_se;
        break;
    case CAMERA:
        arrow_x = 790;
        arrow_y = 658;
        arrow_move = arrow_camera;
        break;
    }
    arrow->render(immediate_context,
        arrow_x + arrow_move, arrow_y,
        1.0f, 1.0f,
        38, 164,
        38, 164,
        0, 0,
        1, 1, 1, 1,
        0);

    DrawDebugGUI();
}
