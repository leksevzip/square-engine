/**************************************************************************/
/*  test_SEO.h                                                            */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "../SEO.h"
#include "../SEO_shape.h"

#include "tests/test_macros.h"

namespace TestSEO {

TEST_CASE("[SceneTree][SEO] SEOPolygon3D") {
	SUBCASE("[SceneTree][SEO] SEOPolygon3D: using accurate path tangent for polygon rotation") {
		const float polygon_radius = 10.0f;

		const Vector3 expected_min_bounds = Vector3(-polygon_radius, -polygon_radius, 0);
		const Vector3 expected_max_bounds = Vector3(100 + polygon_radius, polygon_radius, 100);
		const AABB expected_aabb = AABB(expected_min_bounds, expected_max_bounds - expected_min_bounds);

		Ref<Curve3D> curve;
		curve.instantiate();
		curve->add_point(
				// p_position
				Vector3(0, 0, 0),
				// p_in
				Vector3(),
				// p_out
				Vector3(0, 0, 60));
		curve->add_point(
				// p_position
				Vector3(100, 0, 100),
				// p_in
				Vector3(0, 0, -60),
				// p_out
				Vector3());

		Path3D *path = memnew(Path3D);
		path->set_curve(curve);

		SEOPolygon3D *SEO_polygon_3d = memnew(SEOPolygon3D);
		SceneTree::get_singleton()->get_root()->add_child(SEO_polygon_3d);

		SEO_polygon_3d->add_child(path);
		SEO_polygon_3d->set_path_node(SEO_polygon_3d->get_path_to(path));
		SEO_polygon_3d->set_mode(SEOPolygon3D::Mode::MODE_PATH);

		PackedVector2Array polygon;
		polygon.append(Vector2(-polygon_radius, 0));
		polygon.append(Vector2(0, polygon_radius));
		polygon.append(Vector2(polygon_radius, 0));
		polygon.append(Vector2(0, -polygon_radius));
		SEO_polygon_3d->set_polygon(polygon);

		SEO_polygon_3d->set_path_rotation(SEOPolygon3D::PathRotation::PATH_ROTATION_PATH);
		SEO_polygon_3d->set_path_rotation_accurate(true);

		// Minimize the number of extrusions.
		// This decreases the number of samples taken from the curve.
		// Having fewer samples increases the inaccuracy of the line between samples as an approximation of the tangent of the curve.
		// With correct polygon orientation, the bounding box for the given curve should be independent of the number of extrusions.
		SEO_polygon_3d->set_path_interval_type(SEOPolygon3D::PathIntervalType::PATH_INTERVAL_DISTANCE);
		SEO_polygon_3d->set_path_interval(1000.0f);

		// Call get_brush_faces to force the bounding box to update.
		SEO_polygon_3d->get_brush_faces();

		CHECK(SEO_polygon_3d->get_aabb().is_equal_approx(expected_aabb));

		// Perform the bounding box check again with a greater number of extrusions.
		SEO_polygon_3d->set_path_interval(1.0f);
		SEO_polygon_3d->get_brush_faces();

		CHECK(SEO_polygon_3d->get_aabb().is_equal_approx(expected_aabb));

		SEO_polygon_3d->remove_child(path);
		SceneTree::get_singleton()->get_root()->remove_child(SEO_polygon_3d);

		memdelete(SEO_polygon_3d);
		memdelete(path);
	}
}

} // namespace TestSEO
