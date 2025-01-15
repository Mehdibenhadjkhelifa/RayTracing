#include "Renderer.h"
#include "Walnut/Random.h"
#include "glm/geometric.hpp"
#include <cstddef>
#include <limits>
#include <string.h>
#include <algorithm>
#include <execution>
#include <numeric>

namespace Utils {
	static uint32_t ConvertToRGBA(const glm::vec4& light) {
		uint8_t r =(uint8_t)(light.r * 255.0f);
		uint8_t g = (uint8_t)(light.g * 255.0f);
		uint8_t b = (uint8_t)(light.b * 255.0f);
		uint8_t a = (uint8_t)(light.a * 255.0f);
		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
    static uint32_t PCG_Hash(uint32_t input) {
        uint32_t state = input * 747796405u + 2891336453u;
        uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
        return (word >> 22u) ^ word;
    }
    static float RandomFloat(uint32_t& seed) {
        seed = PCG_Hash(seed);
        return static_cast<float>(seed) / static_cast<float>(std::numeric_limits<uint32_t>::max());
    }
    static glm::vec3 InUnitSphere(uint32_t& seed) {
        return glm::normalize(glm::vec3(RandomFloat(seed) * 2.0f - 1.0f, RandomFloat(seed) * 2.0f - 1.0f, RandomFloat(seed) * 2.0f - 1.0f));
    }
}
void Renderer::OnResize(uint32_t width, uint32_t height) {
	if (m_FinalImage) {
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_FinalImage->Resize(width, height);
	}
	else {
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}
	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
    delete[] m_AccumulationData;
    m_AccumulationData = new glm::vec4[width * height];
    m_ImageHorizontalIter.resize(width);
    m_ImageVerticalIter.resize(height);
    for(uint32_t i = 0; i < width; i++)
        m_ImageHorizontalIter[i] = i;
    for(uint32_t i = 0; i < height; i++)
        m_ImageVerticalIter[i] = i;
}

void Renderer::Render(const Scene& scene,const Camera& camera) {
    m_ActiveScene = &scene;
    m_ActiveCamera = &camera;
    if(m_FrameIndex == 1)
        memset(m_AccumulationData, 0, m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * sizeof(glm::vec4));
#define MT 1 
#if MT
    std::for_each(std::execution::par,m_ImageVerticalIter.begin(),m_ImageVerticalIter.end(),
            [this](uint32_t y){
                std::for_each(std::execution::par,m_ImageHorizontalIter.begin(),m_ImageHorizontalIter.end(),
                [this,y](uint32_t x){
                glm::vec4 light = PerPixel(x,y);
                m_AccumulationData[x + y * m_FinalImage->GetWidth()] += light; // we have to make an average out of this so that the image isn't so bright from accumulating
                glm::vec4 accumulatedColor = m_AccumulationData[x + y * m_FinalImage->GetWidth()];
                accumulatedColor /= (float)m_FrameIndex;
                accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
                m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(accumulatedColor);
                }
                );
            }
            );
            
#else
	for (uint32_t y = 0; y < m_FinalImage->GetHeight();y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
			glm::vec4 light = PerPixel(x,y);
            m_AccumulationData[x + y * m_FinalImage->GetWidth()] += light; // we have to make an average out of this so that the image isn't so bright from accumulating
            glm::vec4 accumulatedColor = m_AccumulationData[x + y * m_FinalImage->GetWidth()];
            accumulatedColor /= (float)m_FrameIndex;
            accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(accumulatedColor);
		}
	}
#endif
	m_FinalImage->SetData(m_ImageData);
    if (m_Settings.Accumulate)
        m_FrameIndex++;
    else
        m_FrameIndex = 1;
}

Renderer::HitPayload Renderer::TraceRay(const Ray& ray)
{
    int closestSphere = -1;
    float hitDistance = std::numeric_limits<float>::max();
    for(size_t i = 0; i < m_ActiveScene->Spheres.size();i++){
        const Sphere& sphere = m_ActiveScene->Spheres[i];

        float a = glm::dot(ray.Direction, ray.Direction);
        float b = 2.0f * (glm::dot(ray.Origin, ray.Direction) - glm::dot(sphere.Position, ray.Direction));
        float c = glm::dot(ray.Origin,ray.Origin) + (-2.0f * (glm::dot(sphere.Position, ray.Origin))) + glm::dot(sphere.Position, sphere.Position) - sphere.radius * sphere.radius;

        float discremenant = b * b - 4.0f * a *  c;
        if (discremenant < 0.0f)
            continue;

        float closest_t = (-b - glm::sqrt(discremenant)) / (2.0f * a);
        if(closest_t > 0.0f && closest_t < hitDistance){
            hitDistance = closest_t;
            closestSphere = static_cast<int>(i);
        }
    }
    if (closestSphere < 0)
        return Miss(ray); 
    return ClosestHit(ray,hitDistance,closestSphere);
}
glm::vec4 Renderer::PerPixel(uint32_t x,uint32_t y){

    Ray ray;
    ray.Origin = m_ActiveCamera->GetPosition();
    ray.Direction = m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

    glm::vec3 light(0.0f);
    int bounces = 5;
    glm::vec3 throughput(1.0f);
    uint32_t seed = x + y * m_FinalImage->GetWidth();
    seed *= m_FrameIndex;
    for(int i = 0; i < bounces; i++){
        seed += i;
        Renderer::HitPayload payload = TraceRay(ray);
        if(payload.HitDistance < 0.0f){
            glm::vec3 skyColor = glm::vec3(0.6f,0.7f,0.9f);
            //light += skyColor * throughput;
            break;
        }


        const Sphere& sphere = m_ActiveScene->Spheres[static_cast<size_t>(payload.ObjectIndex)];
        const Material& material = m_ActiveScene->Materials[static_cast<size_t>(sphere.MaterialIndex)];

        throughput *= material.Albedo;
        light += material.GetEmission() ;
        ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
        //ray.Direction = glm::reflect(ray.Direction,
         //       payload.WorldNormal + material.Roughness * Walnut::Random::Vec3(-0.5f,0.5f));
        //ray.Direction = glm::normalize(payload.WorldNormal + Walnut::Random::InUnitSphere());
        if(m_Settings.SlowRandom)
            ray.Direction = glm::normalize(payload.WorldNormal + Walnut::Random::InUnitSphere());
        else
            ray.Direction = glm::normalize(payload.WorldNormal + Utils::InUnitSphere(seed));


    }

    return glm::vec4(light,1.0f);
}
Renderer::HitPayload Renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex){
    Renderer::HitPayload payload;
    const Sphere& closestSphere = m_ActiveScene->Spheres[static_cast<size_t>(objectIndex)];
    payload.HitDistance = hitDistance;
    payload.ObjectIndex = objectIndex;
	payload.WorldPosition = ray.Origin + ray.Direction * hitDistance;
	payload.WorldNormal = glm::normalize(payload.WorldPosition - closestSphere.Position);
    return payload;
}
Renderer::HitPayload Renderer::Miss(const Ray& ray){
    Renderer::HitPayload payload;
    payload.HitDistance = -1.0f;
    return payload;
}
