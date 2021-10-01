#include "BVH.h"
#include "ray.h"
#include <math.h>
#include "objects.h"
#include "random.h"

aabb aabb_create(point3 a, point3 b)
{
	aabb tmp = { a, b };
	return tmp;
}

int aabb_intersection_test(aabb *bb, ray r, double t_min, double t_max)
{
	for (int i = 0; i < 3; i++)
	{
		double invD = 1.0f / r.dir.e[i];
		double t0 = (bb->min.e[i] - r.orig.e[i]) * invD;
		double t1 = (bb->max.e[i] - r.orig.e[i]) * invD;
	
		if (invD < 0.0f)
		{
			double tmp = t0;
			t0 = t1;
			t1 = tmp;
		}

		t_min = (t0 > t_min) ? t0 : t_min;
		t_max = (t1 < t_max) ? t1 : t_max;

		if (t_max <= t_min) return 0;
	}

	return 1;
}

aabb aabb_surrounding_box(aabb box0, aabb box1)
{
	point3 small = initv3(
		fmin(box0.min.e[0], box1.min.e[0]),
		fmin(box0.min.e[1], box1.min.e[1]),
		fmin(box0.min.e[2], box1.min.e[2])
	);

	point3 big = initv3(
		fmax(box0.max.e[0], box1.max.e[0]),
		fmax(box0.max.e[1], box1.max.e[1]),
		fmax(box0.max.e[2], box1.max.e[2])
	);

	return aabb_create(small, big);
}

int bvh_intersection_test(bvh_node * node, ray r, double t_min, double t_max, struct HIT_RECORD * rec)
{
	if (!aabb_intersection_test(&node->bb, r, t_min, t_max))
		return 0;

	int hit_left = bvh_intersection_test(node->left, r, t_min, t_max, rec);
	int hit_right = bvh_intersection_test(node->right, r, t_min, (hit_left) ? rec->t : t_max, rec);

	return hit_left || hit_right;
}

