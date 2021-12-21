
#include "stage.h"
#include "collision.h"
#include "transform.h"
#include "utility.h"

Stage::Stage() : m_transform(new Transform())
{
    
}

Stage::~Stage()
{
    safe_delete(m_transform);
}


bool Stage::ray_cast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, Hit_Result* hit_result_)
{
    return m_model->raycast(start, end, m_transform->get_matrix(), hit_result_);
}
