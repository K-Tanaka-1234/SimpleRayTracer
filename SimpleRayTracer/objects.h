#pragma once

#include <stdio.h>
#include "ray.h"
#include "vector.h"
#include "vector_op.h"
#include "material.h"
#include "BVH.h"

typedef enum OBJECT_TYPE {
	PARAMETRIC = 0
} object_type;

typedef struct HIT_RECORD {
	point3 p;
	vec3 normal;
	double t;
	double u;
	double v;
	struct MATERIAL *mat;
	int front_face;
}hit_record;

typedef struct OBJECT_DATA {
	object_type type;
	double data[4];
	point3 center;
	struct MATERIAL *mat;
}object_data;

typedef struct OBJECT {
	object_data obj_data;
	int(*intersection_test)(ray r, double t_min, double t_max, hit_record *rec, object_data *data);
	int(*get_bounding_box)(double t0, double t1, struct AABB *bb, object_data *data);
}object;

typedef struct OBJ_NODE {
	object obj;
	struct OBJ_NODE *next;
}obj_node;

typedef struct OBJ_LIST {
	obj_node *root;
}obj_list;

object create_object(object_data obj_data, int(*intersection_test)(ray r, double t_min, double t_max, hit_record *rec, object_data *data), int(*get_bounding_box)(double t0, double t1, struct AABB *bb, object_data *data));
void init_object_list(obj_list *list);
int object_list_append(obj_list *list, object obj, struct MATERIAL mat);
int clear_object_list(obj_list *list);
int objects_intersection_test(ray r, double t_min, double t_max, obj_list *list, hit_record *rec);

inline void set_face_normal(ray r, vec3 outward_normal, hit_record *rec) 
{
	rec->front_face = dotv(r.dir, outward_normal) < 0;
	rec->normal = (rec->front_face) ? outward_normal : mulv(-1, outward_normal);

	return;
}
