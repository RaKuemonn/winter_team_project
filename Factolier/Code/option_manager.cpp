#include <imgui.h>
#include "option_manager.h"
#include "easing.h"

// デバッグ用GUI描画
void Option_Manager::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Option", nullptr, ImGuiWindowFlags_None))
    {
        // トランスフォーム
        if (ImGui::CollapsingHeader("transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::InputFloat("icon_eas", &icon_eas);
            ImGui::InputFloat("icon_pos", &icon_pos);
            ImGui::InputFloat("time", &time);
            ImGui::Checkbox("up_flag", &up_flag);
            ImGui::Checkbox("down_flag", &down_flag);
            ImGui::InputFloat("bgm_vo", &bgm_vo);
            ImGui::InputFloat("bgm_move", &bgm_move);
            ImGui::InputFloat("bgm_move", &bgm_move);
        }
    }
    ImGui::End();
}


Option_Manager::Option_Manager(ID3D11Device* device, ID3D11DeviceContext* context)
{
    immediate_context = context;
    back = make_unique<Sprite>(device, "Data/back.png");
    bgm = make_unique<Sprite>(device, "Data/音量_bgm仮.png");
    se = make_unique<Sprite>(device, "Data/音量_se仮.png");
    camera = make_unique<Sprite>(device, "Data/カメラ_仮.png");
    bar_back = make_unique<Sprite>(device, "Data/option_1仮.png");
    bar = make_unique<Sprite>(device, "Data/option_2仮.png");
    option = make_unique<Sprite>(device, "Data/option_3仮.png");
    icon = make_unique<Sprite>(device, "Data/team1_flower_90×90.png");
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
    // アイコンがBGMのところにいたら
    if (icon_pos == 0)
    {
        if (bgm_bar >= 0 && bgm_bar <= 504)
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
        if (bgm_bar <= 0)
        {
            bgm_bar = 0;
        }
        if (bgm_bar >= 504)
        {
            bgm_bar = 504;
        }
       
    }
    // 何割あるかパーセントで出す
    // 描画範囲　％
    bgm_move = bgm_bar / 504;
    // BGM ボリューム　%
    bgm_vo = 1 * bgm_move;

    // SE
    // やってることはBGMと同じ
    if (icon_pos == 180)
    {
        if (se_bar >= 0 && se_bar <= 504)
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
        if (se_bar <= 0)
        {
            se_bar = 0;
        }
        if (se_bar >= 504)
        {
            se_bar = 504;
        }
       
    }
    se_move = se_bar / 504;
    se_vo = 1 * se_move;


    // カメラのバーを動かす処理
    // BGMと同じ
    if (icon_pos == 360)
    {
        if (se_bar >= 0 && se_bar <= 504)
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
        if (camera_bar <= 0)
        {
            camera_bar = 0;
        }
        if (camera_bar >= 504)
        {
            camera_bar = 504;
        }

    }
    camera_move = camera_bar / 504;
  
}
    



void Option_Manager::render()
{
    back->render(
        immediate_context,
        300, 180,  //position
        1, 1,     // scal
        1280, 720,    // どれくらい描画するか
        1280, 720,   // size
        0, 0,         // pibot
        1, 1, 1, 0.5,   // rgba
        0); // angle

    option->render(
        immediate_context,
        650, 200,  //position
        1, 1,     // scal
        516, 128,    // どれくらい描画するか
        516, 128,   // size
        0, 0,         // pibot
        1, 1, 1, 1.0,   // rgba
        0); // angle

    // アイコン
    icon->render(immediate_context,
        330, 340 + icon_eas + icon_pos,
        1.0f, 1.0f,
        128, 128,
        128, 128,
        0, 0,
        1, 1, 1, 1,
        0);


    // BGM設定
    {
        bgm->render(
            immediate_context,
            500, 340,  //position
            1, 1,     // scal
            256, 128,    // どれくらい描画するか
            256, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle

        bar_back->render(
            immediate_context,
            800, 340,  //position
            1, 1,     // scal
            516, 128,    // どれくらい描画するか
            516, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle

        bar->render(
            immediate_context,
            806, 346,  //position
            1, 1,     // scal
            504, 116,    // どれくらい描画するか
            504 * bgm_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }

    // SE設定
    {
        se->render(
            immediate_context,
            500, 520,  //position
            1, 1,     // scal
            256, 128,    // どれくらい描画するか
            256, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle

        bar_back->render(
            immediate_context,
            800, 520,  //position
            1, 1,     // scal
            516, 128,    // どれくらい描画するか
            516, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
        bar->render(
            immediate_context,
            806, 526,  //position
            1, 1,     // scal
            504, 116,    // どれくらい描画するか
            504 * se_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }

    // カメラ感度設定
    {
        camera->render(
            immediate_context,
            500, 700,  //position
            1, 1,     // scal
            256, 128,    // どれくらい描画するか
            256, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle


        bar_back->render(
            immediate_context,
            800, 700,  //position
            1, 1,     // scal
            516, 128,    // どれくらい描画するか
            516, 128,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
        bar->render(
            immediate_context,
            806, 706,  //position
            1, 1,     // scal
            504, 116,    // どれくらい描画するか
            504 * camera_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }

    DrawDebugGUI();
}
