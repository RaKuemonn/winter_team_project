

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
#include "scene_manager.h"
#include "transform.h"
#include "velocity.h"
#include "model.h"
#include "camera.h"


Player::Player(Scene_Manager* ptr_scene_manager_)
{
    set_ptr_scene_manager(ptr_scene_manager_);
    load_model(get_scene_manager()->model_manager()->load_model("./Data/nico.fbx"));
    m_velocity = std::make_unique<Velocity>();
    m_velocity->set_mass(1.0f);

    constexpr float scale = 0.01f;
    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();
}

void Player::update(const float elapsed_time_)
{
    // 入力値の受け取り
    input(input_direction, *get_scene_manager()->input_manager());

    // 速度の更新
    update_velocity(elapsed_time_);

    // 位置の更新
    get_transform()->add_position(m_velocity->get());

    // 姿勢の更新
    get_transform()->Update();

    // 当たり判定
    collision();

    // モデルの更新
    get_model()->play_animation(elapsed_time_, 0);
}

void Player::render()
{
    using managers = Scene_Manager;

    managers* ptr_managers = get_scene_manager();
    ID3D11DeviceContext* ptr_device_context = ptr_managers->device_context();
    Shader* ptr_shader = ptr_managers->shader_manager()->get_shader(1);

    // シェーダの設定
    ptr_managers->state_manager()->setDS(DS::ON_ON);
    ptr_shader = ptr_managers->shader_manager()->get_shader(0);


    // ー　モデルの描画　ー //
    ptr_shader->begin(ptr_managers->device_context());

    get_model()->render(ptr_managers->device_context(), get_transform()->get_matrix(), { 1.0f, 1.0f, 1.0f, 1.0f });

    ptr_shader->end(ptr_managers->device_context());
    // ーーーーーーーーーー //
}

void Player::input(DirectX::XMFLOAT3& input_direction_, Input_Manager& input_)
{
    input_direction_ = {};


    if (input_.TRG(0) & PAD_UP)
    {
        input_direction_.z += 1.0f;
    }

    if (input_.TRG(0) & PAD_DOWN)
    {
        input_direction_.z += -1.0f;
    }

    if (input_.TRG(0) & PAD_RIGHT)
    {
        input_direction_.x += 1.0f;
    }

    if (input_.TRG(0) & PAD_LEFT)
    {
        input_direction_.x += -1.0f;
    }
    
    const DirectX::XMFLOAT3& camera_axis_z = Camera::Instance().get_front();

    input_direction_.x = input_direction_.x * camera_axis_z.x + input_direction_.z * camera_axis_z.x;
    input_direction_.z = input_direction_.x * camera_axis_z.z + input_direction_.z * camera_axis_z.z;
}

void Player::update_velocity(const float elapsed_time_)
{
    constexpr float speed = 5.0f;

    // 入力方向に加速
    m_velocity->add(input_direction * speed);

    // 速度の更新
    m_velocity->update(elapsed_time_);
}

void Player::collision()
{
    
}
