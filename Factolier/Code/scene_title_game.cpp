#include <imgui.h>
#include "scene_manager.h"
#include "scene_title_game.h"
#include "scene_loading.h"
#include "easing.h"
#include "scene_select.h"
#include "easing.h"
#include "scene_loading.h"
#include "utility.h"
#include "option_manager.h"
#include "scene_game.h"

#define SELECT 0
#define OPTION 200
#define END 400


// デバッグ用GUI描画
void Scene_Title_Game::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Title", nullptr, ImGuiWindowFlags_None))
    {
        // トランスフォーム
        if (ImGui::CollapsingHeader("transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            
            ImGui::InputFloat("icon_eas", &icon_eas);
            ImGui::InputFloat("icon_pos", &icon_pos);
            ImGui::InputFloat("time", &time);
            ImGui::InputFloat("option_vo", &option_vo);
            ImGui::InputFloat("gauge", &gauge);
            ImGui::Checkbox("option_flag", &option_flag);
        }
    }
    ImGui::End();
}

void Scene_Title_Game::initialize(Scene_Manager* parent_)
{
    parent = parent_;
    title = make_unique<Sprite>(parent->device(), "Data/Sprite/title.png");

    back_stage1 = make_unique<Sprite>(parent->device(), "Data/Sprite/title_spring.png");
    back_stage2 = make_unique<Sprite>(parent->device(), "Data/Sprite/title_summer.png");
    back_stage3 = make_unique<Sprite>(parent->device(), "Data/Sprite/title_autumn.png");
    back_stage4 = make_unique<Sprite>(parent->device(), "Data/Sprite/title_winter.png");
    back_stage0 = make_unique<Sprite>(parent->device(), "Data/Sprite/title_spring.png");
   
    icon_spring = make_unique<Sprite>(parent->device(), "Data/Sprite/春カーソル.png");
    icon_summer = make_unique<Sprite>(parent->device(), "Data/Sprite/夏カーソル.png");
    icon_autam  = make_unique<Sprite>(parent->device(), "Data/Sprite/秋カーソル.png");
    icon_winter = make_unique<Sprite>(parent->device(), "Data/Sprite/冬カーソル.png");


    start = make_unique<Sprite>(parent->device(), "Data/Sprite/ステージセレクト.png");
    option_start = make_unique<Sprite>(parent->device(), "Data/Sprite/オプション文字.png");
    exit = make_unique<Sprite>(parent->device(), "Data/Sprite/おわる.png");

    sound = std::make_unique<Sound>(parent->sound_manager()->load_sound(L"./Data/Sound/タイトル.wav"));
    sound->play(true);
   
    se = std::make_unique<Sound>(parent->sound_manager()->load_sound(L"./Data/Sound/se_仮.wav"));
    


}

void Scene_Title_Game::uninitialize()
{

}

void Scene_Title_Game::update(float elapsed_time)
{
    
    slideshow(elapsed_time);

    if(!option_flag)
    {
        move(elapsed_time, parent->input_manager());
    }
    else
    {
        option(elapsed_time, parent->input_manager());
    }
}

void Scene_Title_Game::move(float elapsedTime, Input_Manager* input_manager)
{
    next_move = static_cast<int>(icon_pos);


    if (!icon_flag)
    {
        if (icon_pos != 0)
        {
            if (input_manager->TRG(0) & PAD_UP)
            {
                icon_up_flag = true;
            }
            if (icon_up_flag)
            {
                if (time <= 0.5f)
                {
                    time += elapsedTime;
                    icon_eas = -easing::out_quint(time, 0.5f, 200.0f, 0.0f);
                }
                else
                {
                    icon_up_flag = false;

                    time = 0.0f;
                    icon_pos -= 200.0f;
                    icon_eas = 0.0f;
                }
            }
        }

        if (icon_pos < 400)
        {
            if (input_manager->TRG(0) & PAD_DOWN)
            {
                icon_down_flag = true;
            }
            if (icon_down_flag)
            {
                if (time <= 0.5f)
                {
                    time += elapsedTime;
                    icon_eas = +easing::out_quart(time, 0.5f, 200.0f, 0.0f);
                }
                else
                {
                    icon_down_flag = false;

                    time = 0.0f;
                    icon_pos += 200.0f;
                    icon_eas = 0.0f;
                }
            }
        }
    }

    // enterで決定
    move_flag = false;
    if (time == 0)
    {
        if (icon_pos == 0)
        {
            if (input_manager->TRG(0) & PAD_START)
            {
                move_flag = true;
                icon_flag = true;
            }
            if (move_flag)
            {
                parent->change_scene(new Scene_Loading(new Scene_Select));
                //parent->change_scene(new Scene_Loading(new Scene_Game));
         
            }
        }
        if (icon_pos == 200)
        {
            
            if (input_manager->TRG(0) & PAD_START)
            {
                move_flag = true;
                icon_flag = true;
            }
            if (move_flag)
            {
                // オプション画面に移行する
                option_flag = true;
                icon_flag = false;
            
            }
        }
    }
    
}

void Scene_Title_Game::slideshow(float elapsedTime)
{
  
    slidetimer += elapsedTime / 8;
    slide = static_cast<int>(slidetimer) % 4;
    decimals = modff(slidetimer, &integer);

    if (slide == 0)
    {
        back_stage1_alpha = 1;
        back_stage2_alpha = 1;
         back_stage1_alpha -= decimals;
    }
     else if (slide == 1)
    {
        back_stage2_alpha = 1;
        back_stage3_alpha = 1;
        back_stage2_alpha -= decimals;
    }
     else if (slide == 2)
    {
        back_stage3_alpha = 1;
        back_stage4_alpha = 1;
        back_stage3_alpha -= decimals;
    }
     else if (slide == 3)
    {
        back_stage4_alpha = 1;
        back_stage4_alpha -= decimals;
    }
  
}

