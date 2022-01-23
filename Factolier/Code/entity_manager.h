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
    void render(ID3D11DeviceContext* ptr_device_context);


    // Getter関数 //
    _NODISCARD std::shared_ptr<Entity> get_entity(Entity* entity)      const;
    _NODISCARD std::shared_ptr<Entity> get_entity(const short& index)  const;    // indexが範囲外だとアクセス違反になる
    _NODISCARD std::vector<short>      get_entities(const Tag & tag_)  const;    // tag_と同じインスタンスのindexを全て返す
    _NODISCARD bool                    get_is_update_stop()            const { return m_is_update_stop; }

    // Setter関数 //
    void spawn_register(std::unique_ptr<Entity>& entity);
    void spawn_register(std::unique_ptr<Entity>&& entity);
    void spawn_register(std::shared_ptr<Entity>& entity);
    void remove_register(Entity* entity);                                     // 同じインスタンスがvec_removesに再登録されると、削除処理中に怒られる。  あと"""nullチェック"""してない。
    void all_clear();
    void set_update_stop() { m_is_update_stop = true; };
    void set_update_move() { m_is_update_stop = false; };

private:
    bool m_is_update_stop = false;
    std::vector<std::shared_ptr<Entity>> vec_entities   = {};
    std::vector<std::shared_ptr<Entity>> vec_registers  = {};
    std::vector<std::shared_ptr<Entity>> vec_removes    = {};
};