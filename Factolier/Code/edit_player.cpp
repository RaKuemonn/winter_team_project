
#include "edit_player.h"

#include "camera.h"
#include "model_filepaths.h"
#include "scene_manager.h"
#include "transform.h"

inline void input(DirectX::XMFLOAT3& input_direction_, Input_Manager& input_)
{
    input_direction_ = {};


    if (input_.STATE(0) & PAD_UP)
    {
        input_direction_.z += 1.0f;
    }

    if (input_.STATE(0) & PAD_DOWN)
    {
        input_direction_.z += -1.0f;
    }

    if (input_.STATE(0) & PAD_RIGHT)
    {
        input_direction_.x += 1.0f;
    }

    if (input_.STATE(0) & PAD_LEFT)
    {
        input_direction_.x += -1.0f;
    }

    if (input_.STATE(0) & KEY_SPACE)
    {
        input_direction_.y += 0.5f;
    }

    if (input_.STATE(0) & KEY_L_SHIFT)
    {
        input_direction_.y += -0.5f;
    }


    DirectX::XMFLOAT2 direction = {};
    DirectX::XMStoreFloat2(&direction, DirectX::XMVector2Normalize(DirectX::XMVectorSet(input_direction_.x, input_direction_.z, 0.0f, 0.0f)));

    // カメラ方向に
    const DirectX::XMFLOAT3& camera_axis_x = Camera::Instance().get_right();
    const DirectX::XMFLOAT3& camera_axis_z = Camera::Instance().get_front();

    input_direction_.x = direction.x * camera_axis_x.x + direction.y * camera_axis_z.x;
    input_direction_.z = direction.x * camera_axis_x.z + direction.y * camera_axis_z.z;
}


Edit_Player::Edit_Player(Scene_Manager* ptr_scene_manager_)
{
    set_ptr_scene_manager(ptr_scene_manager_);
    load_model(get_scene_manager()->model_manager()->load_model(Model_Paths::Entity::ball, true));

    set_tag(Tag::Player);

    //m_velocity->set_mass(1.0f);

    constexpr float scale = 0.1f;
    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();
    

}

void Edit_Player::init()
{
    get_transform()->Update();
}

void Edit_Player::update(const float elapsed_time_)
{
    // 入力値の受け取り
    input(input_direction, *get_scene_manager()->input_manager());
    
    constexpr float speed = 9.0f;
    get_transform()->add_position(input_direction * elapsed_time_ * speed);

    // 姿勢の更新
    get_transform()->Update();


    // モデルの更新
    //get_model()->play_animation(elapsed_time_, 0);
}