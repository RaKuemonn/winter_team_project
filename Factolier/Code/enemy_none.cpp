
#include "enemy_none.h"
#include "transform.h"
#include "model.h"
#include "scene_manager.h"



Enemy_None::Enemy_None(Scene_Manager* ptr_scene_manager_) : Enemy(ptr_scene_manager_)
{
    load_model(get_scene_manager()->model_manager()->load_model("./Data/nico.fbx"));

    constexpr float scale = 0.01f;
    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();
}


void Enemy_None::update(const float elapsed_time_)
{
    // �p���̍X�V
    get_transform()->Update();

    // ���f���̍X�V
    get_model()->play_animation(elapsed_time_, 0);

}

void Enemy_None::render()
{

    using managers = Scene_Manager;

    managers* ptr_managers = get_scene_manager();
    ID3D11DeviceContext* ptr_device_context = ptr_managers->device_context();

    // �V�F�[�_�̐ݒ�
    ptr_managers->state_manager()->setDS(DS::ON_ON);
    Shader* ptr_shader = ptr_managers->shader_manager()->get_shader(Shaders::PHONG);


    // �[�@���f���̕`��@�[ //
    ptr_shader->begin(ptr_device_context);

    get_model()->render(ptr_device_context, get_transform()->get_matrix(), { 1.0f, 1.0f, 1.0f, 1.0f });

    ptr_shader->end(ptr_device_context);
    // �[�[�[�[�[�[�[�[�[�[ //
}
