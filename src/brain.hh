#pragma once

#include <entt/entt.hpp>
#include <raylib-cpp.hpp>

class Brain {
protected:
	entt::entity owner;

public:
	virtual void think() = 0;
};

class PlayerBrain : public Brain {
private:
	void move();
	void jump();
	void attack();
	void bite();

public:
	void think();

	PlayerBrain(entt::entity owner);
	virtual ~PlayerBrain();
};

class GuardBrain : public Brain {
private:
	Vector2 find_player();
	float aggro_range = 700.0;
	float attack_range = 400.0;

public:
	void think();

	GuardBrain(entt::entity owner) {
		this->owner = owner;
	}
	virtual ~GuardBrain(){}
};
