
#include "sphere_vehicle.h"
#include "transform.h"
#include "model.h"
#include "scene_manager.h"


Sphere_Vehicle::Sphere_Vehicle(Scene_Manager* ptr_scene_manager_)
{
    set_ptr_scene_manager(ptr_scene_manager_);
    load_model(get_scene_manager()->model_manager()->load_model("./Data/nico.fbx"));
    m_velocity = std::make_unique<Velocity>();
    m_velocity->set_mass(1.0f);

    constexpr float scale = 0.01f;
    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();
}

void Sphere_Vehicle::update(const float elapsed_time_)
{
    // 速度の更新
    update_velocity(elapsed_time_);

    // 位置の更新
    get_transform()->add_position(m_velocity->get());

    // 姿勢の更新
    get_transform()->Update();

    // 当たり判定
    collision();

    // モデルの更新
    get_model()->play_animation(elapsed_time_, 0);
}

void Sphere_Vehicle::render()
{
    using managers = Scene_Manager;

    managers* ptr_managers = get_scene_manager();
    ID3D11DeviceContext* ptr_device_context = ptr_managers->device_context();
    Shader* ptr_shader = ptr_managers->shader_manager()->get_shader(1);

    // シェーダの設定
    ptr_managers->state_manager()->setDS(DS::ON_ON);
    ptr_shader = ptr_managers->shader_manager()->get_shader(0);


    // ー　モデルの描画　ー //
    ptr_shader->begin(ptr_managers->device_context());

    get_model()->render(ptr_managers->device_context(), get_transform()->get_matrix(), { 1.0f, 1.0f, 1.0f, 1.0f });

    ptr_shader->end(ptr_managers->device_context());
    // ーーーーーーーーーー //
}

void Sphere_Vehicle::update_velocity(const float elapsed_time_)
{
    constexpr float speed = 5.0f;

    // 入力方向に加速
    m_velocity->add(input_direction * speed);

    // 速度の更新
    m_velocity->update(elapsed_time_);
}

void Sphere_Vehicle::collision()
{
    
}
