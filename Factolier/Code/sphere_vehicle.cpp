
#include "sphere_vehicle.h"
#include "transform.h"
#include "model.h"
#include "scene_manager.h"
#include "camera.h"


Sphere_Vehicle::Sphere_Vehicle(Scene_Manager* ptr_scene_manager_)
{
    set_ptr_scene_manager(ptr_scene_manager_);
    load_model(get_scene_manager()->model_manager()->load_model("./Data/ball_demo.fbx"));
    m_velocity = std::make_unique<Velocity>();
    m_velocity->set_mass(1.0f);

    constexpr float scale = 0.01f;
    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();
}

void Sphere_Vehicle::update(const float elapsed_time_)
{
    // 入力値の受け取り
    input(input_direction, *get_scene_manager()->input_manager());
    
    // 速度の更新
    update_velocity(elapsed_time_);

    // 位置の更新
    get_transform()->add_position(m_velocity->get());

    // 回転の更新
    rotate(elapsed_time_);

    // 姿勢の更新
    get_transform()->Update();

    // 当たり判定
    collision();

    // モデルの更新
    get_model()->play_animation(elapsed_time_, 0);
}

void Sphere_Vehicle::render()
{
    using managers = Scene_Manager;

    managers* ptr_managers = get_scene_manager();
    ID3D11DeviceContext* ptr_device_context = ptr_managers->device_context();

    // シェーダの設定
    ptr_managers->state_manager()->setDS(DS::ON_ON);
    Shader* ptr_shader = ptr_managers->shader_manager()->get_shader(Shaders::PHONG);


    // ー　モデルの描画　ー //
    ptr_shader->begin(ptr_device_context);

    get_model()->render(ptr_device_context, get_transform()->get_matrix(), { 1.0f, 1.0f, 1.0f, 1.0f });

    ptr_shader->end(ptr_device_context);
    // ーーーーーーーーーー //
}

void Sphere_Vehicle::update_velocity(const float elapsed_time_)
{
    constexpr float speed = 5.0f;

    // 入力方向に加速
    m_velocity->add(input_direction * speed);

    // 速度の更新
    m_velocity->update(elapsed_time_);
}

void Sphere_Vehicle::rotate(const float elapsed_time_)
{
    const DirectX::XMVECTOR xmvector_velocity = DirectX::XMLoadFloat3(&m_velocity->get());
    const DirectX::XMVECTOR xmvector_up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    const DirectX::XMVECTOR rotate_axis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(xmvector_up, xmvector_velocity));


    const float velocity_length = DirectX::XMVectorGetX(DirectX::XMVector3Length(xmvector_velocity));
    const DirectX::XMVECTOR xmvector_quaternion = DirectX::XMQuaternionRotationAxis(rotate_axis, -velocity_length * elapsed_time_);

    DirectX::XMFLOAT4 xmf4_quaternion = {};
    DirectX::XMStoreFloat4(&xmf4_quaternion, xmvector_quaternion);
    get_transform()->add_quaternion(xmf4_quaternion);
}

void Sphere_Vehicle::collision()
{
    
}

void Sphere_Vehicle::input(DirectX::XMFLOAT3& input_direction_, Input_Manager& input_)
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