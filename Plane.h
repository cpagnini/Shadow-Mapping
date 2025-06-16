#pragma once
#include <vector>
#include "Mesh.h"

class Plane : public Mesh {
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="width"></param>
    /// <param name="depth"></param>
    Plane(float width = 10.0f, float depth = 10.0f);
};
