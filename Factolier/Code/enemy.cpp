
#include "enemy.h"
#include "transform.h"
#include "scene_manager.h"


Enemy::Enemy(Scene_Manager* ptr_scene_manager_)
{

    set_ptr_scene_manager(ptr_scene_manager_);
    set_tag(Tag::Enemy);

}
