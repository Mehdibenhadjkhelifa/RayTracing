#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include "Renderer.h"
#include "imgui.h"
using namespace Walnut;
class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render : %.3fms", m_LastRenderTime);
        ImGui::SliderFloat3("Ray Origin",(float*)&_ray_origin,-5.000,5.000);
        ImGui::SliderFloat3("Sphere Origin",(float*)&_sphere_origin,-5.000,5.000);
        ImGui::SliderFloat3("Light Direction",(float*)&_light_direction,-5.000,5.000);
		if (ImGui::Button("Render"))
			Render(_ray_origin,_sphere_origin,_light_direction);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("ViewPort");
		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;
		auto image = m_Renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth() ,(float)image->GetHeight() }
		,ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();
		Render(_ray_origin,_sphere_origin,_light_direction);
	}

	void Render(glm::vec3 ray_origin,glm::vec3 sphere_origin,glm::vec3 light_direction){
		Timer timer;
		m_Renderer.OnResize(m_ViewportWidth,m_ViewportHeight);
		m_Renderer.Render(ray_origin,sphere_origin,light_direction);

		m_LastRenderTime = timer.ElapsedMillis();
	}
private:
	Renderer m_Renderer;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_LastRenderTime = 0;
    glm::vec3 _ray_origin = glm::vec3(0.f,0.f,2.f), _sphere_origin= glm::vec3(0),_light_direction = glm::vec3(-1.f);
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}
