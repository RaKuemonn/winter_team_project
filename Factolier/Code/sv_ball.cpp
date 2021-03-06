

#include "sv_ball.h"
#include "sphere_vehicle.h"
#include "model_filepaths.h"

//SV_Ball::SV_Ball(Scene_Manager* ptr_scene_manager_)
//{
//    m_implements = std::make_unique<Sphere_Vehicle>(ptr_scene_manager_);
//}
//
//void SV_Ball::update(const float elapsed_time_)
//{
//    m_implements->update(elapsed_time_);
//}
//
//void SV_Ball::render()
//{
//    m_implements->render();
//}


SV_Ball::SV_Ball(Scene_Manager* ptr_scene_manager_) : Sphere_Vehicle(ptr_scene_manager_, Model_Paths::Entity::ball)
{
    
}

void SV_Ball::update(const float elapsed_time_)
{
    Sphere_Vehicle::update(elapsed_time_);
}