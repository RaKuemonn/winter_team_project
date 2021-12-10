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