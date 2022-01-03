
#include "sphere_vehicle.h"
#include "transform.h"
#include "model.h"
#include "scene_manager.h"


Sphere_Vehicle::Sphere_Vehicle(Scene_Manager* ptr_scene_manager_)
{
    set_ptr_scene_manager(ptr_scene_manager_);

    load_model(get_scene_manager()->model_manager()->load_model("./Data/ball_demo.fbx",true));

    set_tag(Tag::Vehicle);
    
    m_velocity->set_mass(1.0f);

    constexpr float scale = 2.0f;
    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();
}

void Sphere_Vehicle::update(const float elapsed_time_)
{
    // 速度の更新
    update_velocity(elapsed_time_);

    // 位置の更新
    get_transform()->add_position(m_velocity->get());

    // 回転の更新
    rotate(elapsed_time_);

    // 姿勢の更新
    get_transform()->Update();
}

void Sphere_Vehicle::move_direction(const DirectX::XMFLOAT3& direction_)
{
    constexpr float speed = 5.0f;

    // 引数の方向に加速
    m_velocity->add(direction_ * speed);
}


void Sphere_Vehicle::update_velocity(const float elapsed_time_)
{

    //if (m_is_free) m_velocity->add(m_velocity->get());

    constexpr DirectX::XMFLOAT3 gravity = { 0.0f,-1.0f * 9.8f,0.0f };
    m_velocity->add(gravity);

    // 速度の更新
    m_velocity->update(elapsed_time_);
}

void Sphere_Vehicle::rotate(const float elapsed_time_)
{
    const DirectX::XMVECTOR xmvector_velocity           = DirectX::XMLoadFloat3(&m_velocity->get());
    const float velocity_length                         = DirectX::XMVectorGetX(DirectX::XMVector3Length(xmvector_velocity));

    if (velocity_length < FLT_EPSILON) return;

    const DirectX::XMFLOAT3 xmfloat3 = { 0.0f, 1.0f, 0.0f };
    const DirectX::XMVECTOR xmvector_up = DirectX::XMLoadFloat3(&xmfloat3);

    const float dot                     = DirectX::XMVectorGetX(DirectX::XMVector3Dot(xmvector_up, DirectX::XMVector3Normalize(xmvector_velocity)));
    constexpr  float epsilon = 1.0f - FLT_EPSILON;
    if (std::abs(dot) > 0.98f) return;

    const DirectX::XMVECTOR rotate_axis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(xmvector_up, xmvector_velocity));


    const DirectX::XMVECTOR xmvector_quaternion = DirectX::XMQuaternionRotationAxis(rotate_axis, velocity_length);

    DirectX::XMFLOAT4 xmf4_quaternion = {};
    DirectX::XMStoreFloat4(&xmf4_quaternion, xmvector_quaternion);
    get_transform()->add_quaternion(xmf4_quaternion);
}
