
#include "entity.h"
#include "transform.h"
#include "scene_manager.h"


Entity::Entity() : m_transform(new Transform())
{
    
}

Entity::~Entity()
{
    delete m_transform;
    m_transform = nullptr;
}


// GetterŠÖ” //

Model* Entity::get_model() const
{
    return m_model.get();
}


const DirectX::XMFLOAT3 Entity::get_position() const
{
    return m_transform->get_position();
}

const DirectX::XMFLOAT3 Entity::get_scale() const
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




// SetterŠÖ” //

void Entity::set_position(const DirectX::XMFLOAT3& position_) const
{
    m_transform->set_position(position_);
}


void Entity::add_position(const DirectX::XMFLOAT3& velocity) const
{
    m_transform->add_position(velocity);
}



Scene_Manager* Entity::get_scene_manager() const
{
    return m_ptr_scene_manager;
}

void Entity::set_ptr_scene_manager(Scene_Manager* ptr_scene_manager_)
{
    m_ptr_scene_manager = ptr_scene_manager_;
}

void Entity::load_model(std::shared_ptr<Skinned_Mesh> model_)
{
    m_model = std::make_unique<Model>(model_);
}

Transform* Entity::get_transform()
{
    return m_transform;
}
