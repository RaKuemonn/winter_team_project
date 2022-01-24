
#if 0

#include "scene.h"
#include "model.h"
#include "sprite_batch.h"
#include "sound.h"
#include "model.h"
#include "camera_controller.h"
#include "character.h"

class Player : public Character
{
public:
    Player(Scene_Manager* sm);
    ~Player() {};

    

    void update(float elapsedTime);
    void move(float elapsedTime);
    void render();

public:
    // ‘¬“x
    DirectX::XMFLOAT3 speed = { 0, 0, 0 };
private:
    std::unique_ptr<Model> model = nullptr;

   

};

#endif


#include "entity.h"

enum class P_Anim
{
    stand,
    move,
    jump,
    attack,
    down,
    down_keep,
    joy
};


class Player final : public Entity
{
public:
    Player(class Scene_Manager* ptr_scene_manager_);
    ~Player()                               override = default;

    void init()                             override;
    void update(const float elapsed_time_)  override;

private:
    void update_vehicle(const float elapsed_time_, P_Anim& anim_num_);
    _NODISCARD bool check_has_vehicle() const;
    void control_vehicle();
    void reference_vehicle_position();
    void reference_vehicle_vector(const float elapsed_time_);
    void create_vehicle(const float elapsed_time_);

private:
    P_Anim anim_num = P_Anim::stand;
    std::weak_ptr<Entity> m_wkp_vehicle;
    DirectX::XMFLOAT3 input_direction = {};

};
