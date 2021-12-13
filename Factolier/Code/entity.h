#pragma once

#include <DirectXMath.h>
#include <memory>

class Transform;

class Entity
{
public:
    Entity()            = default;
    virtual ~Entity()   = default;

    virtual void init();

    // Getterä÷êî //
    _NODISCARD const Transform* get_transform() const;
    _NODISCARD const DirectX::XMFLOAT3& get_position() const;
    _NODISCARD const DirectX::XMFLOAT3& get_axis_x() const;
    _NODISCARD const DirectX::XMFLOAT3& get_axis_y() const;
    _NODISCARD const DirectX::XMFLOAT3& get_axis_z() const;

    // Setterä÷êî //


private:
    std::unique_ptr<Transform>          m_transform = nullptr;
    std::unique_ptr<class Skinned_Mesh> m_model     = nullptr;
};