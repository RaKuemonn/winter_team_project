#pragma once

#include <DirectXMath.h>
#include <memory>

#include "model.h"
class Transform;
class Scene_Manager;

// エンティティクラス (抽象クラス)

class Entity
{
public:
    Entity();
    virtual ~Entity();

    virtual void init()                             = 0;
    virtual void update(const float elapsed_time)   = 0;
    virtual void render()                           = 0;

    // Getter関数 //
    _NODISCARD Model*                      get_model()         const;
    _NODISCARD const DirectX::XMFLOAT3     get_position()      const;
    _NODISCARD const DirectX::XMFLOAT3     get_scale()         const;
    _NODISCARD const DirectX::XMFLOAT3     get_axis_x()        const;
    _NODISCARD const DirectX::XMFLOAT3     get_axis_y()        const;
    _NODISCARD const DirectX::XMFLOAT3     get_axis_z()        const;

    // Setter関数 //
    void set_position(const DirectX::XMFLOAT3& position_)   const;
    void add_position(const DirectX::XMFLOAT3& velocity)    const;


protected:
    // 継承先のみで使える関数 (Scene_ManagerとTransformのポインタのアクセサ、Modelの読み込み)
    _NODISCARD Scene_Manager* get_scene_manager()           const;
    void set_ptr_scene_manager(Scene_Manager* ptr_scene_manager_);

    void load_model(std::shared_ptr<class Skinned_Mesh> model_);
    _NODISCARD Transform* get_transform();


private:
    std::unique_ptr<Model>  m_model             = nullptr;
    Transform*              m_transform         = nullptr;
    Scene_Manager*          m_ptr_scene_manager = nullptr;
};