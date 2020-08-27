#pragma once

#include "Components.h"

#include "fnc.h"

#include "InputManager.h"

class Entity;
struct CollisionInfo;

class CollisionComponent : public Component, bac::Collider {
private:
	bool _solid;
public:
	bool& solid = _solid;

	CollisionComponent() {
	}
	~CollisionComponent() override {
		for (fnc::usmall i = 0; i < nfigures; i++)
			delete figures[i].second;
		printf("\t<With %u figures in object %u>collision", nfigures, entity->ID);
		delete[] figures;
	}

	void init() override {		
		if (entity->hasComponent<TransformComponent>())
			transform = &entity->getComponent<TransformComponent>();
		else
			transform = &entity->addComponent<TransformComponent>();
	}

	CollisionComponent & operator<<( fig::cRect *v2){
		addFigure(space::Complexity::cRect, v2);
		return *this;
	}
	CollisionComponent & operator<<( fig::cSphere *v2) {
		addFigure( space::Complexity::cSphere, v2);
		return *this;
	}
	
	void addFigure(space::Complexity c, fig::Figure* fig) {
		auto temp = new std::pair < space::Complexity, fig::Figure* >[nfigures + 1];

		for (fnc::usmall i = 0; i < nfigures; i++) {
			temp[i].first = figures[i].first;
			temp[i].second = figures[i].second;
		}

		temp[nfigures].first = c;
		temp[nfigures].second = fig;

		nfigures++;

		delete[] figures;
		figures = temp;
	}

	virtual std::pair<space::Complexity, fig::Figure*>* getFigures() const override {
		return this->figures;
	}

	void step() override {
	}
	void draw() override {

		for (fnc::usmall i = 0; i < nfigures; i++) {
			switch (figures[i].first)
			{
			case space::Complexity::cRect:
			{
				fig::cRect a{
					*(fig::cRect*)figures[i].second
				};
				a.pos += this->transform->getPos();

				drawer.line.rect( a );
			}
				break;
			case space::Complexity::cSphere:
			{
				fig::cSphere a{
					 *(fig::cSphere*)figures[i].second
				};
				a.pos += this->transform->getPos();

				drawer.line.circle(a);
			}
				break;
			}
		}
	}
};
