#pragma once

#include "vector.h"
#include "vector_op.h"
#include "ray.h"
#include "objects.h"

typedef struct AABB {
	point3 min;
	point3 max;
}aabb;

typedef struct BVH_NODE {
	aabb bb;
	struct BVH_NODE *left;
	struct BVH_NODE *right;
}bvh_node;

typedef struct BVH_TREE {
	bvh_node *root;
}bvh_tree;

aabb aabb_create(point3 a, point3 b);
int aabb_intersection_test(aabb *bb, ray r, double t_min, double t_max);
aabb aabb_surrounding_box(aabb box0, aabb box1);
int bvh_intersection_test(bvh_node *node, ray r, double t_min, double t_max, struct HIT_RECORD *rec);