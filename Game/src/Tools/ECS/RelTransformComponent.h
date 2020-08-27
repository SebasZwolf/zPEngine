#pragma once

#ifndef _relative_transform_component_
#define _relative_transform_component_

#include "NodeComponent.h"
#include "TransformComponent.h"

class RelativeTransform : public TransformComponent{
public:
	NodeComponent* node;
	TransformComponent* parent_transform;

	RelativeTransform(float x = 0.0f, float y = 0.0f, float sx = 1.0f, float sy = 1.0f, float angle = 0.0f) : TransformComponent(x,y,sx,sy,angle){
	}
	~RelativeTransform() override {
		printf("\trelative ");
	}

	fig::Vector2D getPos() const override {
		if(parent_transform == nullptr)
			return { this->position };

		fig::Vector2D relPos{ this->position };

		return relPos + parent_transform->getPos();
	}

	fig::Vector2D getScale() const override {
		if (parent_transform == nullptr)
			return { this->scale };

		fig::Vector2D relPos{ this->scale };

		return relPos * parent_transform->getScale();
	}
	
	float getAngle() const override {
		if (parent_transform == nullptr)
			return angle;

		float relPos{ angle };
		
		return relPos + parent_transform->getAngle();
	}

	void init() override {
		node = &entity->getComponent<NodeComponent>();

		node->parentChanged += [this](ParentChangedArg* p) {
			if (p && p->parent->entity->hasComponent<TransformComponent>())
				this->parent_transform = &p->parent->entity->getComponent<TransformComponent>();
			else
				this->parent_transform = nullptr;
		};
	}
};

template<>
inline fnc::uint getComponentTypeID<RelativeTransform>() {
	return getComponentTypeID<TransformComponent>();
}


#endif // !_transform_component
