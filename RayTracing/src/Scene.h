#pragma once

#include <glm/glm.hpp>

#include <vector>

struct Material{
    glm::vec3 Albedo { 1.0f };
    float Roughness = 1.f;
    float Metalic = 0.0f;

};
struct Sphere{
    glm::vec3 Position{0.0f};
    float radius = 0.5f;
    Material Mat;
};

struct Scene{
    std::vector<Sphere> Spheres;
};
