
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
//#include "velocity.h"


class Player final : public Entity
{
public:
    Player(class Scene_Manager* ptr_scene_manager_);
    ~Player()                               override = default;

    void init()                             override{};
    void update(const float elapsed_time_)  override;
    void render()                           override;

private:
    void check_has_vehicle();
    void ride_on_vehicle();

private:
    std::weak_ptr<Entity> wkp_vehicle;


/*
private:
    inline static void input(DirectX::XMFLOAT3& input_direction, class Input_Manager& input_);
    void update_velocity(const float elapsed_time_);

private:
    std::unique_ptr<Velocity> m_velocity = nullptr;
    DirectX::XMFLOAT3 input_direction = {};
*/
};
