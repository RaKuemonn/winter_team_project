

#if 0

#include "player.h"
#include "scene_manager.h"
#include "camera.h"
#include "input_manager.h"

Player::Player(Scene_Manager* sm)
{
    scale = DirectX::XMFLOAT3{0.01f, 0.01f, 0.01f};
    speed.x = 0.5f;
    // parentの設定
    set_parent(sm);
    // player読み込み
    model = std::make_unique<Model>(parent->model_manager()->load_model("./Data/Jummo/Jummo.mdl"));
}


void Player::update(float elapsedTime)
{
    updatetransfrom();
    move(elapsedTime);
}

void Player::move(float elapsedTime)
{
    
    model->play_animation(elapsedTime,0, true );
    if(parent->input_manager()->STATE(0)& PAD_TRG1)
    {
        position.x += speed.x * elapsedTime;
    }
}



void Player::render()
{
     ID3D11DeviceContext* device_context_ = parent->device_context();

     parent->state_manager()->setSS(SS::POINT);
     parent->state_manager()->setSS(SS::LINEAR);
     parent->state_manager()->setSS(SS::ANISOTROPIC);

     parent->state_manager()->setDS(DS::OFF_OFF);

     parent->state_manager()->setBS(BS::ALPHA);

     parent->state_manager()->setRS(RS::SOLID_NONE);


     Shader* shader = parent->shader_manager()->get_shader(1);

  
     parent->state_manager()->setDS(DS::ON_ON);

     shader = parent->shader_manager()->get_shader(0);

     shader->begin(parent->device_context());

     model->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 1.0f });

     shader->end(parent->device_context());

}

#endif

#include "player.h"

#include "camera.h"
#include "entity_manager.h"
#include "transform.h"
#include "model.h"
#include "scene_manager.h"
#include "input_manager.h"
#include "sphere_vehicle.h"
#include "sv_ball.h"
#include "model_filepaths.h"
#include <memory>

inline void input(DirectX::XMFLOAT3& input_direction_, Input_Manager& input_, P_Anim& anim, bool animetion_playing_)
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

    if (input_.TRG(0) & KEY_SPACE)
    {
        input_direction_.y += 1.0f;
        anim = P_Anim::jump;
    }



    if(input_direction_.x || input_direction_.z )
    {
        if (anim != P_Anim::jump || animetion_playing_ == false) // animがJumpでアニメーション中なら切り替えない
        {
            anim = P_Anim::move;
        }
    }
    else
    {
        if (anim != P_Anim::jump || animetion_playing_ == false) // animがJumpでアニメーション中なら切り替えない
        {
            anim = P_Anim::stand;
        }
    }


    

    DirectX::XMFLOAT2 direction = {};
    DirectX::XMStoreFloat2(&direction, DirectX::XMVector2Normalize(DirectX::XMVectorSet(input_direction_.x, input_direction_.z, 0.0f, 0.0f)));

    // カメラ方向に
    const DirectX::XMFLOAT3& camera_axis_x = Camera::Instance().get_right();
    const DirectX::XMFLOAT3& camera_axis_z = Camera::Instance().get_front();

    input_direction_.x = direction.x * camera_axis_x.x + direction.y * camera_axis_z.x;
    input_direction_.z = direction.x * camera_axis_x.z + direction.y * camera_axis_z.z;
}


Player::Player(Scene_Manager* ptr_scene_manager_)
{
    set_ptr_scene_manager(ptr_scene_manager_);
    load_model(get_scene_manager()->model_manager()->load_model(Model_Paths::Entity::player, true));

    set_tag(Tag::Player);

    constexpr float scale = 0.1f;
    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();

    walk = std::make_unique<Sound>(ptr_scene_manager_->sound_manager()->load_sound(L"./Data/Sound/crick.wav"));

}

void Player::init()
{
    get_transform()->Update();
}

void Player::update(const float elapsed_time_)
{

    // 入力値の受け取り
    input(input_direction, *get_scene_manager()->input_manager(), anim_num/* ジャンプ, 歩き, 待機 */, get_model()->get_anime_play_flag());

    // 乗り物の更新 ・ 位置の更新
    update_vehicle(elapsed_time_, anim_num);

    
    // 姿勢の更新
    get_transform()->Update();


    // モデルの更新
    get_model()->play_animation(elapsed_time_, CAST_I(anim_num));
}


