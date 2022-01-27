
#include "enemy.h"
#include "transform.h"
#include "scene_manager.h"
#include "collide_detection_entity.h"
#include "entity_manager.h"


Enemy::Enemy(Scene_Manager* ptr_scene_manager_, const char* filename_, const DirectX::XMFLOAT3& target_position_, bool default_collde_is_, bool boss_) : target_position(target_position_)
{

    set_ptr_scene_manager(ptr_scene_manager_);
    load_model(get_scene_manager()->model_manager()->load_model(filename_, true));
    set_tag(Tag::Enemy);

    m_velocity->set_mass(1.0f);
    m_velocity->set_friction(0.7f);

    // ザコ敵用の当たり判定
    if(default_collde_is_)
    {
        constexpr float radius = 0.8f;
        std::shared_ptr<Collide_Detection> collide_detection;
        if (boss_)
        {
            collide_detection = std::make_shared<Collide_Detection>(ptr_scene_manager_, radius * 5.0f);
        }
        else
        {
            collide_detection = std::make_shared<Collide_Detection>(ptr_scene_manager_, radius);
        }
        wkp_collide_detection                                   = collide_detection;

        std::shared_ptr<Entity> e_collide_detection             = collide_detection;
        Entity_Manager::instance().spawn_register(e_collide_detection);
    }

}

void Enemy::update_velocity(const float elapsed_time_)
{
    constexpr DirectX::XMFLOAT3 gravity = { 0.0f,-3.0f * 9.8f,0.0f };
    m_velocity->add(gravity);

    // 速度の更新
    m_velocity->update(elapsed_time_);
}

bool Enemy::check_im_die()
{
    
    // expiredがfalseのとき参照されている
    if (wkp_collide_detection.expired() == false) // 参照が切れる処理collide_detectionが消されるのは、collision_managerで削除処理が入った後　
    {
        wkp_collide_detection.lock()->set_position(get_position());
        return false;
    }
    //TODO: てすと ->　右の処理でやろうとすると、削除される前でここを通るのでこのザコ敵が消えない..... entity_managerの削除処理を””前に一つ追加””する

    


    // 以下は参照が切れているときの処理


    // entityのupdate後にremove_entityの処理が通るので、entityフレーム間に削除登録すれば
    // そのentityは消えるので,このまま削除登録する


    Entity_Manager::instance().remove_register(this);

    return true;
}
