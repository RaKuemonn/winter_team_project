
#include "stage.h"
#include "collision.h"
#include "transform.h"
#include "utility.h"

Stage::Stage(const DirectX::XMFLOAT3& position) : m_transform(new Transform())
{
    m_transform->set_position(position);
}

Stage::~Stage()
{
    safe_delete(m_transform);
}

void Stage::render(ID3D11DeviceContext* device_context)
{
    m_model->render(device_context,  m_transform->get_matrix(), { 1.0f, 1.0f, 1.0f, 1.0f });
}

bool Stage::ray_cast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, Hit_Result* hit_result_)
{
    return m_model->raycast(start, end, m_transform->get_matrix(), hit_result_);
}

Model* Stage::get_model() const
{
    return m_model.get();
}

void Stage::load_model(std::shared_ptr<Model_Resource> model_)
{
    m_model = std::make_unique<Model>(model_);
}

Transform* Stage::get_transform()
{
    return m_transform;
}