#include "Renderer.h"
#include "Walnut/Random.h"
#include <limits>

namespace Utils {
	static uint32_t ConvertToRGBA(const glm::vec4& color) {
		uint8_t r =(uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);
		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
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

}

void Renderer::Render(const Scene& scene,const Camera& camera) {
    m_ActiveScene = &scene;
    m_ActiveCamera = &camera;

            
	for (uint32_t y = 0; y < m_FinalImage->GetHeight();y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
			glm::vec4 color = PerPixel(x,y);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
		}
	}
	m_FinalImage->SetData(m_ImageData);
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

    glm::vec3 color(0.0f);
    size_t bounces = 2;
    float multiplier = 1.0f;
    for(size_t i = 0; i < bounces; i++){
        Renderer::HitPayload payload = TraceRay(ray);
        if(payload.HitDistance < 0.0f){
            glm::vec3 skyColor = glm::vec3(0.0f,0.0f,0.0f);
            color += skyColor * multiplier;
            break;
        }

        glm::vec3 LightDir = glm::normalize(glm::vec3(-1, -1, -1));
        float lightIntensity =glm::max(glm::dot(payload.WorldNormal,-LightDir),0.1f);

        const Sphere& sphere = m_ActiveScene->Spheres[(size_t)payload.ObjectIndex];
        glm::vec3 sphereColor = sphere.Albedo;

        sphereColor *= lightIntensity;
        color += sphereColor * multiplier;
        multiplier *= 0.7f;
        ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
        ray.Direction = glm::reflect(ray.Direction,payload.WorldNormal);
    }

    return glm::vec4(color,1.0f);
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
