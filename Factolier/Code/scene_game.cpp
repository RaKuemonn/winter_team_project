
#include "scene_game.h"

#include "scene_manager.h"
#include "shader_manager.h"
#include "camera.h"
#include "entity_manager.h"
#include "stage_manager.h"
#include "Edit_Player.h"
#include "enemy_move_closer_.h"
#include "enemy_spring.h"
#include "camera_controller.h"
#include "imgui.h"
#include "player.h"
#include "utility.h"
#include "sphere_vehicle.h"
#include "scene_loading.h"
#include "scene_select.h"

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


#ifdef NDEBUG
    std::shared_ptr<Entity> player = std::make_shared<Player>(parent);
#else
    std::shared_ptr<Entity> player = std::make_shared<Player>(parent);
    //std::shared_ptr<Entity> player = std::make_shared<Edit_Player>(parent);
#endif

    Entity_Manager::instance().spawn_register(player);


    //???D????????????
    clear_back = std::make_unique<Sprite>(parent->device(), "./Data/Sprite/clear.png");
    spr_key     = std::make_unique<Sprite>(parent->device(), "./Data/Sprite/key.png");
    spr_mouse   = std::make_unique<Sprite>(parent->device(), "./Data/Sprite/mouse_left.png");


    water = std::make_unique<Model>(parent->model_manager()->load_model("./Data/Model/water.fbx"));



    // ??????????????????????????????
#ifdef NDEBUG
    const Stage_Select stage_num = parent->option_manager()->get_now_stage();
#else
    // TODO: debug ????????????????????????????????????
    const Stage_Select stage_num = parent->option_manager()->get_now_stage();
    //const Stage_Select stage_num = Stage_Select::STAGE_1;
#endif

    // ????????????????????????
    init_player_position(stage_num, player);

    // ?????????????????????
    init_stage(stage_num);

    // ????????????
    short* ptr_boss_hp =
        init_enemy(stage_num, player->get_position());


    collision_manager   = std::make_unique<Collision_Manager>   (stage_num);
    clear_judge         = std::make_unique<Clear_Judge>         (stage_num, player->get_position(), ptr_boss_hp);
    camera_controller   = std::make_unique<Camera_Controller>(&player->get_position());
    

    debug_decorator_supporter = std::make_unique<Decotator_Supporter>(parent_, player->get_position());

    bgm_stage1 = std::make_unique<Sound>(parent->sound_manager()->load_sound(L"./Data/Sound/select_BGM.wav"));


    
    bgm_stage1->play(true);

}


void Scene_Game::uninitialize()
{
    Entity_Manager::instance().all_clear();
    Stage_Manager::instance().all_clear();
}


