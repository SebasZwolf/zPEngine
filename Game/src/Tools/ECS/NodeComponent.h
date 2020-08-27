#pragma once

#include "Components.h"
#include <vector>

#include "fnc.h"

#ifndef _node_component_
#define _node_component_

class NodeComponent;

struct ParentChangedArg : del::EvArgs {
	NodeComponent* const parent;

	ParentChangedArg(NodeComponent* parent) : del::EvArgs(), parent(parent) {}
};

class NodeComponent : public Component {
private:
	NodeComponent* _parent = nullptr;
	std::vector<NodeComponent*> _children;

	del::securedEvent<ParentChangedArg> _parentChanged;
public:
	del::event<ParentChangedArg> &parentChanged = _parentChanged;

	void append_child(NodeComponent* child) {
		this->_children.emplace_back(child);

		if (child->_parent != nullptr)
			child->_parent->remove_child(child);

		child->_parent = this;
		child->_parentChanged(new ParentChangedArg(this));
	}
	void remove_child(NodeComponent* child) {
		if (child->_parent->entity->ID != this->entity->ID)
			return;

		_children.erase(std::remove_if(std::begin(_children), std::end(_children), [&child](NodeComponent* node) {
			return node->entity->ID == child->entity->ID;
		}), std::end(_children));

		child->_parent = nullptr;
		child->_parentChanged(new ParentChangedArg(nullptr));
	}

	NodeComponent* const& parent = _parent;
	const std::vector<NodeComponent*>& children = _children;

	void init() {

	}
	NodeComponent() {
	}
	~NodeComponent() {
		if (_parent != nullptr)
			this->_parent->remove_child(this);

		for (auto& node : _children)
			node->_parent = nullptr;
		printf("\node ");
	}
};

#endif // ! _node_component_
