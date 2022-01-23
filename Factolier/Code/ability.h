#pragma once

#include "math_xmfloat.h"



class Ability final
{
public:
    Ability() = default;
    ~Ability() = default;

    // Getterä÷êî
    _NODISCARD const short& get_hp() { return m_hp; }
    _NODISCARD short* get_ptr_hp() { return &m_hp; }

    // Setterä÷êî
    void set_init(const short& hp_)
    {
        m_hp = hp_;
    }
    void add_hp(const short& hp_) { m_hp += hp_; }

private:
    short m_hp = 0;
};

