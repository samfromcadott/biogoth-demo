#pragma once

#include <entt/entt.hpp>
#include <toml.hpp>

// The default range for Magic Enum is [-128, 128]
#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 1024
#include <magic_enum.hpp>

#include "typedefs.hh"

class Weapon {
protected:
	entt::entity owner;

public:
	bool active = false;
	float timer = 0.0;
	Action action = ATTACK_A;

	virtual void fire() = 0;
	virtual void update() = 0;
	virtual void end() = 0;
};

class Gun : public Weapon {
private:
	int damage;		// Damage of each bullet
	vec2 offset;		// Loction bullets spawn
	unsigned int count;	// Number of bullets fired at once
	float spread;		// Spread when bullets are fired
	float speed;		// Speed of each bullet
	float rate;		// Time between shots

public:
	Gun() = default;
	Gun(entt::entity owner, toml::value data);

	void fire();
	void update();
	void end();
};

class Melee : public Weapon {
private:
	unsigned int damage;
	vec2 offset;
	float height, width;
	float rate; // Time between attacks
	float push;
	bool can_cancel;

public:
	Melee() = default;
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
	Bite() = default;
	Bite(entt::entity owner, toml::value data);

	void fire();
	void update();
	void end();
};

class Shield : public Weapon {
private:
	int damage;
	float width;
	float rate;
	float length; // Amount of time the shield is active
	bool deflect; // If true incoming bullets are deflected

public:
	Shield() = default;
	Shield(entt::entity owner, toml::value data);

	void fire();
	void update();
	void end();
};

class Charge : public Weapon {
private:
	int min_damage, max_damage;
	float max_speed;
	vec2 offset;
	float height, width;
	float push;
	vec2 direction;

	bool done = false;
	bool can_fire = true;

	bool stopped(); // Checks if the owner has touched the floor and stopped

public:
	Charge() = default;
	Charge(entt::entity owner, toml::value data);

	void fire();
	void update();
	void end();
};
