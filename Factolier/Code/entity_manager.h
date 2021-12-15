#pragma once

#include "entity.h"


class Entity_Manager final
{
private:
    Entity_Manager()    = default;
    ~Entity_Manager()   = default;

public:
    static Entity_Manager& instance()
    {
        static Entity_Manager instance_;
        return instance_;
    }

    void update(const float elapsed_time);
    void render();


    // Getter�֐� //
    _NODISCARD Entity* get_entity(const short& index) const;    // index���͈͊O���ƃA�N�Z�X�ᔽ�ɂȂ�

    // Setter�֐� //
    void spawn_register(std::unique_ptr<Entity>& entity);
    void remove_register(const Entity* entity);


private:
    std::vector<std::unique_ptr<Entity>> vec_entities = {};
    std::vector<Entity*> vec_removes = {};
};