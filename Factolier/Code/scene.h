#pragma once

#include "state_manager.h"


class Scene_Manager;


class Scene
{
public:
	virtual ~Scene() {};

	virtual void initialize() = 0;
	virtual void uninitialize() = 0;
	virtual void update(float elapsedTime) = 0;
	virtual void render(float elapsedTime) = 0;

protected:
	Scene_Manager* parent = nullptr;
	bool ready = false;

public:
	void set_parent(Scene_Manager* sm) { parent = sm; }

	bool is_ready() const { return ready; }
	void set_ready() { ready = true; }
};
