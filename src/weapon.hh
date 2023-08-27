#pragma once

#include <entt/entt.hpp>
#include <toml.hpp>

class Weapon {
protected:
	entt::entity owner;

public:
	bool active = false;
	float timer = 0.0;
	Action action = ATTACK;

	virtual void fire() = 0;
	virtual void update() = 0;
	virtual void end() = 0;
};

class Gun : public Weapon {
private:
	int damage;		// Damage of each bullet
	unsigned int count;	// Number of bullets fired at once
	float spread;		// Spread when bullets are fired
	float speed;		// Speed of each bullet
	float rate;		// Time between shots

public:
	Gun(){}
	Gun(entt::entity owner, toml::value data);

	void fire();
	void update();
	void end();
};

class Melee : public Weapon {
private:
	unsigned int damage;
	float range;
	float rate; // Time between attacks
	float push;
	bool can_cancel;

public:
	Melee(){}
	Melee(entt::entity owner, toml::value data);

	void fire();
	void update();
	void end();
};

class Bite : public Weapon {
private:
	int damage;
	float range;
	entt::entity target;
	bool has_target = false; // True if owner is currently grabbing target

public:
	Bite(){}
	Bite(entt::entity owner, toml::value data);

	void fire();
	void update();
	void end();
};
