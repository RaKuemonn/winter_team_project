#pragma once

#include "entity.h"


class Enemy : public Entity
{
public:
    Enemy(class Scene_Manager* ptr_scene_manager_, const char* filename_, const DirectX::XMFLOAT3& target_position_, bool default_collde_is_, bool boss_ = false);
    virtual ~Enemy()                                override = default;

    virtual void init()                             override = 0;
    virtual void update(const float elapsed_time_)  override = 0;

    // Getter関数 //
    _NODISCARD const DirectX::XMFLOAT3& get_target_position() const { return target_position; }

protected:
    void update_velocity(const float elapsed_time_);
    bool check_im_die();                            // ザコ敵の死んだか判定用 (ほんとはダメージ処理用)　   ※※※※※ 注意: ザコ敵のupdate処理の一番前に書き込んであげないと動作しない　※※※※※
    bool check_im_die_only(); // ボスのみ


private:
    const DirectX::XMFLOAT3& target_position;       // このゲームの敵は９割方プレイヤーの位置を使うので、変数をここに入れている。　（使わないもの : チュートリアル用の的）

    std::weak_ptr<class Collide_Detection> wkp_collide_detection;   // ザコ敵用の当たり判定 (この参照が消えた時、ダメージ処理を行う。　ザコ敵に体力はないので即死)
};

