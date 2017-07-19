#pragma once
#include "Util.h"


class Camera {
public:
	Vector3 position, direction, up;
	float aperature;
	Camera() {
		position = Vector3(0, 0, -100);
		direction = Vector3(0, 0, 1);
		up = Vector3(0, 1, 0);
		aperature = 10;
	}

	Camera(Vector3 pos, Vector3 dir, Vector3 u, float a) { position = pos; direction = dir; up = u; aperature = a; }
};