void Scene_Game::update(float elapsed_time)
{
    bgm_stage1->set_volume(parent->option_manager()->bgm_vo);


    if (parent->input_manager()->TRG(0) & KEY_ESC && !clear_judge->judge())
    {
        if (!is_option)
        {
            is_option = true;
        }

        else
        {
            is_option = false;
        }
    }

    if (is_option)
    {
        parent->option_manager()->update(elapsed_time, parent->input_manager());

        if (parent->input_manager()->TRG(0) & PAD_START)
        {
            if (parent->option_manager()->return_flag)
            {
                parent->change_scene(new Scene_Loading(new Scene_Select));
            }
        }

        return;
    }

    if (clear_end)
    {
        if (parent->input_manager()->TRG(0) & PAD_START)
        {
            parent->change_scene(new Scene_Loading(new Scene_Select));
            return;
        }
    }
    

    Stage_Manager::instance().update(elapsed_time);
    Stage_Manager::instance().imgui();


    judge_out_stage_player();

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

    parent->state_manager()->setBS(BS::COVERAGE);

    parent->state_manager()->setRS(RS::SOLID_NONE);


    Shader* shader = nullptr;
    ID3D11DeviceContext* ptr_device_context = parent->device_context();
    
    {
        shader = parent->shader_manager()->get_shader(Shaders::SHADOW);

        shader->begin(ptr_device_context, elapsed_time * 0.1f);

        Stage_Manager::instance().render(ptr_device_context);
        Entity_Manager::instance().render(ptr_device_context);

        shader->end(ptr_device_context);
    }

    //parent->state_manager()->setRS(RS::SOLID_BACK);
    
    {
        ID3D11RenderTargetView* rtv = parent->render_target_view();
        ID3D11DepthStencilView* dsv = parent->depth_stencil_view();
        FLOAT color[]{ 1.0f, 1.0f, 1.0f, 1.0f };

        ptr_device_context->ClearRenderTargetView(rtv, color);
        ptr_device_context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        ptr_device_context->OMSetRenderTargets(1, &rtv, dsv);
    }



    parent->state_manager()->setDS(DS::OFF_OFF);

    //
    {
        shader = parent->shader_manager()->get_shader(Shaders::SKY);

        shader->begin(parent->device_context());

        // TODO: debug
        sky_box->render(parent->device_context());

        shader->end(parent->device_context());
    }


    parent->state_manager()->setDS(DS::ON_ON);
    shader = parent->shader_manager()->get_shader(Shaders::PHONG);

    shader->begin(ptr_device_context);

    Stage_Manager::instance().render(ptr_device_context);
    Entity_Manager::instance().render(ptr_device_context);
    debug_decorator_supporter->render(ptr_device_context);

    shader->end(ptr_device_context);


    shader = parent->shader_manager()->get_shader(Shaders::OCEAN);

    parent->state_manager()->setRS(RS::SOLID_NONE);

    if (parent->option_manager()->get_now_stage() == Stage_Select::STAGE_2)
    {

        parent->state_manager()->setBS(BS::ALPHA);

        shader->begin(ptr_device_context, elapsed_time * 0.05f);

        //???????????????
        DirectX::XMFLOAT4X4 world = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        DirectX::XMMATRIX S = DirectX::XMMatrixScaling(50.0f, 50.0f, 50.0f);
        //DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
        //DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, 0.15f, 1.0f);
        DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(0), 0, DirectX::XMConvertToRadians(0));
        DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(0, 0, 0);
        DirectX::XMMATRIX W = S * R * T;

        DirectX::XMStoreFloat4x4(&world, W);

        water->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 0.5f });

        shader->end(ptr_device_context);
    }

    //2D??????
    {
        parent->state_manager()->setDS(DS::OFF_OFF);

        //parent->state_manager()->setBS(BS::ALPHA);

        parent->state_manager()->setRS(RS::SOLID_NONE);

        if(parent->option_manager()->get_binary().clear_flag[0] == false)
        {
            if(parent->input_manager()->TRG(0) & KEY_SPACE)
            {
                once_space = true;
            }


            if(!once_space)
            {
                spr_key->render(ptr_device_context,
                    0, 0,  //position
                    1.0f, 1.0f,     // scal
                    0, 0,    // ??????????????????????????????
                    1920, 1080,   // size
                    0.0, 0.0,         // pibot
                    1, 1, 1, 1,   // rgba
                    0.0f);
            }

            else
            {
                spr_mouse->render(ptr_device_context,
                    1550, 750,  //position
                    1.0f, 1.0f,     // scal
                    0, 0,    // ??????????????????????????????
                    200, 200,   // size
                    0.0, 0.0,         // pibot
                    1, 1, 1, 1,   // rgba
                    0.0f);
            }
            

            
        }


        if (is_option)
        {
            parent->option_manager()->game_render();
        }


        //???????????????
        if (clear_end)
        {
            clear_back->render(parent->device_context(),
                0, 0,  //position
                1.0f, 1.0f,     // scal
                0, 0,    // ??????????????????????????????
                1920, 1080,   // size
                0.0, 0.0,         // pibot
                1, 1, 1, 1,   // rgba
                0.0f);
        }
    }
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
        player_.lock()->set_position({ -4.5f, 9.0f, -83.0f });
        break;
    }

    case Stage_Select::STAGE_3:
    {
        player_.lock()->set_position({ 0.0f, -8.5f, -144.0f });
        break;
    }

    case Stage_Select::STAGE_4:
    {
        player_.lock()->set_position({ 0.0f, 2.2f, -128.0f });
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
        sky_box = std::make_unique<Sky_Box>(parent->device(), L"./Data/Sky_Box/stage_1.dds");
        break;
    }

    case Stage_Select::STAGE_2:
    {
        stage_spawner->set_stage_2();
        sky_box = std::make_unique<Sky_Box>(parent->device(), L"./Data/Sky_Box/stage_2.dds");
        break;
    }

    case Stage_Select::STAGE_3:
    {
        stage_spawner->set_stage_3();
        sky_box = std::make_unique<Sky_Box>(parent->device(), L"./Data/Sky_Box/stage_3.dds");
        break;
    }

    case Stage_Select::STAGE_4:
    {
        stage_spawner->set_stage_4();
        sky_box = std::make_unique<Sky_Box>(parent->device(), L"./Data/Sky_Box/stage_4.dds");
        break;
    }

    case Stage_Select::STAGE_BOSS:
    {
        stage_spawner->set_stage_boss();
        sky_box = std::make_unique<Sky_Box>(parent->device(), L"./Data/Sky_Box/stage_4.dds");
        break;
    }

    }

    // ???????????????????????????update??????????????????
    Stage_Manager::instance().update(0.0f);
}

