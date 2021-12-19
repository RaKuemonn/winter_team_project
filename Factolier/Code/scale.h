#pragma once

#include "math_xmfloat.h"



class Scale final
{
public:
    Scale() = default;
    ~Scale() = default;

    // Getterä÷êî //
    _NODISCARD const auto& get() const { return m_scale; }

    // Setterä÷êî //
    void set(const DirectX::XMFLOAT3& scale_)       { m_scale = scale_; }
    void add(const DirectX::XMFLOAT3& add_scale_)   { m_scale += add_scale_; }

private:
    DirectX::XMFLOAT3 m_scale = { 1.0f,1.0f,1.0f };
    
};