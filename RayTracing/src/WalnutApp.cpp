#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include "Renderer.h"
#include "Camera.h"
#include "imgui.h"
using namespace Walnut;
class ExampleLayer : public Walnut::Layer
{
public:
    ExampleLayer():
        m_Camera(45.f,0.1f,100.f){}
    virtual void OnUpdate(float ts) override{
        m_Camera.OnUpdate(ts);
    
    }
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render : %.3fms", m_LastRenderTime);
        ImGui::SliderFloat3("Ray Origin",(float*)&_ray_origin,-5.000,5.000);
        ImGui::SliderFloat3("Sphere Origin",(float*)&_sphere_origin,-5.000,5.000);
        ImGui::SliderFloat3("Sphere Color",(float*)&_sphere_color,-1.000,1.000);
        ImGui::SliderFloat3("Light Direction",(float*)&_light_direction,-5.000,5.000);
		if (ImGui::Button("Render"))
			Render();
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
		Render();
	}

	void Render(){
		Timer timer;
		m_Renderer.OnResize(m_ViewportWidth,m_ViewportHeight);
		m_Camera.OnResize(m_ViewportWidth,m_ViewportHeight);
		m_Renderer.Render(m_Camera,_ray_origin,_sphere_origin,_light_direction,_sphere_color);

		m_LastRenderTime = timer.ElapsedMillis();
	}
private:
	Renderer m_Renderer;
    Camera m_Camera;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_LastRenderTime = 0;
    glm::vec3 _ray_origin = glm::vec3(0.f,0.f,2.f), _sphere_origin= glm::vec3(0),_light_direction = glm::vec3(-1.f),_sphere_color = glm::vec3(0.f);
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
