#include <imgui.h>
#include "scene_manager.h"
#include "scene_title_game.h"
#include "scene_title.h"
#include "easing.h"
#include "scene_select.h"
#include "easing.h"
#include "scene_loading.h"
#include "utility.h"
#include "option_manager.h"

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
            /*ImGui::InputFloat("time", &time);
            ImGui::InputFloat("elapsedTime", &icon_eas);
            ImGui::InputFloat("slide", &slide_bebug);
            ImGui::InputFloat("slidetimer", &slidetimer);
            ImGui::InputFloat("back_stage1_alpha", &back_stage1_alpha);
            ImGui::InputFloat("back_stage2_alpha", &back_stage2_alpha);
            ImGui::InputFloat("back_stage3_alpha", &back_stage3_alpha);
            ImGui::InputFloat("back_stage4_alpha", &back_stage4_alpha);
            ImGui::InputFloat("alpha_ress", &alpha_ress);
            ImGui::InputFloat("again", &again);
            ImGui::InputFloat("decimals", &again);*/
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
    title = make_unique<Sprite>(parent->device(), "Data/title_仮.png");

    back_stage1 = make_unique<Sprite>(parent->device(), "Data/select1.png");
    back_stage2 = make_unique<Sprite>(parent->device(), "Data/select2.png");
    back_stage3 = make_unique<Sprite>(parent->device(), "Data/select3.png");
    back_stage4 = make_unique<Sprite>(parent->device(), "Data/select4.png");
    back_stage0 = make_unique<Sprite>(parent->device(), "Data/select1.png");
    start = make_unique<Sprite>(parent->device(), "Data/waku.png");
    icon = make_unique<Sprite>(parent->device(), "Data/team1_flower_90×90.png");

    option_start = make_unique<Sprite>(parent->device(), "Data/waku.png");

    

    //option_back = make_unique<Sprite>(parent->device(), "Data/back.png");

    //option_1 = make_unique<Sprite>(parent->device(), "Data/meta1.png");
    //option_2 = make_unique<Sprite>(parent->device(), "Data/meta2.png");
    //option_3 = make_unique<Sprite>(parent->device(), "Data/meta3.png");
    //return_title = make_unique<Sprite>(parent->device(), "Data/enter_仮.png");
    //audio_set = make_unique<Sprite>(parent->device(), "Data/audio_仮.png");

    //sound = std::make_unique<Sound>(parent->sound_manager()->load_sound(L"./Data/_.wav"));
    ////sound->play(true);


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

        if (icon_pos < 200)
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
                parent->change_scene(new Scene_Select);
         
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
    /*if (option_vo < 312)
    {
        if (input_manager->STATE(0) & PAD_RIGHT)
        {
            option_vo += 1;
            gauge += 1.25f;
        }
        
    }
    else
    {
        gauge = 306;
    }
    if (option_vo > 0)
    {
        if (input_manager->STATE(0) & PAD_LEFT)
        {
            option_vo -= 1;
            gauge -= 1.2f;
        }
    }*/
    
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

    // タイトル
    title->render(device_context_,
        100, 150,
        1.0f, 1.0f,
        900, 384,
        900, 384,
        0, 0,
        1, 1, 1, 1.0f,
        -15);

    // 点滅用
    timer++;
    float t_sta = 0;
    float t_op = 0;
    t_sta = CAST_F(timer / 180 % 2);
    t_op = CAST_F(timer / 180 % 2);
   
    if (!option_flag)
    {
        if (icon_eas != 0)
        {
            timer = 0;
            t_sta = 1;
            t_op = 1;
        }
        else
        {
            if (icon_pos == 0)
            {
                t_op = 1;

            }
            else if (icon_pos == 200)
            {
                t_sta = 1;
            }
        }
        if (t_sta)
        {
            start->render(device_context_,
                1400, 400,
                1.0f, 1.0f,
                384, 128,
                384, 128,
                0, 0,
                1, 1, 1, 1,
                0);
        }
        if (t_op)
        {
            option_start->render(device_context_,
                1400, 600,
                1.0f, 1.0f,
                384, 128,
                384, 128,
                0, 0,
                1, 1, 1, 1,
                0); // angle
        }

        icon->render(device_context_,
            1200, 400 + icon_pos + icon_eas,
            1.0f, 1.0f,
            128, 128,
            128, 128,
            0, 0,
            1, 1, 1, 1,
            0);

    }
    // オプション画面を開いたとき
    else
    {
        parent->option_manager()->render();
        
        /*
        option_back->render(device_context_,
            320, 180,
            1.0f, 1.0f,
            1280, 720,
            1280, 720,
            0, 0,
            1, 1, 1, 0.6f,
            0);


        option_1->render(device_context_,
            400, 300,
            1.0f, 1.0f,
            516, 256,
            516, 256,
            0, 0,
            1, 1, 1, 1.0f,
            0);
        option_2->render(device_context_,
            400, 300,
            0.8f, 0.8f,
            516, 256,
            140 + gauge, 256,
            0, 0,
            1, 1, 1, 1.0f,
            0);
        option_3->render(device_context_,
            400 + option_vo, 300,
            0.8f, 0.8f,
            516, 256,
            516, 256,
            0, 0,
            1, 1, 1, 1.0f,
            0);
        return_title->render(device_context_,
            500, 470,
            1.0f, 1.0f,
            900, 384,
            900, 384,
            0, 0,
            1, 1, 1, 1.0f,
            0);
        audio_set->render(device_context_,
            500, 170,
            1.0f, 1.0f,
            258, 128,
            258, 128,
            0, 0,
            1, 1, 1, 1.0f,
            0);*/
    }

   

    //DrawDebugGUI();

}