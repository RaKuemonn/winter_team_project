#include <DirectXMath.h>
#include "state_manager.h"
#include "scene.h"

class Scene_Manager;

class Character 
{
public:
    Character() {}
    virtual ~Character() {}

    // 位置取得
    const DirectX::XMFLOAT3& getposition() const { return position; }
    // 位置設定
    void setposition(const DirectX::XMFLOAT3& position) { this->position = position; }
    // 回転取得
    const DirectX::XMFLOAT3& getAngle() const { return angle; }
    // 回転設定
    void setangle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
    // スケール取得
    const DirectX::XMFLOAT3& getscale() const { return scale; }
    // スケール設定
    void setscale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }
   

    void updatetransfrom();
   

protected:
    DirectX::XMFLOAT3 position = { 0, 0, 0 };
    DirectX::XMFLOAT3 angle = { 0, 0, 0 };
    DirectX::XMFLOAT3 scale = { 1, 1, 1 };

    DirectX::XMFLOAT4X4 world = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    Scene_Manager* parent = nullptr;

   

public:
    void set_parent(Scene_Manager* sm) { parent = sm; }
    
};