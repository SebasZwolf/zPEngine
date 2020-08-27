#include "IManage.h"
#include <cstdio>

//MANAGER------------------------!

Manager* Manager::sInstance = NULL;

Manager* Manager::Instance() {
	if (sInstance == NULL) sInstance = new Manager();
	return sInstance;
};

void Manager::Release() {
	if (sInstance == NULL) return;

	delete sInstance;
	sInstance = NULL;
}

void Manager::born(){
}

void Manager::step(){
	refresh();

	current_room->step();
	for (auto& a : perEntities)
		a->step();
}
void Manager::draw(){
	current_room->draw();
	for (auto& a : perEntities)
		a->draw();
}
void Manager::refresh() {
	current_room->refresh();
	this->perEntities.erase(std::remove_if(std::begin(perEntities), std::end(perEntities),
		[](const std::unique_ptr<Entity>& mEntity)
		{
			return !mEntity->active;
		}),
		std::end(perEntities));
}

Room& Manager::newRoom() {
	return *(current_room = new Room(*this, false));
}

Entity & Manager::createEntity(bool persistence){
	if (current_room == nullptr) throw "no room was created yet";

	Entity* e = new Entity(persistence, persistence ? nullptr : current_room);
	std::unique_ptr<Entity> ptr{ e };
	perEntities.emplace_back(std::move(ptr));
	return *e;
}

Entity & Manager::addEntity(Entity * entity){
	if (current_room == nullptr) throw "no room was created yet";

	std::unique_ptr<Entity> ptr{ entity };
	perEntities.emplace_back(std::move(ptr));
	return *entity;
}

Manager::Manager() {
	printf("Instance Manager %s\n", "Initialized");

	newRoom();
}
Manager::~Manager() {
	perEntities.clear();
	for (auto& c : roomArray)
		if(c) delete c;

	printf("Instance Manager %s\n", "Destroyed");
}