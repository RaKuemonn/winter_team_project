#include "entity.h"
#include "transform.h"
#include "ability.h"
#include "scene_manager.h"
#include "utility.h"


Entity::Entity() : m_transform(new Transform()), m_ability(new Ability()), m_velocity(std::make_unique<Velocity>())
{
    
}

Entity::~Entity()
{
    safe_delete(m_transform);

    safe_delete(m_ability);

}

void Entity::render(ID3D11DeviceContext* device_context)
{
    get_model()->render(device_context, get_transform()->get_matrix(), { 1.0f, 1.0f, 1.0f, 1.0f });
}



// Getter

Model* Entity::get_model() const
{
    return m_model.get();
}

const DirectX::XMFLOAT3& Entity::get_velocity() const
{
    return m_velocity->get();
}

const DirectX::XMFLOAT3& Entity::get_position() const
{
    return m_transform->get_position();
}

const DirectX::XMFLOAT4& Entity::get_quaternion() const
{
    return m_transform->get_quaternion();
}

const DirectX::XMFLOAT3& Entity::get_scale() const
{
    return m_transform->get_scale();
}

const DirectX::XMFLOAT3 Entity::get_axis_x() const
{
    DirectX::XMFLOAT3 axis_x = {};
    DirectX::XMStoreFloat3(&axis_x, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_transform->get_axis_x())));
    return axis_x;
}

const DirectX::XMFLOAT3 Entity::get_axis_y() const
{
    DirectX::XMFLOAT3 axis_y = {};
    DirectX::XMStoreFloat3(&axis_y, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_transform->get_axis_y())));
    return axis_y;
}

const DirectX::XMFLOAT3 Entity::get_axis_z() const
{
    DirectX::XMFLOAT3 axis_z = {};
    DirectX::XMStoreFloat3(&axis_z, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_transform->get_axis_z())));
    return axis_z;
}

const Tag& Entity::get_tag() const
{
    return m_tag;
}

Ability& Entity::get_ability() const
{
    return *m_ability;
}

const float Entity::get_mass() const
{
    return m_velocity->get_mass();
}






// Setter

void Entity::set_position(const DirectX::XMFLOAT3& position_) const
{
    m_transform->set_position(position_);
}

void Entity::set_scale(const DirectX::XMFLOAT3& scale_) const
{
    m_transform->set_scale(scale_);
}

void Entity::set_friction(const float friction_) const
{
    m_velocity->set_friction(friction_);
}

void Entity::set_velocity(const DirectX::XMFLOAT3& velocity_) const
{
    m_velocity->set(velocity_);
}

void Entity::set_velocity_x(const float velocity_x) const
{
    m_velocity->set_x(velocity_x);
}

void Entity::set_velocity_y(const float velocity_y) const
{
    m_velocity->set_y(velocity_y);
}

void Entity::set_velocity_z(const float velocity_z) const
{
    m_velocity->set_z(velocity_z);
}

void Entity::set_quaternion(const DirectX::XMFLOAT4& quaternion_) const
{
    m_transform->set_quaternion(quaternion_);
}

void Entity::add_position(const DirectX::XMFLOAT3& velocity) const
{
    m_transform->add_position(velocity);
}

void Entity::add_velocity(const DirectX::XMFLOAT3& velocity_) const
{
    m_velocity->add(velocity_);
}

void Entity::add_quaternion(const DirectX::XMFLOAT4& quaternion_) const
{
    m_transform->add_quaternion(quaternion_);
}




Scene_Manager* Entity::get_scene_manager() const
{
    return m_ptr_scene_manager;
}

void Entity::set_ptr_scene_manager(Scene_Manager* ptr_scene_manager_)
{
    m_ptr_scene_manager = ptr_scene_manager_;
}

void Entity::load_model(std::shared_ptr<Model_Resource> model_)
{
    m_model = std::make_unique<Model>(model_);
}

Transform* Entity::get_transform()
{
    return m_transform;
}

void Entity::set_tag(Tag tag_)
{
    m_tag = tag_;
}
