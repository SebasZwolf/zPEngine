#pragma once

#ifndef __ECS__
#define __ECS__

#include <array>
#include <vector>
#include <bitset>
#include <memory>
#include <type_traits>

#include "fnc.h"

class Room;

class IManager abstract {
public:
	//Called the FIRSt frame after initialization just ONCE
	virtual void born() = 0;
	//Called each frame after game has started
	virtual void step() = 0;
	//Called once step phase has ended
	virtual void draw() = 0;

	//Called 
	virtual void refresh() {};
};

class Component;
class Entity;

inline fnc::uint getNewComponentTypeID() {
	static fnc::uint lastID = 0;
	return lastID++;
}

template <typename T>
inline fnc::uint getComponentTypeID() {
	static fnc::uint typeID = getNewComponentTypeID();
	return typeID;
}

constexpr fnc::uint maxComponents = 32;
constexpr fnc::uint maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;

class Component {
private:
public:
	Entity* entity;
	virtual void init() = 0;
	virtual void step() {};
	virtual void draw() {};

	virtual ~Component() { printf("destroyed\n"); };
};

class Entity {
private:
	bool _active = true;
	const bool persistence;
	std::vector<std::unique_ptr<Component>> components;

	Room* owner;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;

	fnc::uint getID();
public:
	const fnc::uint ID;
	bool &active = _active;

	void step();
	void draw() const;

	void destroy();

	template<typename T>
	bool hasComponent() const {
		return componentBitSet[getComponentTypeID<T>()];
	}
	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template<typename T> T& getComponent() const {
		Component* ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

	bool operator==(const Entity& other);

	Entity(const Entity&) = delete;

	//SUGGESTION: AVOID USING THIS CONTRUCTOR
	Entity(Entity&&, Room* = nullptr);

	Entity(bool, Room* = nullptr);
	~Entity();
};

class Room {
private:
	std::vector<std::unique_ptr<Entity>> ownEntities;
	IManager& manager;
protected:
	Entity& createEntity();
public:
	const bool persistence;

	virtual void step();
	virtual void draw();

	void refresh();

	virtual void init();
protected:
	Room(IManager& manager, bool persistence);
	~Room();

	friend class Manager;
	//friend Room& IManager::createRoom();
};

#endif // !__ECS__
