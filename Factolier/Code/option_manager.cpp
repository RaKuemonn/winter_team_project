#include <imgui.h>
#include "option_manager.h"
#include "easing.h"

// ÉfÉoÉbÉOópGUIï`âÊ
void Option_Manager::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Option", nullptr, ImGuiWindowFlags_None))
    {
        // ÉgÉâÉìÉXÉtÉHÅ[ÉÄ
        if (ImGui::CollapsingHeader("transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::InputFloat("icon_eas", &icon_eas);
            ImGui::InputFloat("icon_pos", &icon_pos);
            ImGui::InputFloat("time", &time);
            ImGui::Checkbox("up_flag", &up_flag);
            ImGui::Checkbox("down_flag", &down_flag);
            ImGui::InputFloat("bgm_vo", &bgm_vo);
        }
    }
    ImGui::End();
}


Option_Manager::Option_Manager(ID3D11Device* device, ID3D11DeviceContext* context)
{
    immediate_context = context;
    back = make_unique<Sprite>(device, "Data/back.png");
    bgm = make_unique<Sprite>(device, "Data/âπó _bgmâº.png");
    se = make_unique<Sprite>(device, "Data/âπó _seâº.png");
    camera = make_unique<Sprite>(device, "Data/ÉJÉÅÉâ_âº.png");
    meter_back = make_unique<Sprite>(device, "Data/option_1âº.png");
    bar = make_unique<Sprite>(device, "Data/option_2âº.png");
    option = make_unique<Sprite>(device, "Data/option_3âº.png");
    icon = make_unique<Sprite>(device, "Data/team1_flower_90Å~90.png");
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
    if (icon_pos == 0)
    {
        if (input_manager->STATE(0) & PAD_RIGHT)
        {            
            bgm_vo += 1;
        }
        if (input_manager->STATE(0) & PAD_LEFT)
        {
            bgm_vo -= 1;
        }
    }
    
}


void Option_Manager::render()
{
    back->render(
        immediate_context,
        300, 180,  //position
        1, 1,     // scal
        1280, 720,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
        1280, 720,   // size
        0, 0,         // pibot
        1, 1, 1, 0.5,   // rgba
        0); // angle

    option->render(
        immediate_context,
        650, 200,  //position
        1, 1,     // scal
        516, 128,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
        516, 128,   // size
        0, 0,         // pibot
        1, 1, 1, 1.0,   // rgba
        0); // angle

    // ÉAÉCÉRÉì
    icon->render(immediate_context,
        330, 340 + icon_eas + icon_pos,
        1.0f, 1.0f,
        128, 128,
        128, 128,
        0, 0,
        1, 1, 1, 1,
        0);


    // BGMê›íË
    {
        bgm->render(
            immediate_context,
            500, 340,  //position
            1, 1,     // scal
            256, 128,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
            256, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle

        meter_back->render(
            immediate_context,
            800, 340,  //position
            1, 1,     // scal
            516, 128,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
            516, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle

        bar->render(
            immediate_context,
            800, 340,  //position
            1, 1,     // scal
            516, 128,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
            20 + bgm_vo, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }

    // SEê›íË
    {
        se->render(
            immediate_context,
            500, 520,  //position
            1, 1,     // scal
            256, 128,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
            256, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle

        meter_back->render(
            immediate_context,
            800, 520,  //position
            1, 1,     // scal
            516, 128,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
            516, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
        bar->render(
            immediate_context,
            800, 520,  //position
            1, 1,     // scal
            516, 128,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
            516, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }

    // ÉJÉÅÉâä¥ìxê›íË
    {
        camera->render(
            immediate_context,
            500, 700,  //position
            1, 1,     // scal
            256, 128,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
            256, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle


        meter_back->render(
            immediate_context,
            800, 700,  //position
            1, 1,     // scal
            516, 128,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
            516, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
        bar->render(
            immediate_context,
            800, 700,  //position
            1, 1,     // scal
            516, 128,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
            516, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }

    DrawDebugGUI();
}
