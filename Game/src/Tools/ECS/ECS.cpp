#include "ECS.h"

void Entity::step() {
	for (auto& c : components) c->step();
}
void Entity::draw() const {
	for (auto& c : components)	c->draw();
}

fnc::uint Entity::getID() {
	static fnc::uint lastID = 0;
	return lastID++;
}
void Entity::destroy() {
	active = false;
}

bool Entity::operator==(const Entity & other) {
	return this->ID == other.ID;
}

Entity::Entity(Entity && prevEntity, Room * room) : ID(prevEntity.ID), owner(room), persistence(false) {
	if (prevEntity.persistence == true) throw "Error: tried to move a PERSISTENCE entity\n";

	components = std::move(prevEntity.components);
	componentArray = std::move(prevEntity.componentArray);
	componentBitSet = std::move(prevEntity.componentBitSet);
}

Entity::Entity(bool persitence, Room * room) : ID(getID()), owner(room), persistence(persistence) {
	printf("Entity %u created\n", ID);
}
Entity::~Entity() {
	printf("Entity %u\n", ID);
	components.clear();
	printf("component destroyed\n");
}
//ROOM------------------------!

Entity& Room::createEntity() {
	Entity* e = new Entity(false, this);
	std::unique_ptr<Entity> ptr{ e };
	ownEntities.emplace_back(std::move(ptr));
	return *e;
}

void Room::step() {
	for (auto &e : ownEntities) e->step();
}

void Room::draw() {
	for (auto &e : ownEntities) e->draw();
}

void Room::init() {

}

void Room::refresh() {
	this->ownEntities.erase(std::remove_if(std::begin(ownEntities), std::end(ownEntities),
		[](const std::unique_ptr<Entity>& mEntity)
	{
		return !mEntity->active;
	}),
		std::end(ownEntities));
}

Room::Room(IManager& manager, bool persistence) : manager(manager), persistence(persistence) {
}

Room::~Room() {
	ownEntities.clear();
}