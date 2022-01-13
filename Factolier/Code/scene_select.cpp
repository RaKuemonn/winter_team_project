#include <imgui.h>
#include "scene_select.h"
#include "scene_manager.h"
#include "scene_title.h"
#include "easing.h"

// デバッグ用GUI描画
void Scene_Select::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        // トランスフォーム
        if (ImGui::CollapsingHeader("transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
          
            ImGui::InputFloat("position", &position);
            ImGui::InputFloat("move_left", &move_left);
            ImGui::InputFloat("move_right", &move_right);
            ImGui::InputFloat("left_max", &left_max);
            ImGui::InputFloat("light_max", &light_max);
            ImGui::Checkbox(" left_flag", &left_flag);
            ImGui::Checkbox(" right_flag", &right_flag);
            ImGui::InputFloat("eas", &eas);
            ImGui::InputFloat("time", &time);
        }
    }
    ImGui::End();
}

void Scene_Select::initialize(Scene_Manager* parent_)
{
    parent = parent_;
    select_back = make_unique<Sprite_Batch>(parent->device(), "Data/back.png", 1);
    select_stage1 = make_unique<Sprite_Batch>(parent->device(), "Data/select1.png", 1);
    back = make_unique<Sprite>(parent->device(), "Data/back.png");
    stage1 = make_unique<Sprite>(parent->device(), "Data/select1.png");
    stage2 = make_unique<Sprite>(parent->device(), "Data/select2.png");
    stage3 = make_unique<Sprite>(parent->device(), "Data/select3.png");
    stage4 = make_unique<Sprite>(parent->device(), "Data/select4.png");
    stage5 = make_unique<Sprite>(parent->device(), "Data/select5.png");
    choice = make_unique<Sprite>(parent->device(), "Data/waku.png");

   
}

void Scene_Select::uninitialize()
{
 
}


void Scene_Select::update(float elapsed_time)
{
    
   move(elapsed_time, parent->input_manager());
}

void Scene_Select::move(float elapsedTime, Input_Manager* input_manager)
{
    //if (position >= -1990)
    //{
    //    if (left_flag == false)
    //    {
    //        if (input_manager->TRG(0) & PAD_LEFT)
    //        {
    //            left_flag = true;
    //        }
    //    }
    //    if (left_flag == true)
    //    {
    //        move_left += (left_max - move_left) * 0.015f;
    //        if (move_left >= left_max - 10)
    //        {
    //            left_max += 500;
    //            left_flag = false;
    //        }
    //    }
    //}
  

   /*if (position != 0)
    {
        if (right_flag == false)
        {
            if (input_manager->TRG(0) & PAD_RIGHT)
            {
                right_flag = true;
            }
        }
        if (right_flag == true)
        {
            move_right += (-light_max - move_right) * 0.015f;
            if (move_right < -light_max + 10)
            {
                light_max += 500;
                right_flag = false;
            }
        }
    }
    position = -move_left - move_right;
    if (position <= -1990)
    {
        position -= 10;
    }*/
   
    if (position > -2000)
    {
        if (input_manager->TRG(0) & PAD_LEFT)
        {
            left_flag = true;
        }
        if (left_flag)
        {
            if (time <= 1.0f)
            {
                time += elapsedTime;
                eas = -easing::in_quart(time, 1, 500.0f, 0.0f);
            }
            else
            {
                left_flag = false;

                time = 0.0f;
                position -= 500.0f;
                eas = 0.0f;
            }
        }
    }
  
    if (position != 0)
    {
        if (input_manager->TRG(0) & PAD_RIGHT)
        {
            right_flag = true;
        }
        if (right_flag)
        {
            if (time <= 1.0f)
            {
                time += elapsedTime;
                eas = +easing::in_quart(time, 1, 500.0f, 0.0f);
            }
            else
            {
                right_flag = false;

                time = 0.0f;
                position += 500.0f;
                eas = 0.0f;
            }
        }
    }
    //if (left_flag == false)
    //{
    //    if (input_manager->TRG(0) & PAD_LEFT)
    //    {
    //        left_flag = true;
    //    }
    //}
    //if (left_flag == true)
    //{
    //    move_left += easing::in_quart(1, 2, 2, 2);
    //  
    //    if (move_left >= 50)
    //    {
    //        //left_max += 500;
    //        left_flag = false;
    //    }
    //}
    //position -= move_left;
     
}

void Scene_Select::render(float elapsed_time)
{
    ID3D11DeviceContext* device_context_ = parent->device_context();

 /*   parent->state_manager()->setSS(SS::POINT);
    parent->state_manager()->setSS(SS::LINEAR);
    parent->state_manager()->setSS(SS::ANISOTROPIC);

    parent->state_manager()->setDS(DS::OFF_OFF);

    parent->state_manager()->setBS(BS::ALPHA);

    parent->state_manager()->setRS(RS::SOLID_NONE);*/
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

    select_back->render(device_context_,
        0, 0,  //position
        1, 1,     // scal
        1280, 720,    // どれくらい描画するか
        1280, 720,   // size
        0, 0,         // pibot
        1, 1, 1, 1,   // rgba
        0); // angle

    back->render(
            device_context_,
            0, 0,  //position
            1, 1,     // scal
            1280, 720,    // どれくらい描画するか
            1280, 720,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    choice->render(device_context_,
            500, 250,  //position
            0.8f, 0.8f,     // scal
            320, 320,    // どれくらい描画するか
            320, 320,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    stage1->render(device_context_,
            550 + position + eas, 300,  //position
            0.5f, 0.5f,     // scal
            320, 320,    // どれくらい描画するか
            320, 320,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    stage2->render(device_context_,
            1050 + position + eas, 300,  //position
            0.5f, 0.5f,     // scal
            320, 320,    // どれくらい描画するか
            320, 320,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    stage3->render(device_context_,
            1550 + position + eas, 300,  //position
            0.5f, 0.5f,     // scal
            320, 320,    // どれくらい描画するか
            320, 320,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    stage4->render(device_context_,
        2050 + position + eas, 300,  //position
        0.5f, 0.5f,     // scal
        320, 320,    // どれくらい描画するか
        320, 320,   // size
        0, 0,         // pibot
        1, 1, 1, 1,   // rgba
        0); // angle
    stage5->render(device_context_,
        2550 + position + eas, 300,  //position
        0.5f, 0.5f,     // scal
        320, 320,    // どれくらい描画するか
        320, 320,   // size
        0, 0,         // pibot
        1, 1, 1, 1,   // rgba
        0); // angle

    DrawDebugGUI();

}