
#include "collide_detection_entity.h"
#include "transform.h"


Collide_Detection::Collide_Detection(Scene_Manager* ptr_scene_manager_, const float radius_) : radius(radius_)
{
    set_ptr_scene_manager(ptr_scene_manager_);
    //load_model(get_scene_manager()->model_manager()->load_model(Model_Paths::Entity::player, true));

    set_tag(Tag::Collide);

    //constexpr float scale = 0.005f;
    //get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();
}

void Collide_Detection::update(const float elapsed_time_)
{
    get_transform()->Update();
}

const float Collide_Detection::get_radius() const
{
    return radius;
}
