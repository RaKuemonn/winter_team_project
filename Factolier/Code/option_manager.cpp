#include <imgui.h>
#include "option_manager.h"
#include "imgui.h"
#include "easing.h"

#define BGM 0
#define SE 180
#define CAMERA 360
#define BAR_MAX 504
#define BAR_MIN 0


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

    bgm = make_unique<Sprite>(device, "Data/âπó _bgmâº.png");
    se = make_unique<Sprite>(device, "Data/âπó _seâº.png");
    camera = make_unique<Sprite>(device, "Data/ÉJÉÅÉâ_âº.png");
    bar_back = make_unique<Sprite>(device, "Data/option_1âº.png");
    bar = make_unique<Sprite>(device, "Data/option_2âº.png");
    option = make_unique<Sprite>(device, "Data/option_3âº.png");
    icon = make_unique<Sprite>(device, "Data/team1_flower_90Å~90.png");
    arrow = make_unique<Sprite>(device, "Data/ÉoÅ[ñÓàÛ(38,164).png");
    enter = make_unique<Sprite>(device, "Data/push the Enter.png");
    title = make_unique<Sprite>(device, "Data/title_returnâº.png");
    option_bakc = make_unique<Sprite>(device, "Data/option_4âº.png");
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
    // ÉAÉCÉRÉìÇ™BGMÇÃÇ∆Ç±ÇÎÇ…Ç¢ÇΩÇÁ

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
    // âΩäÑÇ†ÇÈÇ©ÉpÅ[ÉZÉìÉgÇ≈èoÇ∑
    // ï`âÊîÕàÕÅ@Åì
    bgm_move = bgm_bar / BAR_MAX;
    // BGM É{ÉäÉÖÅ[ÉÄÅ@%
    bgm_vo = 1 * bgm_move;
    // SE
    // Ç‚Ç¡ÇƒÇÈÇ±Ç∆ÇÕBGMÇ∆ìØÇ∂
    se_move = se_bar / BAR_MAX;
    se_vo = 1 * se_move;
    // ÉJÉÅÉâÇÃÉoÅ[ÇìÆÇ©Ç∑èàóù
    // BGMÇ∆ìØÇ∂
    camera_move = camera_bar / BAR_MAX;
    // ñÓàÛ
    arrow_bgm = bgm_move * 504;
    arrow_se = se_move * 504;
    arrow_camera = camera_move * 504;
  
}
    


// É^ÉCÉgÉãÇ≈ï\é¶Ç∑ÇÈÉIÉvÉVÉáÉìâÊñ 
void Option_Manager::title_render()
{
    // îwåi
    // îwåi
    option_bakc->render(
        immediate_context,
        0, 0,  //position
        1, 1,     // scal
        1920, 1080,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
        1920, 1080,   // size
        0, 0,         // pibot
        1, 1, 1, 0.5,   // rgba
        0); // angle
    back->render(
        immediate_context,
        300, 180,  //position
        1, 1,     // scal
        1280, 720,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
        1280, 720,   // size
        0, 0,         // pibot
        1, 1, 1, 0.5,   // rgba
        0); // angle

    //ÉIÉvÉVÉáÉì
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

        bar_back->render(
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
            806, 346,  //position
            1, 1,     // scal
            504, 116,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
            504 * bgm_move, 116,   // size
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

        bar_back->render(
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
            806, 526,  //position
            1, 1,     // scal
            504, 116,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
            504 * se_move, 116,   // size
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


        bar_back->render(
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
            806, 706,  //position
            1, 1,     // scal
            504, 116,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
            504 * camera_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }


    // ñÓàÛ
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

    enter->render(immediate_context,
        710, 840,  //position
        1.0f, 1.0f,     // scal
        384, 64,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
        384, 64,   // size
        0, 0,         // pibot
        1, 1, 1, 1,   // rgba
        0); // angle

    //DrawDebugGUI();
}


// ÉQÅ[ÉÄì‡Ç≈ï\é¶Ç∑ÇÈÉIÉvÉVÉáÉìâÊñ 
void Option_Manager::game_render()
{
    // îwåi
    option_bakc->render(
        immediate_context,
        0, 0,  //position
        1, 1,     // scal
        1920, 1080,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
        1920, 1080,   // size
        0, 0,         // pibot
        1, 1, 1, 0.5,   // rgba
        0); // angle
    back->render(
        immediate_context,
        300, 180,  //position
        1, 1,     // scal
        1280, 720,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
        1280, 720,   // size
        0, 0,         // pibot
        1, 1, 1, 0.5,   // rgba
        0); // angle

    //ÉIÉvÉVÉáÉì
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

        bar_back->render(
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
            806, 346,  //position
            1, 1,     // scal
            504, 116,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
            504 * bgm_move, 116,   // size
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

        bar_back->render(
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
            806, 526,  //position
            1, 1,     // scal
            504, 116,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
            504 * se_move, 116,   // size
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


        bar_back->render(
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
            806, 706,  //position
            1, 1,     // scal
            504, 116,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
            504 * camera_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }


    // ñÓàÛ
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

    title->render(immediate_context,
        710, 840,  //position
        1.0f, 1.0f,     // scal
        384, 64,    // Ç«ÇÍÇ≠ÇÁÇ¢ï`âÊÇ∑ÇÈÇ©
        384, 64,   // size
        0, 0,         // pibot
        1, 1, 1, 1,   // rgba
        0); // angle

    
}