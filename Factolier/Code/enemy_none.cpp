
#include "enemy_none.h"
#include "transform.h"
#include "model.h"
#include "scene_manager.h"
#include "model_filepaths.h"



Enemy_None::Enemy_None(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_) : Enemy(ptr_scene_manager_, Model_Paths::Entity::enemy_none, {})
{
    load_model(get_scene_manager()->model_manager()->load_model("./Data/nico.fbx"));

    constexpr float scale = 0.01f;
    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();
}


void Enemy_None::update(const float elapsed_time_)
{
    // 姿勢の更新
    get_transform()->Update();

    // モデルの更新
    get_model()->play_animation(elapsed_time_, 0);

}