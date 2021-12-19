
#include "entity_manager.h"


void Entity_Manager::update(const float elapsed_time)
{
    for(auto& entity : vec_entities)
    {
        entity->update(elapsed_time);
    }

    // entity->get_massage_update();

    //for (auto itr = vec_removes.begin(); itr != vec_removes.end();)
    //{
    //    auto found_itr = std::find(vec_entities.begin(), vec_entities.end(), itr);
    //
    //    if (found_itr != vec_entities.end())
    //    {
    //        itr = vec_removes.erase(itr);
    //        found_itr->reset();
    //        vec_entities.erase(found_itr);
    //        continue;
    //    }
    //
    //    ++itr;
    //}

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

void Entity_Manager::spawn_register(std::unique_ptr<Entity>& entity)
{
    vec_entities.emplace_back(std::move(entity));
}

void Entity_Manager::remove_register(const Entity* entity)
{
    //vec_removes.emplace_back(entity);
}
