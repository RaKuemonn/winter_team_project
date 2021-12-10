#include <DirectXMath.h>
#include "state_manager.h"
#include "scene.h"

class Scene_Manager;

class Character 
{
public:
    Character() {}
    virtual ~Character() {}

    // �ʒu�擾
    const DirectX::XMFLOAT3& getposition() const { return position; }
    // �ʒu�ݒ�
    void setposition(const DirectX::XMFLOAT3& position) { this->position = position; }
    // ��]�擾
    const DirectX::XMFLOAT3& getAngle() const { return angle; }
    // ��]�ݒ�
    void setangle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
    // �X�P�[���擾
    const DirectX::XMFLOAT3& getscale() const { return scale; }
    // �X�P�[���ݒ�
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