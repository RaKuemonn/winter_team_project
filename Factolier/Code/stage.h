#pragma once

#include <DirectXMath.h>
#include "model.h"
class Transform;

class Stage
{
public:
    Stage();
    virtual ~Stage();

    virtual void update(const float elapsed_time)   = 0;
    virtual void render()                           = 0;
    bool ray_cast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, struct Hit_Result* hit_result_);

private:
    std::unique_ptr<Model>  m_model     = nullptr;
    Transform*              m_transform = nullptr;
};