short* Scene_Game::init_enemy(const Stage_Select stage_, const DirectX::XMFLOAT3& target_position)
{
    std::unique_ptr<Enemy_Spawner>		enemy_spawner = nullptr;
    enemy_spawner = std::make_unique<Enemy_Spawner>(parent);


    short* ptr_boss_hp = nullptr;

    // TODO: debug ??????????????????
    //ptr_boss_hp = enemy_spawner->set_enemy<Boss>(player->get_position())->get_ability().get_ptr_hp();
    //enemy_spawner->set_enemy<Enemy_Move_Closer_>({ 0.0f,5.0f,-120.0f }, target_position);
    //enemy_spawner->set_enemy<Enemy_Spring>({ 0.0f,5.0f,-120.0f }, target_position);


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
    // ?????????????????????????????????????????????
    if (clear_judge->judge() == false) return false;


    //?????????????????????????????????


    //????????????????????????????????????????????????
    Option_Manager* opm = parent->option_manager();

    switch (CAST_I(opm->get_now_stage()))
    {
    case CAST_I(Stage_Select::STAGE_1):
    {
        opm->get_binary().clear_flag[CAST_I(Stage_Select::STAGE_1)] = true;
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


    // ????????????????????????????????????????????????????????????Entity?????????????????????????????????....            if?????????????????????return?????????
    if(camera_controller->get_is_performance_end() || Entity_Manager::instance().get_is_update_stop())
    {
        // ?????????????????????????????????????????????????????????????????????????????????      ???Scene_Game???Initialize()??????????????????setter????????????????????????
        // Entity?????????????????????
        Entity_Manager::instance().set_update_stop();
        clear_end = true;
        return false;
    }


    // ????????????
    // ??????????????????
    camera_controller->set_clear();
    {
        // ??????????????????????????????????????????????????????
        const std::vector<short> vec = Entity_Manager::instance().get_entities(Tag::Vehicle);
        if (int size = CAST_I(vec.size()))
        {
            for(int i = 0; i < size;++i)
            {
                std::shared_ptr<Entity> player_vehicle = Entity_Manager::instance().get_entity(vec.at(i));

                if(static_cast<Sphere_Vehicle*>(player_vehicle.get())->get_is_free() == true) continue;

                // ??????????????????????????????????????????????????????0.0?????????
                player_vehicle->set_velocity_x(0.0f);
                player_vehicle->set_velocity_z(0.0f);
                // ????????????????????????????????????????????????
                static_cast<Sphere_Vehicle*>(player_vehicle.get())->set_no_jump();

                // ????????????
                break;
            }
            
        }

        std::shared_ptr<Entity> player = Entity_Manager::instance().get_entity(Entity_Manager::instance().get_entities(Tag::Player).front());

        if(player->get_tag() == Tag::Player)
        {
            static_cast<Player*>(player.get())->set_clear();
        }


    }

    return true;
}


bool Scene_Game::judge_out_stage_player()
{
    std::vector<short> vec = Entity_Manager::instance().get_entities(Tag::Vehicle);

    for(auto i : vec)
    {
        std::shared_ptr<Entity> player = Entity_Manager::instance().get_entity(i);

        if(static_cast<Sphere_Vehicle*>(player.get())->get_is_free()) continue;

        if (player->get_latest_position().y < -30.0f)
        {
            init_player_position(parent->option_manager()->get_now_stage(), player);
        }
    }
    return false;
}
