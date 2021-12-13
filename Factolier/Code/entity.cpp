
#include "entity.h"
#include "transform.h"


void Entity::init()
{
    m_transform = std::make_unique<Transform>();
}

const Transform* Entity::get_transform() const
{
    return m_transform.get();
}

const DirectX::XMFLOAT3& Entity::get_position() const
{
    return 
}

const DirectX::XMFLOAT3& Entity::get_axis_x() const
{
    
}

const DirectX::XMFLOAT3& Entity::get_axis_y() const
{

}

const DirectX::XMFLOAT3& Entity::get_axis_z() const
{
    
}
