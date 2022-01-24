#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "model.h"
class Transform;

namespace  stage
{
    constexpr DirectX::XMFLOAT3 scale = {0.2f,0.2f,0.2f };
};

class Stage
{
public:
    Stage(const DirectX::XMFLOAT3& position = {});
    virtual ~Stage();

    void         render(ID3D11DeviceContext* device_context);
    virtual void update(const float elapsed_time)               = 0;
    virtual bool ray_cast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, struct Hit_Result* hit_result_); // 動作するステージ用に仮想関数にしている


    _NODISCARD Model* get_model()         const;

    _NODISCARD bool get_is_raycast() const { return is_raycast; }

protected:
    void load_model(std::shared_ptr<Model_Resource> model_);
    _NODISCARD Transform* get_transform();

    _NODISCARD void set_is_raycast(bool is_) { is_raycast = is_; }      // レイキャストしたくないstageオブジェクトはこの関数でis_raycastをfalseにする
private:
    std::unique_ptr<Model>  m_model     = nullptr;
    Transform*              m_transform = nullptr;
    bool is_raycast                     = true;
};