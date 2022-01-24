
#include "scene_game.h"

#include "scene_manager.h"
#include "shader_manager.h"
#include "camera.h"
#include "entity_manager.h"
#include "stage_manager.h"
#include "player.h"
#include "enemy_move_closer.h"
#include "enemy_move_closer_.h"
#include "camera_controller.h"
#include "imgui.h"
#include "utility.h"
#include "sphere_vehicle.h"

inline void imgui(bool goal)
{
#ifdef _DEBUG
    ImGui::Begin("Entity");

    ImGui::Text("goal %d", goal);

    std::vector<short> vec = Entity_Manager::instance().get_entities(Tag::Player);
    if (vec.size())
    {
        ImGui::Text("Player");
        std::shared_ptr<Entity> player = Entity_Manager::instance().get_entity(vec.at(0));
        DirectX::XMFLOAT3 pos = player->get_position();
        ImGui::InputFloat3("position", &pos.x);
        ImGui::Spacing();
    }

    vec = Entity_Manager::instance().get_entities(Tag::Vehicle);
    int size = static_cast<int>(vec.size());

    ImGui::Text("control Vehicle");
    for (int i = 0; i < size; ++i)
    {
        std::shared_ptr<Entity> entity = Entity_Manager::instance().get_entity(vec.at(i));

        if (entity)
        {
            if(static_cast<Sphere_Vehicle*>(entity.get())->get_is_free() == true)continue;

            DirectX::XMFLOAT3 pos = entity->get_position();
            ImGui::InputFloat3("position", &pos.x);
            DirectX::XMFLOAT3 velo = entity->get_velocity();
            ImGui::InputFloat3("velocity", &velo.x);
            float velo_length = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMLoadFloat3(&velo)));
            ImGui::InputFloat("velocity_length", &velo_length);

        }
    }

    vec = Entity_Manager::instance().get_entities(Tag::Collide);
    size = static_cast<int>(vec.size());

    ImGui::Text("Enemy");
    for(int i = 0; i < size; ++i)
    {
        ImGui::Text("Number %d", i);

        std::shared_ptr<Entity> entity = Entity_Manager::instance().get_entity(vec.at(i));

        if (entity)
        {
            DirectX::XMFLOAT3 pos = entity->get_position();
            ImGui::InputFloat3("position", &pos.x);
            //DirectX::XMFLOAT3 velo = entity->get_velocity();
            //ImGui::InputFloat3("velocity", &velo.x);
            //float velo_length = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMLoadFloat3(&velo)));
            //ImGui::InputFloat("velocity_length", &velo_length);

        }
    }
    
    ImGui::End();
#endif
}


void Scene_Game::initialize(Scene_Manager* parent_)
{
    parent = parent_;

    Camera& camera = Camera::Instance();
    camera.set_lookat(
        DirectX::XMFLOAT3(0, 10, -10),
        DirectX::XMFLOAT3(0, 0, 0),
        DirectX::XMFLOAT3(0, 1, 0)
    );
    camera.set_perspective_fov(DirectX::XMConvertToRadians(60),
        CAST_F(SCREEN_WIDTH) / CAST_F(SCREEN_HEIGHT),
        0.1f,
        1000.0f);


    Entity_Manager::instance().set_update_move();

    std::shared_ptr<Entity> player = std::make_shared<Player>(parent);
    Entity_Manager::instance().spawn_register(player);




    // 選ばれているステージ
#ifdef NDEBUG
    const Stage_Select stage_num = parent->option_manager()->get_now_stage();
#else
    // TODO: debug ステージが固定されている
    const Stage_Select stage_num = Stage_Select::STAGE_BOSS;
#endif

    // プレイヤーの位置
    init_player_position(stage_num, player);

    // ステージの設定
    init_stage(stage_num);

    // 敵の設定
    short* ptr_boss_hp =
        init_enemy(stage_num, player->get_position());


    collision_manager = std::make_unique<Collision_Manager>(/*parent->option_manager()->get_now_stage()*/stage_num);
    clear_judge = std::make_unique<Clear_Judge>(stage_num, player->get_position(), ptr_boss_hp);
    camera_controller = std::make_unique<Camera_Controller>(&player->get_position());

    // TODO: debug
    //sky_box = std::make_unique<Sky_Box>(parent->device(), L"./Data/Sky_Box/stage_4.dds");

    debug_decorator_supporter = std::make_unique<Decotator_Supporter>(parent_);
}


