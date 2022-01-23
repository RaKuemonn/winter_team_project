#pragma once
#include "entity.h"


class Collide_Detection final : public Entity
{
public:
    Collide_Detection(class Scene_Manager* ptr_scene_manager_, const float radius_);
    ~Collide_Detection()                               override = default;

    void init()                             override{};
    void update(const float elapsed_time_)  override;


    _NODISCARD const float get_radius()     const;

private:
    const float radius;
};
