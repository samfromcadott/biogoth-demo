#pragma once

#include <entt/entt.hpp>

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
public:
	void think();

	GuardBrain(entt::entity owner);
	virtual ~GuardBrain();
};
