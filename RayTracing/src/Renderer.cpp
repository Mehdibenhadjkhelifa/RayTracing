#include "Renderer.h"
#include "Walnut/Random.h"

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

void Renderer::Render(glm::vec3 sphereOrigin) {
	for (uint32_t y = 0; y < m_FinalImage->GetHeight();y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
			glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(),(float)y / (float)m_FinalImage->GetHeight() };
			coord = coord * 2.0f - 1.0f;//remap from 0.0 -> 1.1 to -1.-1 -> 1.1
			m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coord, sphereOrigin, 0);
			x++;
			m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coord, sphereOrigin,1);
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

uint32_t Renderer::PerPixel(glm::vec2 coord,glm::vec3 sphereOrigin,uint32_t i)
{

	uint8_t r = (uint8_t)(coord.x * 255.0f);
	uint8_t g = (uint8_t)(coord.y * 255.0f);
	glm::vec3 rayOrigin(0.0f,0.0f,2.0f);
	glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
	float radius = 10.f;
	// (bx^2 + by^2)t^2 + 2((axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
	// a = ray origin
	// b = ray direction 
	// r = sphere radius
	// t = hit distance
	//float a = glm::dot(rayDirection, rayDirection);
	//float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	//float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;
	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * (glm::dot(rayOrigin, rayDirection) - glm::dot(sphereOrigin, rayDirection));
	float c = (-2.0f * (glm::dot(sphereOrigin, rayOrigin))) + glm::dot(sphereOrigin, sphereOrigin) - radius * radius;
	float discremenant = b * b - (4.0f * a *  c);
	if (discremenant > 0.0f) {
		float t[] = { ((-b - glm::sqrt(discremenant)) / (2 * a)),((-b + glm::sqrt(discremenant)) / (2 * a)) };
		glm::vec3 intersec = rayOrigin + (rayDirection * t[i]);
		glm::vec3 normal = intersec - sphereOrigin;
		//normal = glm::normalize(normal);
		normal = (normal * 0.5f) + 0.5f;
		uint32_t color = 0xff000000;
		color |= (uint8_t)normal.x * 255 << 16;
		color |= (uint8_t)normal.z * 255 << 8;
		color |= (uint8_t)normal.y * 255;
		return color;
		
	}

		//return 0xffff00ff;
	return 0xff000000;
}
