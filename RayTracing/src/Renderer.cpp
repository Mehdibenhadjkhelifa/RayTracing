#include "Renderer.h"
#include "Walnut/Random.h"

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

void Renderer::Render(const Camera& camera,glm::vec3 ray_origin,glm::vec3 sphere_origin,glm::vec3 light_direction,glm::vec3 sphere_color) {
    Ray ray;
    ray.Origin = camera.GetPosition();
            
	for (uint32_t y = 0; y < m_FinalImage->GetHeight();y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
            ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];
			glm::vec4 color = TraceRay(ray,ray_origin,sphere_origin,light_direction,sphere_color);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::TraceRay(const Ray& ray,glm::vec3 ray_origin,glm::vec3 sphere_origin,glm::vec3 light_direction,glm::vec3 sphere_color)
{

	//glm::vec3 rayOrigin = ray_origin;
	//glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
	//glm::vec3 sphereOrigin(0.0f, 2.0f, -3.0f);
	glm::vec3 sphereOrigin = sphere_origin;
	float radius = 1.f;
	// (bx^2 + by^2)t^2 + 2((axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
	// a = ray origin
	// b = ray direction 
	// r = sphere radius
	// t = hit distance
	//float a = glm::dot(rayDirection, rayDirection);
	//float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	//float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;
    float a = glm::dot(ray.Direction, ray.Direction);
	float b = 2.0f * (glm::dot(ray.Origin, ray.Direction) - glm::dot(sphereOrigin, ray.Direction));
	float c = glm::dot(ray.Origin,ray.Origin) + (-2.0f * (glm::dot(sphereOrigin, ray.Origin))) + glm::dot(sphereOrigin, sphereOrigin) - radius * radius;
	float discremenant = b * b - 4.0f * a *  c;
	if (discremenant < 0.0f)
		return glm::vec4(0,0,0,1);

	float closest_t = (-b - glm::sqrt(discremenant)) / (2.0f * a);

	glm::vec3 hitPoint = ray.Origin + ray.Direction * closest_t;
	glm::vec3 normal = glm::normalize(hitPoint- sphereOrigin);
    //glm::vec3 LightDir = glm::normalize(glm::vec3(-1, -1, -1));
    glm::vec3 LightDir = glm::normalize(light_direction);


	//glm::vec3 sphereColor(1, 0, 1);
    glm::vec3 sphereColor = sphere_color;
    //normal * 0.5f + 0.5 to map normal from -1/1 -> 0/1 
    //normal is representing the x , y ,z axis as color values here
	sphereColor = (sphereColor * 0.5f + 0.5f) * (glm::max(glm::dot(normal,-LightDir),0.1f));
	return glm::vec4(sphereColor,1.0f);
}
