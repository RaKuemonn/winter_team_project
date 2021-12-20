
#include "sphere_vehicle.h"
#include "transform.h"
#include "model.h"
#include "scene_manager.h"


Sphere_Vehicle::Sphere_Vehicle(Scene_Manager* ptr_scene_manager_)
{
    set_ptr_scene_manager(ptr_scene_manager_);

    load_model(get_scene_manager()->model_manager()->load_model("./Data/ball_demo.fbx",true));

    set_tag(Tag::Vehicle);

    m_velocity = std::make_unique<Velocity>();
    m_velocity->set_mass(1.0f);

    constexpr float scale = 3.0f;
    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();
}

void Sphere_Vehicle::update(const float elapsed_time_)
{
    // ���x�̍X�V
    update_velocity(elapsed_time_);

    // �ʒu�̍X�V
    get_transform()->add_position(m_velocity->get());

    // ��]�̍X�V
    rotate(elapsed_time_);

    // �p���̍X�V
    get_transform()->Update();
}

void Sphere_Vehicle::render()
{
    using managers = Scene_Manager;

    managers* ptr_managers = get_scene_manager();
    ID3D11DeviceContext* ptr_device_context = ptr_managers->device_context();

    // �V�F�[�_�̐ݒ�
    ptr_managers->state_manager()->setDS(DS::ON_ON);
    Shader* ptr_shader = ptr_managers->shader_manager()->get_shader(Shaders::PHONG);


    // �[�@���f���̕`��@�[ //
    ptr_shader->begin(ptr_device_context);

    get_model()->render(ptr_device_context, get_transform()->get_matrix(), { 1.0f, 1.0f, 1.0f, 1.0f });

    ptr_shader->end(ptr_device_context);
    // �[�[�[�[�[�[�[�[�[�[ //
}

void Sphere_Vehicle::move_direction(const DirectX::XMFLOAT3& direction_)
{
    constexpr float speed = 5.0f;

    // �����̕����ɉ���
    m_velocity->add(direction_ * speed);
}


void Sphere_Vehicle::update_velocity(const float elapsed_time_)
{

    if (m_is_free) m_velocity->add(m_velocity->get());

    // ���x�̍X�V
    m_velocity->update(elapsed_time_);
}

void Sphere_Vehicle::rotate(const float elapsed_time_)
{
    const DirectX::XMVECTOR xmvector_velocity = DirectX::XMLoadFloat3(&m_velocity->get());
    const float velocity_length = DirectX::XMVectorGetX(DirectX::XMVector3Length(xmvector_velocity));

    if (velocity_length < FLT_EPSILON) return;

    const DirectX::XMVECTOR xmvector_up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    const DirectX::XMVECTOR rotate_axis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(xmvector_up, xmvector_velocity));


    const DirectX::XMVECTOR xmvector_quaternion = DirectX::XMQuaternionRotationAxis(rotate_axis, -velocity_length * elapsed_time_);

    DirectX::XMFLOAT4 xmf4_quaternion = {};
    DirectX::XMStoreFloat4(&xmf4_quaternion, xmvector_quaternion);
    get_transform()->add_quaternion(xmf4_quaternion);
}
