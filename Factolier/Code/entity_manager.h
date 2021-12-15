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


    // GetterŠÖ” //


    // SetterŠÖ” //


private:
    std::vector<std::unique_ptr<Entity>> vec_entities = {};
};