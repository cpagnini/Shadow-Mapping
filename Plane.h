#pragma once
#include <vector>
#include "Mesh.h"

class Plane : public Mesh {
public:
    Plane(float width = 10.0f, float depth = 10.0f);
};
