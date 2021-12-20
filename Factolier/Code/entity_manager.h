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


    // Getter関数 //
    _NODISCARD Entity*              get_entity(const short& index)  const;    // indexが範囲外だとアクセス違反になる
    _NODISCARD std::vector<short>   get_entity(const Tag & tag_)    const;    // tag_と同じインスタンスのindexを全て返す

    // Setter関数 //
    void spawn_register(std::unique_ptr<Entity>& entity);
    void spawn_register(std::unique_ptr<Entity>&& entity);
    void spawn_register(std::shared_ptr<Entity>& entity);
    void remove_register(Entity* entity);                                     // 同じインスタンスがvec_removesに再登録されると、削除処理中に怒られる
    void all_clear();


private:
    std::vector<std::shared_ptr<Entity>> vec_entities = {};
    std::vector<std::shared_ptr<Entity>> vec_removes = {};
};