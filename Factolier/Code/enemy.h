#pragma once

#include "entity.h"
#include "velocity.h"


class Enemy final : public Entity
{
public:
    Enemy(class Scene_Manager* ptr_scene_manager_);
    ~Enemy()                                override = default;

    virtual void init()                             override = 0;
    virtual void update(const float elapsed_time_)  override = 0;
    virtual void render()                           override = 0;

private:

};

