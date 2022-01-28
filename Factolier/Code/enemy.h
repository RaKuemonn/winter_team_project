#pragma once

#include "entity.h"


class Enemy : public Entity
{
public:
    Enemy(class Scene_Manager* ptr_scene_manager_, const char* filename_, const DirectX::XMFLOAT3& target_position_, bool default_collde_is_, bool boss_ = false);
    virtual ~Enemy()                                override = default;

    virtual void init()                             override = 0;
    virtual void update(const float elapsed_time_)  override = 0;

    // Getter�֐� //
    _NODISCARD const DirectX::XMFLOAT3& get_target_position() const { return target_position; }

protected:
    void update_velocity(const float elapsed_time_);
    bool check_im_die();                            // �U�R�G�̎��񂾂�����p (�ق�Ƃ̓_���[�W�����p)�@   ���������� ����: �U�R�G��update�����̈�ԑO�ɏ�������ł����Ȃ��Ɠ��삵�Ȃ��@����������
    bool check_im_die_only(); // �{�X�̂�


private:
    const DirectX::XMFLOAT3& target_position;       // ���̃Q�[���̓G�͂X�����v���C���[�̈ʒu���g���̂ŁA�ϐ��������ɓ���Ă���B�@�i�g��Ȃ����� : �`���[�g���A���p�̓I�j

    std::weak_ptr<class Collide_Detection> wkp_collide_detection;   // �U�R�G�p�̓����蔻�� (���̎Q�Ƃ����������A�_���[�W�������s���B�@�U�R�G�ɑ̗͂͂Ȃ��̂ő���)
};