void Scene_Game::uninitialize()
{
    Entity_Manager::instance().all_clear();
    Stage_Manager::instance().all_clear();
}


void Scene_Game::update(float elapsed_time)
{
    Stage_Manager::instance().update(elapsed_time);

    Entity_Manager::instance().update(elapsed_time);


    camera_controller->update(parent->device_context(), parent->input_manager(), parent->option_manager(), elapsed_time);

    collision_manager->judge(elapsed_time);

    bool clear = judge_clear();

    imgui(clear);

    debug_decorator_supporter->imgui_control();
}


void Scene_Game::render(float elapsed_time) 
{
    parent->state_manager()->setDS(DS::ON_ON);

    parent->state_manager()->setBS(BS::ALPHA);

    parent->state_manager()->setRS(RS::SOLID_BACK);


    Shader* shader = nullptr;
    ID3D11DeviceContext* ptr_device_context = parent->device_context();
    
    {
        shader = parent->shader_manager()->get_shader(Shaders::SHADOW);

        shader->begin(ptr_device_context, elapsed_time * 0.1f);

        Stage_Manager::instance().render(ptr_device_context);
        Entity_Manager::instance().render(ptr_device_context);

        shader->end(ptr_device_context);
    }

    
    {
        ID3D11RenderTargetView* rtv = parent->render_target_view();
        ID3D11DepthStencilView* dsv = parent->depth_stencil_view();
        FLOAT color[]{ 1.0f, 1.0f, 1.0f, 1.0f };

        ptr_device_context->ClearRenderTargetView(rtv, color);
        ptr_device_context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        ptr_device_context->OMSetRenderTargets(1, &rtv, dsv);
    }



    parent->state_manager()->setDS(DS::OFF_OFF);

    //�X�J�C�{�b�N�X�`��
    {
        shader = parent->shader_manager()->get_shader(Shaders::SKY);

        shader->begin(parent->device_context());

        // TODO: debug
        //sky_box->render(parent->device_context());

        shader->end(parent->device_context());
    }


    parent->state_manager()->setDS(DS::ON_ON);
    shader = parent->shader_manager()->get_shader(Shaders::PHONG);

    shader->begin(ptr_device_context);

    Stage_Manager::instance().render(ptr_device_context);
    Entity_Manager::instance().render(ptr_device_context);
    debug_decorator_supporter->render(ptr_device_context);

    shader->end(ptr_device_context);
}


void Scene_Game::init_player_position(const Stage_Select stage_, std::weak_ptr<Entity> player_)
{
    switch (stage_)
    {

    case Stage_Select::STAGE_1:
    {
        player_.lock()->set_position({0.0f, -8.0f, -127.0f });
        break;
    }

    case Stage_Select::STAGE_2:
    {
        player_.lock()->set_position({});
        break;
    }

    case Stage_Select::STAGE_3:
    {
        player_.lock()->set_position({});
        break;
    }

    case Stage_Select::STAGE_4:
    {
        player_.lock()->set_position({});
        break;
    }

    case Stage_Select::STAGE_BOSS:
    {
        player_.lock()->set_position({0.0f,10.0f,-30.0f});
        break;
    }

    }
}

void Scene_Game::init_stage(const Stage_Select stage_)
{
    std::unique_ptr<Stage_Spawner>		stage_spawner = nullptr;
    stage_spawner = std::make_unique<Stage_Spawner>(parent);    

    // TODO: debug
    switch (stage_)
    {

    case Stage_Select::STAGE_1:
    {
        stage_spawner->set_stage_1();
        //sky_box = std::make_unique<Sky_Box>(parent->device(), L"./Data/Sky_Box/stage_4.dds");
        break;
    }

    case Stage_Select::STAGE_2:
    {
        stage_spawner->set_stage_2();
        //sky_box = std::make_unique<Sky_Box>(parent->device(), L"./Data/Sky_Box/stage_4.dds");
        break;
    }

    case Stage_Select::STAGE_3:
    {
        stage_spawner->set_stage_3();
        //sky_box = std::make_unique<Sky_Box>(parent->device(), L"./Data/Sky_Box/stage_4.dds");
        break;
    }

    case Stage_Select::STAGE_4:
    {
        stage_spawner->set_stage_4();
        //sky_box = std::make_unique<Sky_Box>(parent->device(), L"./Data/Sky_Box/stage_4.dds");
        break;
    }

    case Stage_Select::STAGE_BOSS:
    {
        stage_spawner->set_stage_boss();
        //sky_box = std::make_unique<Sky_Box>(parent->device(), L"./Data/Sky_Box/stage_4.dds");
        break;
    }

    }

    // 登録したステージをupdateに移動させる
    Stage_Manager::instance().update(0.0f);
}

