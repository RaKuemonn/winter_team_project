#pragma once

#include "entity.h"
#include "velocity.h"


class Enemy : public Entity
{
public:
    Enemy(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_);
    virtual ~Enemy()                                override = default;

    virtual void init()                             override = 0;
    virtual void update(const float elapsed_time_)  override = 0;

protected:
    _NODISCARD const DirectX::XMFLOAT3& get_target_position() const { return target_position; }

private:
    const DirectX::XMFLOAT3& target_position;   // このゲームの敵は９割方プレイヤーの位置を使うので、変数をここに入れている。　（使わないもの : チュートリアル用の的）
};

