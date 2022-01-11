#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "model.h"
class Transform;

class Stage
{
public:
    Stage();
    virtual ~Stage();

    void         render(ID3D11DeviceContext* device_context);
    virtual void update(const float elapsed_time)               = 0;
    virtual bool ray_cast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, struct Hit_Result* hit_result_); // 動作するステージ用に仮想関数にしている


    _NODISCARD Model* get_model()         const;

protected:
    void load_model(std::shared_ptr<Model_Resource> model_);
    _NODISCARD Transform* get_transform();

private:
    std::unique_ptr<Model>  m_model     = nullptr;
    Transform*              m_transform = nullptr;
};