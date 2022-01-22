#include "boss_body.h"
#include "transform.h"
using namespace body;

Boss_Body::Boss_Body(Scene_Manager* ptr_scene_manager_, const char* filename_, const DirectX::XMFLOAT3& target_position_) : Enemy(ptr_scene_manager_, filename_, target_position_)
{
    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();
}