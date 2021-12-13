#pragma once

#include "math_xmfloat.h"



class Position
{
public:
    Position() = default;
    ~Position() = default;

    // Getterä÷êî //
    const auto& get() const { return m_position; }

    // Setterä÷êî //
    void set(const DirectX::XMFLOAT3& position_)        { m_position = position_; }
    void add(const DirectX::XMFLOAT3& add_position_)    { m_position += add_position_; }

private:
    DirectX::XMFLOAT3 m_position = { };

private:

};
