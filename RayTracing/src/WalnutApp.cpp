#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include "Renderer.h"
using namespace Walnut;
class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		static float sphere_x = 0.0f;
		static float sphere_y = 0.0f;
		static float sphere_z = 0.0f;
		ImGui::Begin("Settings");
		ImGui::SliderFloat("sphere_x", &sphere_x, -50.0f, 50.0f);
		ImGui::SliderFloat("sphere_y", &sphere_y, -50.0f, 50.0f);
		ImGui::SliderFloat("sphere_z", &sphere_z, -50.0f, 50.0f);
		ImGui::Text("Last render : %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render"))
			Render(glm::vec3(sphere_x, sphere_y, sphere_z));
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("ViewPort");
		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;
		auto image = m_Renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth() ,(float)image->GetHeight() },
				ImVec2(0, 1),ImVec2(1,0));
		ImGui::End();
		ImGui::PopStyleVar();
		Render(glm::vec3(sphere_x,sphere_y,sphere_z));
	}

	void Render(glm::vec3 sphere_origin){
		Timer timer;
		m_Renderer.OnResize(m_ViewportWidth,m_ViewportHeight);
		m_Renderer.Render(sphere_origin);

		m_LastRenderTime = timer.ElapsedMillis();
	}
private:
	Renderer m_Renderer;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_LastRenderTime = 0;
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