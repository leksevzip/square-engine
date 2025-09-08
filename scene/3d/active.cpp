#include "active.h"

void Active::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_active", "active"), &Active::set_active);
	ClassDB::bind_method(D_METHOD("is_active"), &Active::is_active);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "active"), "set_active", "is_active");
}

void Active::_set_active_recursive(Node *node, bool state) {
	if (!node) {
		return;
	}

	if (node->is_class("Node3D")) {
		Node3D *n3d = Object::cast_to<Node3D>(node);
		if (n3d) {
			n3d->set_visible(state);
			n3d->set_process(state);
			n3d->set_physics_process(state);
		}
	}

	for (int i = 0; i < node->get_child_count(); i++) {
		_set_active_recursive(node->get_child(i), state);
	}
}

void Active::set_active(bool p_active) {
	active = p_active;
	_set_active_recursive(this, active);
}

bool Active::is_active() const {
	return active;
}

Active::Active() {
	active = true;
}
