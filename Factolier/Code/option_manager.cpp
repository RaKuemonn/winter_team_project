#include <imgui.h>
#include "option_manager.h"
#include "imgui.h"
#include "easing.h"

#define BGM 0
#define SE 180
#define CAMERA 360
#define RETURN 480
#define BAR_MAX 504
#define BAR_MIN 0


// �f�o�b�O�pGUI�`��
void Option_Manager::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(800, 100), ImGuiCond_FirstUseEver);
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
            ImGui::InputFloat("camera_move", &camera_move);
            ImGui::InputFloat("icon_eas_x", &icon_eas_x);
            ImGui::InputFloat("icon_pos_x", &icon_pos_x);
        }
    }
    ImGui::End();
}


Option_Manager::Option_Manager(ID3D11Device* device, ID3D11DeviceContext* context)
{
    immediate_context = context;
    title_back = make_unique<Sprite>(device, "./Data/Sprite/�I�v�V����.png");
    game_back = make_unique<Sprite>(device, "./Data/Sprite/�Q�[�����I�v�V����.png");

    File_IO::open("save.dat", binary_data);

    bar = make_unique<Sprite>(device, "./Data/Sprite/�I�v�V�����o�[.png");
    icon_spring = make_unique<Sprite>(device, "./Data/Sprite/�t�J�[�\��.png");
    icon_summer = make_unique<Sprite>(device, "./Data/Sprite/�ăJ�[�\��.png");
    icon_autam = make_unique<Sprite>(device, "./Data/Sprite/�H�J�[�\��.png");
    icon_winter = make_unique<Sprite>(device, "./Data/Sprite/�~�J�[�\��.png");
    arrow = make_unique<Sprite>(device, "./Data/Sprite/�o�[���(38,164).png");
    //option_bakc = make_unique<Sprite>(device, "./Data/Sprite/option_4��.png");


    // �������邩�p�[�Z���g�ŏo��
    // �`��͈́@��
    bgm_move = binary_data.bgm_bar / BAR_MAX;
    // BGM �{�����[���@%
    bgm_vo = 1 * bgm_move;
    // SE
    // ����Ă邱�Ƃ�BGM�Ɠ���
    se_move = binary_data.se_bar / BAR_MAX;
    se_vo = 1 * se_move;
    // �J�����̃o�[�𓮂�������
    // BGM�Ɠ���
    camera_move = binary_data.camera_bar / BAR_MAX;
}


void Option_Manager::update(float elapsedTime, Input_Manager* input_manager)
{
    slideshow(elapsedTime);
    if (icon_pos > 0 && down_flag == false)
    {
        if (input_manager->TRG(0) & PAD_UP)
        {
            up_flag = true;
        }
        if (up_flag)
        {
            if (time <= 0.5f)
            {
                time += elapsedTime;
                if (icon_pos > 360)
                {
                    icon_eas = -easing::out_quint(time, 0.5f, 120, 0.0f);
                    icon_eas_x = -easing::out_quint(time, 0.5f, 240, 0.0f);
                }
                else
                {
                    icon_eas = -easing::out_quint(time, 0.5f, 180, 0.0f);
                }
            }
            else
            {
                up_flag = false;

                time = 0.0f;
                if (icon_pos > 360)
                {
                    icon_pos -= 120;
                }
                else
                {
                    icon_pos -= 180;
                }
                icon_eas = 0.0f;
            }
        }
    }

    // �����@select 23
    if (icon_pos < 380 && up_flag == false)
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
                
                if (icon_pos >= 360)
                {
                    icon_eas = +easing::out_quint(time, 0.5f, 120, 0.0f);
                    icon_eas_x  = +easing::out_quint(time, 0.5f, 240, 0.0f);
                    
                }
                else
                {
                    icon_eas = +easing::out_quint(time, 0.5f, 180, 0.0f);
                }
            }
            else
            {
                down_flag = false;

                time = 0.0f;
                if (icon_pos >= 360)
                {
                    icon_pos += 120;
                   
                }
                else
                {
                    icon_pos += 180;
                }
                
                icon_eas = 0.0f;
            }
        }
    }
    if (icon_eas_x < 0)
    {
        icon_eas_x += 240;
    }
    setvolume(elapsedTime, input_manager);
   
    
}

