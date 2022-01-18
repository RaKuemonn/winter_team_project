#pragma once

template<typename Enemy>
class Base_Move
{
public:
    Base_Move() = default;
    virtual ~Base_Move() = default;

public:
    virtual void update(float elapsed_time_, Enemy& type_) = 0;


};