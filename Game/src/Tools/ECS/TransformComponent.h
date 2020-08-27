#pragma once

#ifndef _transform_component_
#define _transform_component_

#include "ECS.h"

class TransformComponent : public Component, public bac::Transform {
public:
	TransformComponent(float x = 0.0f, float y = 0.0f, float sx = 1.0f, float sy = 1.0f, float an = 0.0f) {
		position = { x , y };
		scale = { sx, sy };
		angle = an;
	}

	~TransformComponent() override{
		printf("\ttransform ");
	}

	virtual fig::Vector2D getPos() const override{
		return { this->position };
	}
	virtual fig::Vector2D getScale() const override {
		return { this->scale };
	}
	virtual float getAngle() const override {
		return this->angle;
	}

	void init() override {
	}
};



#endif // !_transform_component
