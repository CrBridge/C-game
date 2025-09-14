#include "vector.h"

Vector3f vector3f_add_vector3f(Vector3f a, Vector3f b) {
	Vector3f v;
	v.x = a.x + b.x;
	v.y = a.y + b.y;
	v.z = a.z + b.z;

	return v;
}

Vector3f vector3f_add_component(Vector3f a, f32 b) {
	Vector3f v;
	v.x = a.x + b;
	v.y = a.y + b;
	v.z = a.z + b;

	return v;
}

Vector3f vector3f_mul_vector3f(Vector3f a, Vector3f b) {
	Vector3f v;
	v.x = a.x * b.x;
	v.y = a.y * b.y;
	v.z = a.z * b.z;

	return v;
}

Vector3f vector3f_mul_component(Vector3f a, f32 b) {
	Vector3f v;
	v.x = a.x * b;
	v.y = a.y * b;
	v.z = a.z * b;

	return v;
}

Vector3f vector3f_div_vector3f(Vector3f a, Vector3f b) {
	Vector3f v;
	v.x = a.x / b.x;
	v.y = a.y / b.y;
	v.z = a.z / b.z;

	return v;
}

Vector3f vector3f_div_component(Vector3f a, f32 b) {
	Vector3f v;
	v.x = a.x / b;
	v.y = a.y / b;
	v.z = a.z / b;

	return v;
}

Vector2f vector2f_div_component(Vector2f a, f32 b) {
	Vector2f v;
	v.x = a.x / b;
	v.y = a.y / b;

	return v;
}
