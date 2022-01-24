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

        ImGui::InputInt("select_to_stage", &select_to_stage);
    }
    ImGui::End();
}

void Scene_Select::initialize(Scene_Manager* parent_)
{
    parent = parent_;
    ID3D11Device* dev = parent->device();

    back = make_unique<Sprite>(dev, "Data/Sprite/select_boss.png");
    stage1 = make_unique<Sprite>(dev, "Data/Sprite/select_icon_spring.png");
    stage2 = make_unique<Sprite>(dev, "Data/Sprite/select_icon_summer.png");
    stage3 = make_unique<Sprite>(dev, "Data/Sprite/selectstage_stage3.png");
    stage4 = make_unique<Sprite>(dev, "Data/Sprite/select_icon_winter.png");
    stage5 = make_unique<Sprite>(dev, "Data/Sprite/select_icon_boss.png");
    choice = make_unique<Sprite>(dev, "Data/Sprite/frame.png");

    enter = make_unique<Sprite>(dev, "Data/Sprite/push the Enter.png");

    key_stage2 = make_unique<Sprite>(parent->device(), "Data/Sprite/南京錠.png");
    key_stage3 = make_unique<Sprite>(parent->device(), "Data/Sprite/南京錠.png");
    key_stage4 = make_unique<Sprite>(parent->device(), "Data/Sprite/南京錠.png");
    key_boss = make_unique<Sprite>(parent->device(), "Data/Sprite/南京錠.png");

    spring = make_unique<Sprite>(dev, "Data/Sprite/select_spring.png");
    summer = make_unique<Sprite>(dev, "Data/Sprite/select_summer.png");
    autum = make_unique<Sprite>(dev, "Data/Sprite/select_autumn.png");
    winter = make_unique<Sprite>(dev, "Data/Sprite/select_winter.png");
    //winter = make_unique<Sprite>(dev, "Data/Sprite/select_winter.png");

    sound = std::make_unique<Sound>(parent->sound_manager()->load_sound(L"./Data/Sound/select_BGM.wav"));
    sound->play(true);
    crick = std::make_unique<Sound>(parent->sound_manager()->load_sound(L"./Data/Sound/crick.wav"));
}

void Scene_Select::uninitialize()
{
    File_IO::write("save.dat", parent->option_manager()->get_binary());
}


void Scene_Select::update(float elapsed_time)
{
    
   move(elapsed_time, parent->input_manager());
}

void Scene_Select::move(float elapsedTime, Input_Manager* input_manager)
{

  
    if (position > -2000 && right_flag == false)
    {
        
        if (input_manager->TRG(0) & PAD_RIGHT)
        {
            left_flag = true;
        }
        if (left_flag)
        {
            if (time <= 0.5f)
            {
                time += elapsedTime;
                eas = -easing::out_quint(time, 0.5f, 500.0f, 0.0f);
            }
            else
            {
                left_flag = false;

                time = 0.0f;
                position -= 500.0f;
                eas = 0.0f;

                //現在選んでいるステージ
                select_to_stage++;
            }
        }
    }

    if (position != 0 && left_flag == false)
    {
       
        if (input_manager->TRG(0) & PAD_LEFT)
        {
            right_flag = true;
            
        }
        if (right_flag)
        {
            if (time <= 0.5f)
            {
                time += elapsedTime;
                eas = +easing::out_quint(time, 0.5f, 500.0f, 0.0f);
            }
            else
            {
                right_flag = false;

                time = 0.0f;
                position += 500.0f;
                eas = 0.0f;

                //現在選んでいるステージ
                select_to_stage--;
            }
        }
    }
  
   
        if (eas == 0)
        {
            if (input_manager->TRG(0) & PAD_START)
            {
                crick->play(false);
                //選んでいるステージに応じた処理
                Option_Manager* opm = parent->option_manager();

                switch (select_to_stage)
                {
                case CAST_I(Stage_Select::STAGE_1):
                {
                    opm->set_next_stage(Stage_Select::STAGE_1);
                    break;
                }

                case CAST_I(Stage_Select::STAGE_2):
                {
                    //前のステージをクリアしているかどうか
                    if (opm->get_binary().clear_flag[select_to_stage - 1])
                    {
                        opm->set_next_stage(Stage_Select::STAGE_2);
                    }
                    break;
                }

                case CAST_I(Stage_Select::STAGE_3):
                {
                    //前のステージをクリアしているかどうか
                    if (opm->get_binary().clear_flag[select_to_stage - 1])
                    {
                        opm->set_next_stage(Stage_Select::STAGE_3);
                    }
                    break;
                }

                case CAST_I(Stage_Select::STAGE_4):
                {
                    //前のステージをクリアしているかどうか
                    if (opm->get_binary().clear_flag[select_to_stage - 1])
                    {
                        opm->set_next_stage(Stage_Select::STAGE_4);
                    }
                    break;
                }

                case CAST_I(Stage_Select::STAGE_BOSS):
                {
                    //前のステージをクリアしているかどうか
                    if (opm->get_binary().clear_flag[select_to_stage - 1])
                    {
                        opm->set_next_stage(Stage_Select::STAGE_BOSS);
                    }
                    break;
                }

                }

                parent->change_scene(new Scene_Title_Game);
            }
        }
    
}

