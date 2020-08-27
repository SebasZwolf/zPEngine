#include "CollisionManager.h"

CollisionManager* CollisionManager::sInstance = nullptr;

CollisionManager * CollisionManager::Instance(){
	if (sInstance == nullptr) sInstance = new CollisionManager();
	return sInstance;
}

void CollisionManager::Release(){
	if (sInstance == nullptr) return;
	delete sInstance;
	sInstance = nullptr;
}

CollisionManager::CollisionManager(){
#ifdef _IOSTREAM_
	std::cout << "CollisionManager initiailzated\n";
#endif // _IOSTREAM_
}

CollisionManager::~CollisionManager(){
#ifdef _IOSTREAM_
	std::cout << "CollisionManager destroyed\n";
#endif // _IOSTREAM_
}

