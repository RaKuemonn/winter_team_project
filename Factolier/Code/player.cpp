

#if 0

#include "player.h"
#include "scene_manager.h"
#include "camera.h"
#include "input_manager.h"

Player::Player(Scene_Manager* sm)
{
    scale = DirectX::XMFLOAT3{0.01f, 0.01f, 0.01f};
    speed.x = 0.5f;
    // parentの設定
    set_parent(sm);
    // player読み込み
    model = std::make_unique<Model>(parent->model_manager()->load_model("./Data/Jummo/Jummo.mdl"));
}


void Player::update(float elapsedTime)
{
    updatetransfrom();
    move(elapsedTime);
}

void Player::move(float elapsedTime)
{
    
    model->play_animation(elapsedTime,0, true );
    if(parent->input_manager()->STATE(0)& PAD_TRG1)
    {
        position.x += speed.x * elapsedTime;
    }
}



void Player::render()
{
     ID3D11DeviceContext* device_context_ = parent->device_context();

     parent->state_manager()->setSS(SS::POINT);
     parent->state_manager()->setSS(SS::LINEAR);
     parent->state_manager()->setSS(SS::ANISOTROPIC);

     parent->state_manager()->setDS(DS::OFF_OFF);

     parent->state_manager()->setBS(BS::ALPHA);

     parent->state_manager()->setRS(RS::SOLID_NONE);


     Shader* shader = parent->shader_manager()->get_shader(1);

  
     parent->state_manager()->setDS(DS::ON_ON);

     shader = parent->shader_manager()->get_shader(0);

     shader->begin(parent->device_context());

     model->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 1.0f });

     shader->end(parent->device_context());

}

#endif

#include "player.h"

#include "camera.h"
#include "entity_manager.h"
#include "transform.h"
#include "model.h"
#include "scene_manager.h"
#include "input_manager.h"
#include "sphere_vehicle.h"
#include "sv_ball.h"


Player::Player(Scene_Manager* ptr_scene_manager_)
{
    set_ptr_scene_manager(ptr_scene_manager_);
    load_model(get_scene_manager()->model_manager()->load_model("./Data/nico.fbx"));

    set_tag(Tag::Player);

    constexpr float scale = 0.01f;
    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();
}

void Player::init()
{
    get_transform()->Update();
}

void Player::update(const float elapsed_time_)
{
    // 入力値の受け取り
    input(input_direction, *get_scene_manager()->input_manager());

    // 乗り物の更新 ・ 位置の更新
    update_vehicle();
    

    // 姿勢の更新
    get_transform()->Update();

    // モデルの更新
    get_model()->play_animation(elapsed_time_, 0);
}


void Player::input(DirectX::XMFLOAT3& input_direction_, Input_Manager& input_)
{
    input_direction_ = {};


    if (input_.STATE(0) & PAD_UP)
    {
        input_direction_.z += 1.0f;
    }

    if (input_.STATE(0) & PAD_DOWN)
    {
        input_direction_.z += -1.0f;
    }

    if (input_.STATE(0) & PAD_RIGHT)
    {
        input_direction_.x += 1.0f;
    }

    if (input_.STATE(0) & PAD_LEFT)
    {
        input_direction_.x += -1.0f;
    }

    if(input_.TRG(0) & KEY_SPACE)
    {
         input_direction_.y += 1.0f;
    }
    
    //const DirectX::XMFLOAT3& camera_axis_z = Camera::Instance().get_front();
    //
    //input_direction_.x = input_direction_.x * camera_axis_z.x + input_direction_.z * camera_axis_z.x;
    //input_direction_.z = input_direction_.x * camera_axis_z.z + input_direction_.z * camera_axis_z.z;
}

void Player::update_vehicle()
{
    //  乗ってる乗り物があるか
    if(check_has_vehicle())
    {
        // あるので更新する

        if(get_scene_manager()->input_manager()->TRG(0) & PAD_START)
        {
            static_cast<Sphere_Vehicle*>(m_wkp_vehicle.lock().get())->set_is_free();

            return;
        }
            
        control_vehicle();
        reference_vehicle_position();
        return;
    }

    create_vehicle();
}

bool Player::check_has_vehicle() const
{
    // 参照先があるか          (expired()は参照先が無いときにtrueになる)
    if (m_wkp_vehicle.expired() == false)
    {
        return (static_cast<Sphere_Vehicle*>(m_wkp_vehicle.lock().get())->get_is_free() == false);
    }

    return false;
}

void Player::control_vehicle()
{
    static_cast<Sphere_Vehicle*>(m_wkp_vehicle.lock().get())->move_direction(input_direction);
}

void Player::reference_vehicle_position()
{
    // 1 frame 遅れている

    DirectX::XMFLOAT3 vehicle_position = m_wkp_vehicle.lock()->get_position();
    vehicle_position.y                += m_wkp_vehicle.lock()->get_scale().y;

    set_position(vehicle_position);

}

void Player::create_vehicle()
{
    DirectX::XMFLOAT4 quaternion = { 0.0f,0.0f,0.0f,1.0f };
    
    if (m_wkp_vehicle.expired() == false)
    {
        quaternion = m_wkp_vehicle.lock().get()->get_quaternion();
    }

    // weak_ptrを解放しておく
    m_wkp_vehicle.reset();

    //std::shared_ptr<Entity> vehicle = std::make_shared<Sphere_Vehicle>(get_scene_manager());
    std::shared_ptr<Entity> vehicle = std::make_shared<SV_Ball>(get_scene_manager());

    // 位置の設定
    DirectX::XMFLOAT3 position = get_position();
    position.y += -1.0f * pudding_y + 5.0f;
    vehicle->set_position(position);

    // 回転値の設定
    vehicle->set_quaternion(quaternion);

    // プレイヤーが参照する乗り物を変更
    m_wkp_vehicle = vehicle;

    Entity_Manager::instance().spawn_register(vehicle);
}