void Player::update_vehicle(const float elapsed_time_, P_Anim& anim_num_)
{
    //  乗ってる乗り物があるか
    if(check_has_vehicle())
    {
        // あるので更新する

        if (static_cast<Sphere_Vehicle*>(m_wkp_vehicle.lock().get())->get_on_ground())
        {
            if (get_scene_manager()->input_manager()->TRG(0) & PAD_START)
            {
                // 地面にいて、Enterキーが押されたとき
                // 発射する
                static_cast<Sphere_Vehicle*>(m_wkp_vehicle.lock().get())->set_is_free();

                anim_num_ = P_Anim::attack;

                return;
            }
        }
        
            
        control_vehicle();
        reference_vehicle_position();
        reference_vehicle_vector(elapsed_time_);
        return;
    }

    create_vehicle(elapsed_time_);
}

bool Player::check_has_vehicle() const
{
    // 参照先があるか          (expired()は参照先が無いときにtrueになる)
    if (m_wkp_vehicle.expired() == false)
    {
        return (static_cast<Sphere_Vehicle*>(m_wkp_vehicle.lock().get())->get_is_free() == false);
    }

    return false;
}

void Player::control_vehicle()
{
    static_cast<Sphere_Vehicle*>(m_wkp_vehicle.lock().get())->move_direction(input_direction);
}

void Player::reference_vehicle_position()
{
    // 1 frame 遅れている

    DirectX::XMFLOAT3 vehicle_position = m_wkp_vehicle.lock()->get_latest_position();
    vehicle_position.y                += m_wkp_vehicle.lock()->get_scale().y;


    set_position(vehicle_position);

}

void Player::reference_vehicle_vector(const float elapsed_time_)
{
    // 1 frame 遅れている
    DirectX::XMFLOAT3 velocity = m_wkp_vehicle.lock()->get_velocity();
    velocity.y = 0.0f;

    const DirectX::XMVECTOR vehicle_move_vec = DirectX::XMLoadFloat3(&velocity);

    const float move_length = DirectX::XMVectorGetX(DirectX::XMVector3LengthEst(vehicle_move_vec));

    if (move_length <= FLT_EPSILON)return;

    const DirectX::XMVECTOR vehicle_move_dir    = DirectX::XMVector3Normalize(vehicle_move_vec);

    const DirectX::XMVECTOR axis_z              = DirectX::XMLoadFloat3(&get_axis_z());

    const DirectX::XMVECTOR cross   = DirectX::XMVector3Cross(axis_z, vehicle_move_dir);
    const float dot                 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(axis_z, vehicle_move_dir));

    if(std::abs(dot) >= 1.0f - FLT_EPSILON)return;

    const float radian = acosf(dot)/*回転量*/ /*回転方向*/ * 3.0f * elapsed_time_/*フレームレート*/;

    //const DirectX::XMVECTOR axis_y = DirectX::XMLoadFloat3(&get_axis_y());
    DirectX::XMFLOAT4 add_quaternion_;
    DirectX::XMStoreFloat4(&add_quaternion_, DirectX::XMQuaternionRotationAxis(cross, radian));

    add_quaternion(add_quaternion_);

}

void Player::create_vehicle(const float elapsed_time_)
{
    DirectX::XMFLOAT4 quaternion = { 0.0f,0.0f,0.0f,1.0f };
    DirectX::XMFLOAT3 velocity = {};
    
    if (m_wkp_vehicle.expired() == false)
    {
        quaternion = m_wkp_vehicle.lock().get()->get_quaternion();
        velocity = m_wkp_vehicle.lock().get()->get_velocity() * elapsed_time_;
    }

    // weak_ptrを解放しておく
    m_wkp_vehicle.reset();
    
    std::shared_ptr<Entity> vehicle = std::make_shared<SV_Ball>(get_scene_manager());

    // 位置の設定
    DirectX::XMFLOAT3 position = get_latest_position() - velocity;
    position.y += vehicle->get_scale().y + 0.5f;
    vehicle->set_position(position);

    // 回転値の設定
    vehicle->set_quaternion(quaternion);

    // プレイヤーが参照する乗り物を変更
    m_wkp_vehicle = vehicle;

    Entity_Manager::instance().spawn_register(vehicle);
}

