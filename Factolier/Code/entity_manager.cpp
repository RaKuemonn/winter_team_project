
#include "entity_manager.h"


void Entity_Manager::update(const float elapsed_time)
{
    for(auto& entity : vec_entities)
    {
        entity->update(elapsed_time);

#ifdef _DEBUG
        //entity->imgui();
#endif

    }

    // entity->get_massage_update();

    for (auto& entity : vec_removes)
    {
        auto it = std::find(vec_entities.begin(), vec_entities.end(), entity);

        if (it != vec_entities.end())
        {
            vec_entities.erase(it);
        }

        if (entity == nullptr) continue;

        entity.reset();
    }
    vec_removes.clear();

}

void Entity_Manager::render()
{
    for (auto& entity : vec_entities)
    {
        entity->render();
    }
}

Entity* Entity_Manager::get_entity(const short& index) const
{
    return vec_entities.at(index).get();
}

std::vector<short> Entity_Manager::get_entity(const Tag& tag_) const
{
    std::vector<short> indices = {};

    short index = {};
    for(auto& entity : vec_entities)
    {
        if(entity->get_tag() == tag_)
        {
            indices.emplace_back(index);
        }

        ++index;
    }

    return indices;
}


void Entity_Manager::spawn_register(std::unique_ptr<Entity>& entity)
{
    vec_entities.emplace_back(std::move(entity));
}

void Entity_Manager::spawn_register(std::unique_ptr<Entity>&& entity)
{
    vec_entities.emplace_back(std::move(entity));
}

void Entity_Manager::spawn_register(std::shared_ptr<Entity>& entity)
{
    vec_entities.emplace_back(entity);
}


void Entity_Manager::remove_register(Entity* entity)
{
    vec_removes.emplace_back(entity);
}

void Entity_Manager::all_clear()
{
    vec_removes.clear();
    vec_entities.clear();
}
