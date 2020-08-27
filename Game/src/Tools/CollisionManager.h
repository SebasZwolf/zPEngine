#pragma once
#ifndef __collision_manager__
#define __collision_manager__

#include <vector>
#include "fnc.h"

class CollisionComponent;

class CollisionManager {
private:
	static CollisionManager* sInstance;
	std::vector<CollisionComponent*> colliders;
public:
	static CollisionManager* Instance();
	static void Release();

	void update() {};
	void postupdate() {};

	void add(CollisionComponent*);

private:
	CollisionManager();
	~CollisionManager();
};

#endif // !__collision_manager__

