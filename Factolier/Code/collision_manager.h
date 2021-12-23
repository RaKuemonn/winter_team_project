#pragma once


// Stage_ManagerとEntity_Managerのインスタンスの
// 当たり判定処理を受け持つクラス

class Collision_Manager final
{
public:
    Collision_Manager() = default;
    ~Collision_Manager() = default;

    void judge();

};
