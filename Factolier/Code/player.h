
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



class Player final : public Entity
{
public:
    Player(class Scene_Manager* ptr_scene_manager_);
    ~Player()                               override = default;

    void init()                             override{};
    void update(const float elapsed_time_)  override;
    void render()                           override;

private:
    void reference_vehicle_position();
    void update_vehicle();
    _NODISCARD bool check_has_vehicle() const;
    void create_vehicle();
    void control_vehicle();
    inline static void input(DirectX::XMFLOAT3& input_direction, class Input_Manager& input_);

private:
    std::weak_ptr<Entity> m_wkp_vehicle;
    DirectX::XMFLOAT3 input_direction = {};

private:
    static constexpr float pudding_y = 5.0f;

};