void Option_Manager::title_update(float elapsedTime, Input_Manager* input_manager)
{
    slideshow(elapsedTime);
    if (icon_pos > 0 && down_flag == false)
    {
        if (input_manager->TRG(0) & PAD_UP)
        {
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

    // �����@select 23
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
    };
    if (icon_eas_x < 0)
    {
        icon_eas_x += 240;
    }
    setvolume(elapsedTime, input_manager);


}

void Option_Manager::setvolume(float elapsedTime, Input_Manager* input_manager)
{
    icon_select = static_cast<int>(icon_pos);
    // �A�C�R����BGM�̂Ƃ���ɂ�����

    //�A�C�R�����ړ����łȂ���
    if (!up_flag && !down_flag)
    {
        //�Q�[�W����
        switch (icon_select)
        {
        case BGM:
            if (binary_data.bgm_bar >= BAR_MIN && binary_data.bgm_bar <= BAR_MAX)
            {
                if (input_manager->STATE(0) & PAD_RIGHT)
                {
                    binary_data.bgm_bar += 4;
                }

                if (input_manager->STATE(0) & PAD_LEFT)
                {
                    binary_data.bgm_bar -= 4;
                }
            }
            if (binary_data.bgm_bar <= BAR_MIN)
            {
                binary_data.bgm_bar = BAR_MIN;
            }
            if (binary_data.bgm_bar >= BAR_MAX)
            {
                binary_data.bgm_bar = BAR_MAX;
            }

            return_flag = false;
            break;

        case SE:
            if (binary_data.se_bar >= BAR_MIN && binary_data.se_bar <= BAR_MAX)
            {
                if (input_manager->STATE(0) & PAD_RIGHT)
                {
                    binary_data.se_bar += 4;
                }

                if (input_manager->STATE(0) & PAD_LEFT)
                {
                    binary_data.se_bar -= 4;
                }
            }
            if (binary_data.se_bar <= BAR_MIN)
            {
                binary_data.se_bar = BAR_MIN;
            }
            if (binary_data.se_bar >= BAR_MAX)
            {
                binary_data.se_bar = BAR_MAX;
            }

            return_flag = false;
            break;

        case CAMERA:
            if (binary_data.camera_bar >= BAR_MIN && binary_data.camera_bar <= BAR_MAX)
            {
                if (input_manager->STATE(0) & PAD_RIGHT)
                {
                    binary_data.camera_bar += 4;
                }

                if (input_manager->STATE(0) & PAD_LEFT)
                {
                    binary_data.camera_bar -= 4;
                }
            }
            if (binary_data.camera_bar <= BAR_MIN)
            {
                binary_data.camera_bar = BAR_MIN;
            }
            if (binary_data.camera_bar >= BAR_MAX)
            {
                binary_data.camera_bar = BAR_MAX;
            }

            return_flag = false;
            break;

        case RETURN:
            return_flag = true;
            break;

        }
    }

    
    // �������邩�p�[�Z���g�ŏo��
    // �`��͈́@��
    bgm_move = binary_data.bgm_bar / BAR_MAX;
    // BGM �{�����[���@%
    bgm_vo = 1 * bgm_move;
    // SE
    // ����Ă邱�Ƃ�BGM�Ɠ���
    se_move = binary_data.se_bar / BAR_MAX;
    se_vo = 1 * se_move;
    // �J�����̃o�[�𓮂�������
    // BGM�Ɠ���
    camera_move = binary_data.camera_bar / BAR_MAX;
    // ���
    arrow_bgm = bgm_move * 504;
    arrow_se = se_move * 504;
    arrow_camera = camera_move * 504;
  
}

    
void Option_Manager::slideshow(float elapsedTime)
{
    slidetimer += elapsedTime / 8;
    slide = static_cast<int>(slidetimer) % 4;
    decimals = modff(slidetimer, &integer);

    if (slide == 0)
    {
        spring_alpha = 1;
        summer_alpha = 1;
        spring_alpha -= decimals;
    }
    else if (slide == 1)
    {
        summer_alpha = 1;
        autam_alpha = 1;
        summer_alpha -= decimals;
    }
    else if (slide == 2)
    {
        autam_alpha = 1;
        winter_alpha = 1;
        autam_alpha -= decimals;
    }
    else if (slide == 3)
    {
        winter_alpha = 1;
        winter_alpha -= decimals;
    }
}

// �Q�[�����ŕ\������I�v�V�������
void Option_Manager::game_render()
{
    game_back->render(
        immediate_context,
        0, 0,  //position
        1, 1,     // scal
        1920, 1080,    // �ǂꂭ�炢�`�悷�邩
        1920, 1080,   // size
        0, 0,         // pibot
        1, 1, 1, 1.0,   // rgba
        0); // angle


    // �A�C�R��
    {
        icon_spring->render(immediate_context,
            446 + icon_eas_x + icon_pos_x, 358 + icon_eas + icon_pos,
            1.0f, 1.0f,
            64, 64,
            64, 64,
            0, 0,
            1, 1, 1, 1,
            0);
        icon_winter->render(immediate_context,
            446 + icon_eas_x + icon_pos_x, 358 + icon_eas + icon_pos,
            1.0f, 1.0f,
            64, 64,
            64, 64,
            0, 0,
            1, 1, 1, winter_alpha,
            0);
        icon_autam->render(immediate_context,
            446 + icon_eas_x + icon_pos_x, 358 + icon_eas + icon_pos,
            1.0f, 1.0f,
            64, 64,
            64, 64,
            0, 0,
            1, 1, 1, autam_alpha,
            0);
        icon_summer->render(immediate_context,
            446 + icon_eas_x + icon_pos_x, 358 + icon_eas + icon_pos,
            1.0f, 1.0f,
            64, 64,
            64, 64,
            0, 0,
            1, 1, 1, summer_alpha,
            0);

        icon_spring->render(immediate_context,
            446 + icon_eas_x + icon_pos_x, 358 + icon_eas + icon_pos,
            1.0f, 1.0f,
            64, 64,
            64, 64,
            0, 0,
            1, 1, 1, spring_alpha,
            0);
    }

    //BGM�ݒ�
    {
        bar->render(
            immediate_context,
            862, 339,  //position
            1, 1,     // scal
            504, 116,    // �ǂꂭ�炢�`�悷�邩
            504 * bgm_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }

    // SE�ݒ�
    {
        bar->render(
            immediate_context,
            862, 516,  //position
            1, 1,     // scal
            504, 116,    // �ǂꂭ�炢�`�悷�邩
            504 * se_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }

    // �J�������x�ݒ�
    {

        bar->render(
            immediate_context,
            862, 693,  //position
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
        arrow_x = 844;
        arrow_y = 291;
        arrow_move = arrow_bgm;
        break;
    case SE:
        arrow_x = 844;
        arrow_y = 468;
        arrow_move = arrow_se;
        break;
    case CAMERA:
        arrow_x = 844;
        arrow_y = 645;
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


// �^�C�g���ŕ\������I�v�V�������
void Option_Manager::title_render()
{
    // �w�i
    title_back->render(
        immediate_context,
        0, 0,  //position
        1, 1,     // scal
        1920, 1080,    // �ǂꂭ�炢�`�悷�邩
        1920, 1080,   // size
        0, 0,         // pibot
        1, 1, 1, 1.0,   // rgba
        0); // angle


    // �A�C�R��
    {
        icon_spring->render(immediate_context,
            446 + icon_eas_x + icon_pos_x, 358 + icon_eas + icon_pos,
            1.0f, 1.0f,
            64, 64,
            64, 64,
            0, 0,
            1, 1, 1, 1,
            0);
        icon_winter->render(immediate_context,
            446 + icon_eas_x + icon_pos_x, 358 + icon_eas + icon_pos,
            1.0f, 1.0f,
            64, 64,
            64, 64,
            0, 0,
            1, 1, 1, winter_alpha,
            0);
        icon_autam->render(immediate_context,
            446 + icon_eas_x + icon_pos_x, 358 + icon_eas + icon_pos,
            1.0f, 1.0f,
            64, 64,
            64, 64,
            0, 0,
            1, 1, 1, autam_alpha,
            0);
        icon_summer->render(immediate_context,
            446 + icon_eas_x + icon_pos_x, 358 + icon_eas + icon_pos,
            1.0f, 1.0f,
            64, 64,
            64, 64,
            0, 0,
            1, 1, 1, summer_alpha,
            0);

        icon_spring->render(immediate_context,
            446 + icon_eas_x + icon_pos_x, 358 + icon_eas + icon_pos,
            1.0f, 1.0f,
            64, 64,
            64, 64,
            0, 0,
            1, 1, 1, spring_alpha,
            0);
    }



     //BGM�ݒ�
    {
        bar->render(
            immediate_context,
            862, 339,  //position
            1, 1,     // scal
            504, 116,    // �ǂꂭ�炢�`�悷�邩
            504 * bgm_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }

    // SE�ݒ�
    {
        bar->render(
            immediate_context,
            862, 516,  //position
            1, 1,     // scal
            504, 116,    // �ǂꂭ�炢�`�悷�邩
            504 * se_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }

    // �J�������x�ݒ�
    {

        bar->render(
            immediate_context,
            862, 693,  //position
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
        arrow_x = 844;
        arrow_y = 291;
        arrow_move = arrow_bgm;
        break;
    case SE:
        arrow_x = 844;
        arrow_y = 468;
        arrow_move = arrow_se;
        break;
    case CAMERA:
        arrow_x = 844;
        arrow_y = 645;
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