short* Scene_Game::init_enemy(const Stage_Select stage_, const DirectX::XMFLOAT3& target_position)
{
    std::unique_ptr<Enemy_Spawner>		enemy_spawner = nullptr;
    enemy_spawner = std::make_unique<Enemy_Spawner>(parent);


    short* ptr_boss_hp = nullptr;

    // TODO: debug 敵の動作確認
    //ptr_boss_hp = enemy_spawner->set_enemy<Boss>(player->get_position())->get_ability().get_ptr_hp();
    enemy_spawner->set_enemy<Enemy_Move_Closer_>({ 0.0f,5.0f,-120.0f }, target_position);


    switch (stage_)
    {

    case Stage_Select::STAGE_1:
    {
        enemy_spawner->set_enemy_1(target_position);
        break;
    }

    case Stage_Select::STAGE_2:
    {
        enemy_spawner->set_enemy_2(target_position);
        break;
    }

    case Stage_Select::STAGE_3:
    {
        enemy_spawner->set_enemy_3(target_position);
        break;
    }

    case Stage_Select::STAGE_4:
    {
        enemy_spawner->set_enemy_4(target_position);
        break;
    }

    case Stage_Select::STAGE_BOSS:
    {
        ptr_boss_hp = enemy_spawner->set_enemy_boss(target_position);
        break;
    }

    }



    return ptr_boss_hp;
}




bool Scene_Game::judge_clear()
{
    // クリア判定が出ていないか調べる
    if (clear_judge->judge() == false) return false;


    //　↓　クリア判定！　↓


    //現在のステージをクリア状態にする
    Option_Manager* opm = parent->option_manager();

    switch (CAST_I(opm->get_now_stage()))
    {
    case CAST_I(Stage_Select::STAGE_1):
    {
        opm->get_binary().clear_flag[CAST_I(opm->get_now_stage())] = true;
        break;
    }

    case CAST_I(Stage_Select::STAGE_2):
    {
        opm->get_binary().clear_flag[CAST_I(opm->get_now_stage())] = true;
        break;
    }

    case CAST_I(Stage_Select::STAGE_3):
    {
        opm->get_binary().clear_flag[CAST_I(opm->get_now_stage())] = true;
        break;
    }

    case CAST_I(Stage_Select::STAGE_4):
    {
        opm->get_binary().clear_flag[CAST_I(opm->get_now_stage())] = true;
        break;
    }

    }


    // カメラのゴール演出が終わったフレームか、Entityの更新が止まっていたら....            if文を通って早期returnさせる
    if(camera_controller->get_is_performance_end() || Entity_Manager::instance().get_is_update_stop())
    {
        // ここの処理を通った以降は、シーンが切り替えられない限り      （Scene_GameのInitialize()で動くようにsetterで設定している）
        // Entityは更新されない
        Entity_Manager::instance().set_update_stop();
        return false;
    }


    // 上以外の
    // クリア用処理
    camera_controller->set_clear();
    {
        // プレイヤーが乗っている乗り物を探して
        const std::vector<short> vec = Entity_Manager::instance().get_entities(Tag::Vehicle);
        if (int size = CAST_I(vec.size()))
        {
            for(int i = 0; i < size;++i)
            {
                std::shared_ptr<Entity> player_vehicle = Entity_Manager::instance().get_entity(vec.at(0));

                if(static_cast<Sphere_Vehicle*>(player_vehicle.get())->get_is_free() == true) continue;

                // プレイヤーが乗っている乗り物の速度を0.0にする
                player_vehicle->set_velocity_x(0.0f);
                player_vehicle->set_velocity_z(0.0f);
                // ジャンプ操作ができないようにする
                static_cast<Sphere_Vehicle*>(player_vehicle.get())->set_no_jump();

                // おわり！
                break;
            }
            
        }
    }

    return true;
}
