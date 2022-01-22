
#include "sphere_vehicle.h"

#include "entity_manager.h"
#include "imgui.h"
#include "transform.h"
#include "model.h"
#include "scene_manager.h"
#include "Timer.h"

// �萔
constexpr float scale = 2.0f;

// ���Ԋu�ŃX�P�[��������Ă����֐�
// TODO : ���x��0.0f�ɋ߂Â��ɂ�ăX�P�[����0.0f�ɋ߂Â�����
inline void scale_decreases(const float elapsed_time, Sphere_Vehicle& me)
{
    if (me.get_is_free() == false) return;

    DirectX::XMFLOAT3 scale = me.get_scale();

    constexpr DirectX::XMFLOAT3 xmf3_decrease = { SPHERE_SCALE_DECREASE,SPHERE_SCALE_DECREASE,SPHERE_SCALE_DECREASE };

    scale += xmf3_decrease * elapsed_time;

    if(scale.x < 0.0f)
    {
        // �X�P�[���l���O�ɂȂ�����폜�\�񂷂�
        scale = {};

        Entity_Manager::instance().remove_register(&me);
    }

    me.set_scale(scale);
}



Sphere_Vehicle::Sphere_Vehicle(Scene_Manager* ptr_scene_manager_, const char* filename_)
{
    set_ptr_scene_manager(ptr_scene_manager_);

    load_model(get_scene_manager()->model_manager()->load_model(filename_,true));

    set_tag(Tag::Vehicle);
    
    m_velocity->set_mass(2.0f);
    m_velocity->set_friction(0.0f);

    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();

    //m_timer = std::make_unique<Timer>();
    //m_timer->Initialize(nullptr, COUNT::DOWN, 3.0);
}

void Sphere_Vehicle::update(const float elapsed_time_)
{
    // ���x�̍X�V
    update_velocity(elapsed_time_);

    // �ʒu�̍X�V
    get_transform()->add_position(m_velocity->get() * elapsed_time_);

    // ��]�̍X�V
    rotate(elapsed_time_);

    // �X�P�[���̍X�V
    scale_decreases(elapsed_time_, *this);

    // �p���̍X�V
    get_transform()->Update();
}

void Sphere_Vehicle::move_direction(const DirectX::XMFLOAT3& direction_)
{

    constexpr float speed = 30.0f;

    // �����̕����ɉ���
    m_velocity->add({ direction_.x * speed, 0.0f, direction_.z * speed });

    // TODO: �ꎞ�I�ɃR�����g�A�E�g���Ă���
    // ��x�ł����n���Ă���΁@������󂯕t����悤�ɂ��Ă���
    //if (m_on_ground == false)return;

    m_is_jump = static_cast<bool>(direction_.y);
}


void Sphere_Vehicle::update_velocity(const float elapsed_time_)
{
    if (m_is_free)
    {
        m_velocity->set_friction(0.01f);
        m_velocity->set_mass((get_scale().x < scale) ? get_scale().x : 1.0f);
    }

    const DirectX::XMFLOAT3 gravity = { 0.0f,(2.0f + m_velocity->get_mass()) * -9.8f,0.0f };
    m_velocity->add(gravity);

    // ���x�̍X�V
    m_velocity->update(elapsed_time_);


    m_velocity->set_y(m_velocity->get().y + static_cast<float>(m_is_jump) * 20.0f);

    m_is_jump = false;
}

void Sphere_Vehicle::rotate(const float elapsed_time_)
{
    const DirectX::XMFLOAT3 copy_velocity               = m_velocity->get();
    const DirectX::XMFLOAT3 y_0_velocity                = { copy_velocity.x,0.0f,copy_velocity.z }; // ���������̑��x�ɍi���Ă���

    const DirectX::XMVECTOR xmvector_velocity           = DirectX::XMLoadFloat3(&y_0_velocity);
    const float velocity_length                         = DirectX::XMVectorGetX(DirectX::XMVector3Length(xmvector_velocity));

    // �����Ă��Ȃ���Ή�]�����Ȃ�
    if (velocity_length * velocity_length <= FLT_EPSILON) return;

    const DirectX::XMFLOAT3 xmfloat3 = { 0.0f, 1.0f, 0.0f };
    const DirectX::XMVECTOR xmvector_up = DirectX::XMLoadFloat3(&xmfloat3);

    // xmvector_up��xmvector_velocity������̐���ɂ��邩�̊m�F
    const float dot                     = DirectX::XMVectorGetX(DirectX::XMVector3Dot(xmvector_up, DirectX::XMVector3Normalize(xmvector_velocity)));
    constexpr  float epsilon = 1.0f - FLT_EPSILON;
    if (std::abs(dot) > 0.98f) return;


    const DirectX::XMVECTOR rotate_axis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(xmvector_up, xmvector_velocity));

    const DirectX::XMVECTOR xmvector_quaternion = DirectX::XMQuaternionRotationAxis(rotate_axis, velocity_length / get_scale().x * 0.5f * 0.01f);

    DirectX::XMFLOAT4 xmf4_quaternion = {};
    DirectX::XMStoreFloat4(&xmf4_quaternion, xmvector_quaternion);
    get_transform()->add_quaternion(xmf4_quaternion);
}
