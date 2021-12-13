
#include "entity.h"
#include "transform.h"


Entity::Entity() : m_transform(std::make_unique<Transform>())
{
    
}


// GetterŠÖ” //

const Transform* Entity::get_transform() const
{
    return m_transform.get();
}

const DirectX::XMFLOAT3& Entity::get_position() const
{
    return m_transform.get()->get_position();
}

const DirectX::XMFLOAT3& Entity::get_axis_x() const
{
    DirectX::XMFLOAT3 axis_x = {};
    DirectX::XMStoreFloat3(&axis_x, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_transform.get()->get_axis_x())));
    return axis_x;
}

const DirectX::XMFLOAT3& Entity::get_axis_y() const
{
    DirectX::XMFLOAT3 axis_y = {};
    DirectX::XMStoreFloat3(&axis_y, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_transform.get()->get_axis_y())));
    return axis_y;
}

const DirectX::XMFLOAT3& Entity::get_axis_z() const
{
    DirectX::XMFLOAT3 axis_z = {};
    DirectX::XMStoreFloat3(&axis_z, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_transform.get()->get_axis_z())));
    return axis_z;
}



// SetterŠÖ” //

void Entity::set_position(const DirectX::XMFLOAT3& position_) const
{
    m_transform.get()->set_position(position_);
}

void Entity::add_position(const DirectX::XMFLOAT3& velocity) const
{
    m_transform.get()->add_position(velocity);
}
