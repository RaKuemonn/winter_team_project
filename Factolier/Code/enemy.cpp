
#include "enemy.h"
#include "transform.h"
#include "model.h"
#include "scene_manager.h"


Enemy::Enemy(Scene_Manager* ptr_scene_manager_)
{
    set_ptr_scene_manager(ptr_scene_manager_);
    load_model(get_scene_manager()->model_manager()->load_model("./Data/nico.fbx"));

    set_tag(Tag::Enemy);

    //m_velocity = std::make_unique<Velocity>();
    //m_velocity->set_mass(1.0f);



    constexpr float scale = 0.01f;
    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();
}

void Enemy::update(const float elapsed_time_)
{

    // 姿勢の更新
    get_transform()->Update();

    // モデルの更新
    //get_model()->play_animation(elapsed_time_, 0);
}

void Enemy::render()
{
    using managers = Scene_Manager;

    managers* ptr_managers = get_scene_manager();
    ID3D11DeviceContext* ptr_device_context = ptr_managers->device_context();

    // シェーダの設定
    ptr_managers->state_manager()->setDS(DS::ON_ON);
    Shader* ptr_shader = ptr_managers->shader_manager()->get_shader(Shaders::PHONG);


    // ー　モデルの描画　ー //
    ptr_shader->begin(ptr_device_context);

    get_model()->render(ptr_device_context, get_transform()->get_matrix(), { 1.0f, 1.0f, 1.0f, 1.0f });

    ptr_shader->end(ptr_device_context);
    // ーーーーーーーーーー //
}
