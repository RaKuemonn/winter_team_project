
#include "entity_manager.h"
#include "scene_manager.h"


void Entity_Manager::update(const float elapsed_time)
{
    for(auto& entity : vec_registers)
    {
        vec_entities.emplace_back(entity);
    }
    vec_registers.clear();


    for (auto& entity : vec_entities)
    {
        entity->update(elapsed_time);
    }

    for (auto& entity : vec_removes)
    {
        auto it = std::find(vec_entities.begin(), vec_entities.end(), entity);

        if (it != vec_entities.end())
        {
            vec_entities.erase(it);
        }

        if (entity.get() == nullptr) continue;
        entity.reset();
    }
    vec_removes.clear();

}

void Entity_Manager::render(ID3D11DeviceContext* ptr_device_context)
{
    for (auto& entity : vec_entities)
    {
        entity->render(ptr_device_context);
    }
}

std::shared_ptr<Entity> Entity_Manager::get_entity(Entity* entity_) const
{
    //auto it = std::find(vec_entities.begin(), vec_entities.end(), entity_);
    //return *it;

    for (auto& entity : vec_entities)
    {
        if(entity.get() != entity_) continue;
    
        return entity;
    }
}

std::shared_ptr<Entity> Entity_Manager::get_entity(const short& index) const
{
    return vec_entities.at(index);
}

std::vector<short> Entity_Manager::get_entities(const Tag& tag_) const
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
    vec_registers.emplace_back(std::move(entity));
}

void Entity_Manager::spawn_register(std::unique_ptr<Entity>&& entity)
{
    vec_registers.emplace_back(std::move(entity));
}

void Entity_Manager::spawn_register(std::shared_ptr<Entity>& entity)
{
    vec_registers.emplace_back(entity);
}


void Entity_Manager::remove_register(Entity* entity)
{
    //vec_removes.emplace_back(entity);
    vec_removes.emplace_back(get_entity(entity));
}

void Entity_Manager::all_clear()
{
    vec_removes.clear();
    vec_registers.clear();
    vec_entities.clear();
}
