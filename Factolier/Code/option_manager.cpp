#include <imgui.h>
#include "option_manager.h"
#include "imgui.h"
#include "easing.h"

#define BGM 0
#define SE 180
#define CAMERA 360
#define BAR_MAX 504
#define BAR_MIN 0


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
            ImGui::InputFloat("arrow_move", &arrow_move);
        }
    }
    ImGui::End();
}


Option_Manager::Option_Manager(ID3D11Device* device, ID3D11DeviceContext* context)
{
    immediate_context = context;
    title_back = make_unique<Sprite>(device, "./Data/Sprite/オプション.png");
    game_back = make_unique<Sprite>(device, "./Data/Sprite/ゲーム中オプション.png");

    File_IO::open("./Data/Binary/save.dat", binary_data);

    bar = make_unique<Sprite>(device, "./Data/Sprite/オプションバー.png");
    icon = make_unique<Sprite>(device, "./Data/Sprite/team1_flower_90×90.png");
    arrow = make_unique<Sprite>(device, "./Data/Sprite/バー矢印(38,164).png");
    option_bakc = make_unique<Sprite>(device, "./Data/Sprite/option_4仮.png");
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

    // 高さ　select 23
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
    // アイコンがBGMのところにいたら

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
    // 何割あるかパーセントで出す
    // 描画範囲　％
    bgm_move = bgm_bar / BAR_MAX;
    // BGM ボリューム　%
    bgm_vo = 1 * bgm_move;
    // SE
    // やってることはBGMと同じ
    se_move = se_bar / BAR_MAX;
    se_vo = 1 * se_move;
    // カメラのバーを動かす処理
    // BGMと同じ
    camera_move = camera_bar / BAR_MAX;
    // 矢印
    arrow_bgm = bgm_move * 504;
    arrow_se = se_move * 504;
    arrow_camera = camera_move * 504;
  
}
    

// ゲーム内で表示するオプション画面
void Option_Manager::game_render()
{
    game_back->render(
        immediate_context,
        0, 0,  //position
        1, 1,     // scal
        1920, 1080,    // どれくらい描画するか
        1920, 1080,   // size
        0, 0,         // pibot
        1, 1, 1, 1.0,   // rgba
        0); // angle


    // アイコン
    icon->render(immediate_context,
        382, 340 + icon_eas + icon_pos,
        1.0f, 1.0f,
        128, 128,
        128, 128,
        0, 0,
        1, 1, 1, 1,
        0);




    //BGM設定
    {
        bar->render(
            immediate_context,
            862, 339,  //position
            1, 1,     // scal
            504, 116,    // どれくらい描画するか
            504 * bgm_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }

    // SE設定
    {
        bar->render(
            immediate_context,
            862, 516,  //position
            1, 1,     // scal
            504, 116,    // どれくらい描画するか
            504 * se_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }

    // カメラ感度設定
    {

        bar->render(
            immediate_context,
            862, 693,  //position
            1, 1,     // scal
            504, 116,    // どれくらい描画するか
            504 * camera_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }


    // 矢印
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

}


// タイトルで表示するオプション画面
void Option_Manager::title_render()
{
    // 背景
    //option_bakc->render(
    //    immediate_context,
    //    0, 0,  //position
    //    1, 1,     // scal
    //    1920, 1080,    // どれくらい描画するか
    //    1920, 1080,   // size
    //    0, 0,         // pibot
    //    1, 1, 1, 0.5,   // rgba
    //    0); // angle
    title_back->render(
        immediate_context,
        0, 0,  //position
        1, 1,     // scal
        1920, 1080,    // どれくらい描画するか
        1920, 1080,   // size
        0, 0,         // pibot
        1, 1, 1, 1.0,   // rgba
        0); // angle


    // アイコン
    icon->render(immediate_context,
        382, 340 + icon_eas + icon_pos,
        1.0f, 1.0f,
        128, 128,
        128, 128,
        0, 0,
        1, 1, 1, 1,
        0);



     //BGM設定
    {
        bar->render(
            immediate_context,
            862, 339,  //position
            1, 1,     // scal
            504, 116,    // どれくらい描画するか
            504 * bgm_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }

    // SE設定
    {
        bar->render(
            immediate_context,
            862, 516,  //position
            1, 1,     // scal
            504, 116,    // どれくらい描画するか
            504 * se_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }

    // カメラ感度設定
    {

        bar->render(
            immediate_context,
            862, 693,  //position
            1, 1,     // scal
            504, 116,    // どれくらい描画するか
            504 * camera_move, 116,   // size
            0, 0,         // pibot
            1, 1, 1, 1.0,   // rgba
            0); // angle
    }


    // 矢印
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