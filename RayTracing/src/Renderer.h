#pragma once

#include "Walnut/Image.h"
#include "Camera.h"
#include "Ray.h"
#include <memory>
#include "Scene.h"
#include <glm/glm.hpp>
class Renderer {
public:
	Renderer() = default;
	void OnResize(uint32_t width, uint32_t height);
	void Render(const Scene& scene,const Camera& camera);
	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }
private:
    struct HitPayload{
        float HitDistance;
        glm::vec3 WorldPosition;
        glm::vec3 WorldNormal;
        int ObjectIndex;
    };
    glm::vec4 PerPixel(uint32_t y, uint32_t x); // RayGen shader
	HitPayload TraceRay(const Ray& ray);
	HitPayload ClosestHit(const Ray& ray,float HitDistance,int objectIndex);
	HitPayload Miss(const Ray& ray);
    
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
    const Scene* m_ActiveScene = nullptr;
    const Camera* m_ActiveCamera = nullptr;
	uint32_t* m_ImageData = nullptr;
};
