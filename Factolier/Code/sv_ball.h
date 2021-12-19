#pragma once

#include "entity.h"

class SV_Ball final : public Entity
{
public:
    SV_Ball(class Scene_Manager* ptr_scene_manager_);
    ~SV_Ball()                              override = default;

    void init()                             override {};
    void update(const float elapsed_time_)  override;
    void render()                           override;
    
private:
    std::unique_ptr<Entity> m_implements = nullptr;

};