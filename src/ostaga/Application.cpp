
// Precompiled
#include "ospch.h"
//

#include <Ostaga.h>

#include "Application.h"

#include <TimeStep.h>
#include <TestingLayer.h>

#include <AudioDevice.h>
#include <Renderer.h>

namespace Ostaga {

	using namespace Audio;
	using namespace Graphics;

	Application *Application::s_Current = nullptr;

	Application::Application()
	{
		PROFILE_SESSION_BEGIN("Ostaga-Startup");
		PROFILE_FUNCTION();

		s_Current = this;

		WindowProps props = {
			1280,
			720,
			"Realms of Ostaga",
			OSTAGA_IF_DEBUG(WindowMode::Windowed, WindowMode::WindowedFullscreen),
			true 
		};

		m_Window = CreateScope<Window>(props);
		m_Window->SetEventCallback(std::bind(&::Ostaga::Application::OnEvent, this, std::placeholders::_1));

		OSTAGA_IF_DEBUG(m_ImGui = CreateScope<ImGuiSurface>(*m_Window);)

		AudioDevice::Init();
		Renderer::Init({ 10000 });
		PROFILE_SESSION_END();

	}

	Application::~Application()
	{
		Renderer::Shutdown();
		AudioDevice::Shutdown();
	}

	void Application::Update(TimeStep ts)
	{
		m_Layers.OnUpdate(ts);

		// No need to be renderering if the window is
		// minimized.
		if (!m_Iconified)
		{
			m_Layers.OnRender();

			OSTAGA_IF_DEBUG(
				m_ImGui->Begin();
				m_Layers.OnGui();
				m_ImGui->End();)
		}
	}

	void Application::OnEvent(Event &e)
	{
		if (e.GetType() == EventType::WindowClose)
			m_Running = false;

		e.Dispatch<WindowIconified>([&](WindowIconified& e){
			m_Iconified = e.iconified;
			return true;
		});

		m_Layers.OnEvent(e);
	}

	void Application::Run()
	{
		PushLayer(new TestingLayer);

		float lastTime = (float) glfwGetTime();

		m_Running = true;
		m_Window->SetVisible(true);

		while (m_Running)
		{
			float now = (float) glfwGetTime();
			TimeStep ts{now - lastTime};
			lastTime = now;

			Update(ts);
			m_Window->Update();
		}
	}
}