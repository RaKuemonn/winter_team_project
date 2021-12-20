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
    _NODISCARD Entity*              get_entity(const short& index)  const;    // index���͈͊O���ƃA�N�Z�X�ᔽ�ɂȂ�
    _NODISCARD std::vector<short>   get_entity(const Tag & tag_)    const;    // tag_�Ɠ����C���X�^���X��index��S�ĕԂ�

    // Setter�֐� //
    void spawn_register(std::unique_ptr<Entity>& entity);
    void spawn_register(std::unique_ptr<Entity>&& entity);
    void spawn_register(std::shared_ptr<Entity>& entity);
    void remove_register(Entity* entity);                                     // �����C���X�^���X��vec_removes�ɍēo�^�����ƁA�폜�������ɓ{����
    void all_clear();


private:
    std::vector<std::shared_ptr<Entity>> vec_entities = {};
    std::vector<std::shared_ptr<Entity>> vec_removes = {};
};