#ifndef ACTIVE_H
#define ACTIVE_H

#include "scene/3d/node_3d.h"

class Active : public Node3D {
	GDCLASS(Active, Node3D);

private:
	bool active = true;

protected:
	static void _bind_methods();

public:
	void set_active(bool p_active);
	bool is_active() const;

	Active();

private:
	void _set_active_recursive(Node *node, bool state);
};

#endif // ACTIVE_NODE_H