void Scene_Title_Game::option(float elapsedTime, Input_Manager* input_manager)
{
    
    //sound->pause();
    parent->option_manager()->title_update(elapsedTime, input_manager);
    sound->set_volume(parent->option_manager()->bgm_vo);
    se->set_volume(parent->option_manager()->se_vo);

    if (parent->option_manager()->down_flag ||
        parent->option_manager()->up_flag)
    {
        se->play(false);
    }

    if (input_manager->TRG(0) & PAD_START)
    {
        option_flag = false;
    }
   
    
    
}


void Scene_Title_Game::render(float elapsed_time)
{
    ID3D11DeviceContext* device_context_ = parent->device_context();

    
       //レンダーターゲットビューと深度ステンシルビューを元に戻す
    {
        ID3D11RenderTargetView* rtv = parent->render_target_view();
        ID3D11DepthStencilView* dsv = parent->depth_stencil_view();
        FLOAT color[]{ 1.0f, 1.0f, 1.0f, 1.0f };

        parent->device_context()->ClearRenderTargetView(rtv, color);
        parent->device_context()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        parent->device_context()->OMSetRenderTargets(1, &rtv, dsv);
    }
    parent->state_manager()->setSS(SS::POINT);
    parent->state_manager()->setSS(SS::LINEAR);
    parent->state_manager()->setSS(SS::ANISOTROPIC);

    parent->state_manager()->setDS(DS::OFF_OFF);

    parent->state_manager()->setBS(BS::ALPHA);

    parent->state_manager()->setRS(RS::SOLID_NONE);

    // スライドショー
    {
        back_stage0->render(
            device_context_,
            0, 0,  //position
            1, 1,     // scal
            1920, 1080,    // どれくらい描画するか
            1920, 1080,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle

        back_stage4->render(
            device_context_,
            0, 0,
            1, 1,
            1920, 1080,
            1920, 1080,
            0, 0,
            1, 1, 1, back_stage4_alpha,
            0);

        back_stage3->render(
            device_context_,
            0, 0,
            1, 1,
            1920, 1080,
            1920, 1080,
            0, 0,
            1, 1, 1, back_stage3_alpha,
            0);

        back_stage2->render(
            device_context_,
            0, 0,
            1, 1,
            1920, 1080,
            1920, 1080,
            0, 0,
            1, 1, 1, back_stage2_alpha,
            0);

        back_stage1->render(
            device_context_,
            0, 0,
            1, 1,
            1920, 1080,
            1920, 1080,
            0, 0,
            1, 1, 1, back_stage1_alpha,
            0);
    }

    // アイコン
    {
        icon_spring->render(device_context_,
            1300, 320 + icon_eas + icon_pos,
            1.0f, 1.0f,
            64, 64,
            64, 64,
            0, 0,
            1, 1, 1, 1,
            0);
        icon_winter->render(device_context_,
            1300, 320 + icon_eas + icon_pos,
            1.0f, 1.0f,
            64, 64,
            64, 64,
            0, 0,
            1, 1, 1, back_stage4_alpha,
            0);
        icon_autam->render(device_context_,
            1300, 320 + icon_eas + icon_pos,
            1.0f, 1.0f,
            64, 64,
            64, 64,
            0, 0,
            1, 1, 1, back_stage3_alpha,
            0);

        icon_summer->render(device_context_,
            1300, 320 + icon_eas + icon_pos,
            1.0f, 1.0f,
            64, 64,
            64, 64,
            0, 0,
            1, 1, 1, back_stage2_alpha,
            0);

        icon_spring->render(device_context_,
            1300, 320 + icon_eas + icon_pos,
            1.0f, 1.0f,
            64, 64,
            64, 64,
            0, 0,
            1, 1, 1, back_stage1_alpha,
            0);

    }

    // タイトル
    {
        title->render(device_context_,
            100, 80,
            1.0f, 1.0f,
            900, 384,
            900, 384,
            0, 0,
            1, 1, 1, 1.0f,
            0);
    }

    // 点滅用
    timer++;
    float t_sta = 0;
    float t_op = 0;
    float t_end = 0;
    t_sta = CAST_F(timer / 40 % 2);
    t_op = CAST_F(timer / 40 % 2);
    t_end = CAST_F(timer / 40 % 2);
   
    if (!option_flag)
    {
        if (icon_eas != 0)
        {
            timer = 0;
            t_sta = 1;
            t_op = 1;
            t_end = 1;
        }
        else
        {
            switch (next_move)
            {
            case SELECT:
                t_op = 1;
                t_end = 1;
                break;

            case OPTION:
                t_sta = 1;
                t_end = 1;
                break;

            case END:
                t_sta = 1;
                t_op = 1;
                break;
            }
        }
        if (t_sta)
        {
            // セレクト
            start->render(device_context_,
                1400, 300,
                1.0f, 1.0f,
                384, 128,
                384, 128,
                0, 0,
                1, 1, 1, 1,
                0);
        }
        if (t_op)
        {
            // オプション
            option_start->render(device_context_,
                1400, 500,
                1.0f, 1.0f,
                384, 128,
                384, 128,
                0, 0,
                1, 1, 1, 1,
                0); // angle
        }
        if (t_end)
        {
            // ゲーム終了
            exit->render(device_context_,
                1400, 700,
                1.0f, 1.0f,
                384, 128,
                384, 128,
                0, 0,
                1, 1, 1, 1,
                0); // angle
        }

    }
    // オプション画面を開いたとき
    else
    {
        parent->option_manager()->title_render();
    }

   

    //DrawDebugGUI();

}