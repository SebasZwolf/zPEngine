#pragma once

#ifndef __default_manager__
#define __default_manager__

#include "ECS/ECS.h"
#include <map>

inline fnc::usmall getNewRoomID() {
	static fnc::usmall lastID = 0;
	return lastID++;
}

template<typename room>
inline fnc::usmall getRoomID() {
	static fnc::usmall ID = getNewRoomID();
	return ID;
}

class Manager : public IManager {
private:
	//std::vector<std::unique_ptr<Room>> perRooms;
	std::vector<std::unique_ptr<Entity>> perEntities;

	std::array<Room*, 16> roomArray;
	fnc::usmall perRooms = 0;

	std::map<fnc::usmall, Room*> perRoom;

	static Manager* sInstance;
public:
	static Manager* Instance();
	static void Release();

	Room* current_room;

	void born() override;
	void step() override;
	void draw() override;

	void refresh() override;

	Entity& createEntity(bool = true);
	Entity& addEntity(Entity* entity);

	Room& newRoom();

	template<typename t> fnc::usmall	reserveRoom() {
		perRooms++;
		return getRoomID<t>();
	}
	template<typename t> Room&			addRoom() {
		perRooms++;
		Room* r = new t(*this, true);
		r->init();

		roomArray[getRoomID<t>()] = r;
		return r;
	}

	template<typename t> Room&			refresRoom() {
		if (roomArray[getRoomID<t>()] == nullptr) return;
		delete roomArray[getRoomID<t>()];
		roomArray[getRoomID<t>()] = new t(*this, true);
		roomArray[getRoomID<t>()]->init();
	}
	template<typename T> Room&			gotoRoom(){
		if (!current_room->persistence)
			delete current_room;
	
		if (getRoomID<T>() >= perRooms) {
			current_room = new T(*this, false);
			current_room->init();
		}
		else {
			if (roomArray[getRoomID<T>()] == nullptr) {
				current_room = new T(*this, false);
				roomArray[getRoomID<T>()] = current_room;
				current_room->init();
			}
			else
				current_room = roomArray[getRoomID<T>()];
		}

		return *current_room;
	}

private:
	Manager();
	~Manager();
};

#define sngIManager (*Manager::Instance())

#endif // !__default_manager__
