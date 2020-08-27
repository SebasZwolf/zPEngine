#pragma once

//external component dependency
#include "TransformComponent.h"

//tool dependency
#include "../Master.h"
#include "../AssetManager.h"

//external tool dependency
#include "fnc.h"

//classic c++ tool dependency
#include <queue>

#ifndef _sprite_component_
#define _sprite_component_

class SpriteComponent : public Component {
private:
	bac::Transform *transform;
	fncSprite *sprite;

	std::queue<fncSprite*>chain;
	fnc::usmall acumulated;

	const char*name;
public:
	struct {
		//*If set bigger than sprite's length
		//*endAnim will be triggered
		fnc::ushort index = 0;
		//*FOTOGRAMS PER GAME TICK = SPEED / 100
		fnc::usmall speed = 0;
	} image;

	SpriteComponent(const char* key, fnc::usmall image_speed = 0, fnc::ushort image_index = 0) : name(key){
		this->sprite = sngAManager.sprite[key];
		this->image.speed = image_speed;
		this->image.index = image_index;
	}
	~SpriteComponent() override {
		printf("\sprite ");
	}
	
	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			printf("no hay componente!\n");
		}
		transform = &entity->getComponent<TransformComponent>();


	}
	void step() override {
		acumulated += image.speed;

		if (acumulated < 100) return;
		acumulated %= 100;

		image.index++;

		if (image.index == sprite->image_number) {
			if (endAnim != nullptr)
				endAnim(*this);

			image.index %= sprite->image_number;

			if (chain.empty()) return;

			fncSprite* a = chain.front();
			chain.pop();
			if (a != nullptr)
				this->sprite = a;
		}
	}
	void draw() override {
		MasterTool::Instance()->draw.sprite(sprite, image.index, (int)transform->getPos().x, (int)transform->getPos().y);
	}

	SpriteComponent& chain_play(const char* key) {
		if (strcmp(key, "keep") == 0) 
			chain.push(nullptr);
		else
			chain.push(sngAManager.sprite[key]);
		return *this;
	}
	SpriteComponent& play(const char* key, fnc::usmall index = 0) {
		image.index = index;
		this->sprite = sngAManager.sprite[key];
		return *this;
	}

	fig::cRect getArea() {
		fig::Vector2D a{ transform->getScale() };
		a *= fig::Vector2D{ sprite->size };

		return { transform->getPos(), a};
	}

	void(*endAnim)(SpriteComponent&);
};

#endif // !_sprite_component_