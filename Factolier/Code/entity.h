#pragma once

#include <DirectXMath.h>
#include <memory>

#include "model.h"
#include "velocity.h"
class Transform;
class Ability;
class Scene_Manager;
enum class Tag
{
    None,
    Player,
    Vehicle,
    Enemy,
    Collide,
};

// エンティティクラス (抽象クラス)

class Entity
{
public:
    Entity();
    virtual ~Entity();

    virtual void init()                              = 0;
    virtual void update(const float elapsed_time)    = 0;
            void render(ID3D11DeviceContext*);
    virtual void imgui() {};

public:
    // Getter関数 //
    _NODISCARD Model*                      get_model()          const;
    _NODISCARD const DirectX::XMFLOAT3&    get_velocity()       const;
    _NODISCARD const DirectX::XMFLOAT3&    get_latest_position()const;
    _NODISCARD const DirectX::XMFLOAT3&    get_position()       const;
    _NODISCARD const DirectX::XMFLOAT4&    get_quaternion()     const;
    _NODISCARD const DirectX::XMFLOAT3&    get_scale()          const;
    _NODISCARD const DirectX::XMFLOAT3     get_axis_x()         const;
    _NODISCARD const DirectX::XMFLOAT3     get_axis_y()         const;
    _NODISCARD const DirectX::XMFLOAT3     get_axis_z()         const;
    _NODISCARD const Tag&                  get_tag()            const;
    _NODISCARD       Ability&              get_ability()        const;
    _NODISCARD const float                 get_mass()           const;


    // Setter関数 //
    void set_position(const DirectX::XMFLOAT3& position_)       const;
    void set_scale(const DirectX::XMFLOAT3& scale_)             const;
    void set_friction(const float friction_)                    const;
    void set_velocity(const DirectX::XMFLOAT3& velocity)        const;
    void set_velocity_x(const float velocity_x)                 const;
    void set_velocity_y(const float velocity_y)                 const;
    void set_velocity_z(const float velocity_z)                 const;
    void set_quaternion(const DirectX::XMFLOAT4& quaternion_)   const;
    void add_position(const DirectX::XMFLOAT3& velocity)        const;
    void add_velocity(const DirectX::XMFLOAT3& velocity)        const;
    void add_quaternion(const DirectX::XMFLOAT4& quaternion_)   const;


protected:
    // 継承先のみで使える関数 (Scene_ManagerとTransformのポインタのアクセサ、Modelの読み込み)
    _NODISCARD Scene_Manager* get_scene_manager()           const;
    void set_ptr_scene_manager(Scene_Manager* ptr_scene_manager_);

    void load_model(std::shared_ptr<class Model_Resource> model_);
    _NODISCARD Transform* get_transform();

    void set_tag(Tag tag_);


    std::unique_ptr<Velocity>   m_velocity          = nullptr;
private:
    std::unique_ptr<Model>      m_model             = nullptr;
    Transform*                  m_transform         = nullptr;
    Ability*                    m_ability           = nullptr;
    Scene_Manager*              m_ptr_scene_manager = nullptr;
    Tag                         m_tag               = Tag::None;
};