#pragma once

#include <DirectXMath.h>
#include <memory>

class Transform;

class Entity
{
public:
    Entity();
    virtual ~Entity()   = default;

    virtual void init() = 0;

    // Getterä÷êî //
    _NODISCARD const Transform*         get_transform()     const;
    _NODISCARD const DirectX::XMFLOAT3& get_position()      const;
    _NODISCARD const DirectX::XMFLOAT3& get_axis_x()        const;
    _NODISCARD const DirectX::XMFLOAT3& get_axis_y()        const;
    _NODISCARD const DirectX::XMFLOAT3& get_axis_z()        const;

    // Setterä÷êî //
    void set_position(const DirectX::XMFLOAT3& position_)   const;
    void add_position(const DirectX::XMFLOAT3& velocity)    const;




private:
    std::unique_ptr<Transform>          m_transform = nullptr;
    std::unique_ptr<class Skinned_Mesh> m_model     = nullptr;
};