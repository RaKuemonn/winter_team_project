#include <imgui.h>
#include "scene_select.h"
#include "scene_manager.h"
#include "scene_title_game.h"
#include "easing.h"
#include "scene_loading.h"

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

  
    if (position > -2000 && right_flag == false)
    {
        
        if (input_manager->TRG(0) & PAD_LEFT)
        {
            left_flag = true;
        }
        if (left_flag)
        {
            if (time <= 0.7f)
            {
                time += elapsedTime;
                eas = -easing::out_quint(time, 0.7, 500.0f, 0.0f);
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

    if (position != 0 && left_flag == false)
    {
       
        if (input_manager->TRG(0) & PAD_RIGHT)
        {
            right_flag = true;
            
        }
        if (right_flag)
        {
            if (time <= 0.6f)
            {
                time += elapsedTime;
                eas = +easing::out_quint(time, 0.6, 500.0f, 0.0f);
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
  
   
    if (position == 0)
    {
        if (eas == 0)
        {
            if (input_manager->TRG(0) & PAD_START)
            {
                parent->change_scene(new Scene_Title_Game);
            }
        }
        
    }
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

   

    back->render(
            device_context_,
            0, 0,  //position
            1, 1,     // scal
        1980, 1080,    // どれくらい描画するか
        1980, 1080,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    choice->render(device_context_,
            820, 400,  //position
            1.0f, 1.0f,     // scal
            308, 308,    // どれくらい描画するか
            308, 308,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    stage1->render(device_context_,
            876 + position + eas, 454,  //position
            1.0f, 1.0f,     // scal
            192, 192,    // どれくらい描画するか
            192, 192,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    stage2->render(device_context_,
            1376 + position + eas, 454,  //position
            1.0f, 1.0f,     // scal
            192, 192,    // どれくらい描画するか
            192, 192,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    stage3->render(device_context_,
            1876 + position + eas, 454,  //position
            1.0f, 1.0f,     // scal
            192, 192,    // どれくらい描画するか
            192, 192,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    stage4->render(device_context_,
        2376 + position + eas, 454,  //position
        1.0f, 1.0f,     // scal
        192, 192,    // どれくらい描画するか
        192, 192,   // size
        0, 0,         // pibot
        1, 1, 1, 1,   // rgba
        0); // angle
    stage5->render(device_context_,
        2876 + position + eas, 454,  //position
        1.0f, 1.0f,     // scal
        192, 192,    // どれくらい描画するか
        192, 192,   // size
        0, 0,         // pibot
        1, 1, 1, 1,   // rgba
        0); // angle

    DrawDebugGUI();

}