void Scene_Select::render(float elapsed_time)
{
    ID3D11DeviceContext* device_context_ = parent->device_context();

    //レンダーターゲットビューと深度ステンシルビューを元に戻す
    {
        ID3D11RenderTargetView* rtv = parent->render_target_view();
        ID3D11DepthStencilView* dsv = parent->depth_stencil_view();
        FLOAT color[]{ 1.0f, 1.0f, 1.0f, 1.0f };

        device_context_->ClearRenderTargetView(rtv, color);
        device_context_->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        device_context_->OMSetRenderTargets(1, &rtv, dsv);
    }
    parent->state_manager()->setSS(SS::POINT);
    parent->state_manager()->setSS(SS::LINEAR);
    parent->state_manager()->setSS(SS::ANISOTROPIC);

    parent->state_manager()->setDS(DS::OFF_OFF);

    parent->state_manager()->setBS(BS::ALPHA);

    parent->state_manager()->setRS(RS::SOLID_NONE);

   
    switch (select_to_stage)
    {
    case CAST_I(Stage_Select::STAGE_1):
    {
        spring->render(
            device_context_,
            0, 0,  //position
            1, 1,     // scal
            1920, 1080,    // どれくらい描画するか
            1920, 1080,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
        break;
    }

    case CAST_I(Stage_Select::STAGE_2):
    {
        summer->render(
            device_context_,
            0, 0,  //position
            1, 1,     // scal
            1920, 1080,    // どれくらい描画するか
            1920, 1080,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
        break;
    }

    case CAST_I(Stage_Select::STAGE_3):
    {
        autum->render(
            device_context_,
            0, 0,  //position
            1, 1,     // scal
            1920, 1080,    // どれくらい描画するか
            1920, 1080,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
        break;
    }

    case CAST_I(Stage_Select::STAGE_4):
    {
        winter->render(
            device_context_,
            0, 0,  //position
            1, 1,     // scal
            1920, 1080,    // どれくらい描画するか
            1920, 1080,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
        break;
    }

    case CAST_I(Stage_Select::STAGE_BOSS):
    {
        back->render(
            device_context_,
            0, 0,  //position
            1, 1,     // scal
            1920, 1080,    // どれくらい描画するか
            1920, 1080,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
        break;
    }

    }


    choice->render(device_context_,
            806, 386,  //position
            1.0f, 1.0f,     // scal
            308, 308,    // どれくらい描画するか
            308, 308,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    stage1->render(device_context_,
            864 + position + eas, 444,  //position
            1.0f, 1.0f,     // scal
            192, 192,    // どれくらい描画するか
            192, 192,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    stage2->render(device_context_,
            1364 + position + eas, 444,  //position
            1.0f, 1.0f,     // scal
            192, 192,    // どれくらい描画するか
            192, 192,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    stage3->render(device_context_,
            1864 + position + eas, 444,  //position
            1.0f, 1.0f,     // scal
            192, 192,    // どれくらい描画するか
            192, 192,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    stage4->render(device_context_,
        2364 + position + eas, 444,  //position
        1.0f, 1.0f,     // scal
        192, 192,    // どれくらい描画するか
        192, 192,   // size
        0, 0,         // pibot
        1, 1, 1, 1,   // rgba
        0); // angle
    stage5->render(device_context_,
        2864 + position + eas, 444,  //position
        1.0f, 1.0f,     // scal
        192, 192,    // どれくらい描画するか
        192, 192,   // size
        0, 0,         // pibot
        1, 1, 1, 1,   // rgba
        0); // angle


    //鍵の描画
    Option_Manager* opm = parent->option_manager();
    if (!opm->get_binary().clear_flag[CAST_I(Stage_Select::STAGE_2) - 1])
    {
        key_stage2->render(device_context_,
            1360 + position + eas, 444,  //position
            1.0f, 1.0f,     // scal
            200, 200,    // どれくらい描画するか
            200, 200,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    }

    if (!opm->get_binary().clear_flag[CAST_I(Stage_Select::STAGE_3) - 1])
    {
        key_stage3->render(device_context_,
            1860 + position + eas, 444,  //position
            1.0f, 1.0f,     // scal
            200, 200,    // どれくらい描画するか
            200, 200,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    }

    if (!opm->get_binary().clear_flag[CAST_I(Stage_Select::STAGE_4) - 1])
    {
        key_stage4->render(device_context_,
            2360 + position + eas, 444,  //position
            1.0f, 1.0f,     // scal
            200, 200,    // どれくらい描画するか
            200, 200,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    }

    if (!opm->get_binary().clear_flag[CAST_I(Stage_Select::STAGE_BOSS) - 1])
    {
        key_boss->render(device_context_,
            2860 + position + eas, 444,  //position
            1.0f, 1.0f,     // scal
            200, 200,    // どれくらい描画するか
            200, 200,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    }

    
    if (eas == 0)
    {
        enter->render(device_context_,
            770, 740,  //position
            1.0f, 1.0f,     // scal
            384, 64,    // どれくらい描画するか
            384, 64,   // size
            0, 0,         // pibot
            1, 1, 1, 1,   // rgba
            0); // angle
    }
  


    DrawDebugGUI();

}