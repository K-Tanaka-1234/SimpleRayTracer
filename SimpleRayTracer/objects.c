#include "simple_ray_tracer.h"
#include "objects.h"
#include "ray.h"
#include <stdlib.h>

object create_object(object_data obj_data, int(*intersection_test)(ray r, double t_min, double t_max, hit_record *rec, object_data *data), int(*get_bounding_box)(double t0, double t1, struct AABB *bb, object_data *data))
{
	object obj;
	obj.obj_data = obj_data;
	obj.intersection_test = intersection_test;
	obj.get_bounding_box = get_bounding_box;
	return obj;
}

void init_object_list(obj_list *list)
{
	list->root = NULL;

	return;
}

int object_list_append(obj_list *list, object obj, struct MATERIAL mat)
{
	obj_node *tmp = (obj_node *)malloc(sizeof(obj_node));
	tmp->obj = obj;
	tmp->obj.obj_data.mat = (material *)malloc(sizeof(material));
	*tmp->obj.obj_data.mat = mat;
	tmp->next = NULL;

	if (list->root == NULL)
	{
		list->root = tmp;
		return 0;
	}

	obj_node *p = list->root;
	while (p->next != NULL)
	{
		p = p->next;
	}
	p->next = tmp;

	return 0;
}

int clear_object_list(obj_list *list)
{
	obj_node *p = list->root;

	while (p != NULL) 
	{
		obj_node *q = p->next;
		free(p->obj.obj_data.mat);
		free(p);
		p = q;
	}

	list->root = NULL;

	return 0;
}

int objects_intersection_test(ray r, double t_min, double t_max, obj_list *list, hit_record *rec)
{
	hit_record tmp_rec;
	int hit = 0;
	double curt_t_max = t_max;

	obj_node *p = list->root;
	while (p != NULL)
	{
		if (p->obj.intersection_test(r, t_min, curt_t_max, &tmp_rec, &p->obj.obj_data))
		{
			hit = 1;
			curt_t_max = tmp_rec.t;
			*rec = tmp_rec;
		}
		p = p->next;
	}

	return hit;
}

int objects_calc_aabb(double t0, double t1, aabb *bb, obj_list *list)
{
	aabb tmp_bb;
	int first_bb = 1;

	obj_node *p = list->root;
	while (p != NULL)
	{
		if (!p->obj.get_bounding_box(t0, t1, &tmp_bb, &p->obj.obj_data)) return 0;
		*bb = (first_bb) ? tmp_bb : aabb_surrounding_box(*bb, tmp_bb);
		first_bb = 0;
		p = p->next;
	}
	
	return 1;
}