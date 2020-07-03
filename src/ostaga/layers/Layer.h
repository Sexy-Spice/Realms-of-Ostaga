#pragma once

#include <Ostaga.h>

#include <events/Event.h>
#include <events/KeyEvent.h>
#include <events/MouseEvent.h>
#include <events/SystemEvent.h>

#include <util/TimeStep.h>

namespace Ostaga {

	class Layer
	{
	public:
		// Debugging functionality
		OSTAGA_IF_DEBUG(
			Layer(const std::string &name = "Unnamed-Layer") : m_Name(name) {},
			Layer() = default;)
		//

		virtual ~Layer() = default;

		virtual void OnStart() = 0;
		virtual void OnStop() = 0;

		virtual void OnUpdate(TimeStep ts) = 0;
		virtual void OnRender() = 0;
		virtual void OnEvent(Event &e) = 0;

	public:
		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(mutable std::string m_Name;)
		OSTAGA_IF_DEBUG(mutable bool m_Visible = true;)
	};

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *overlay);

		Layer* PopLayer();
		Layer* PopOverlay();

		void OnUpdate(TimeStep ts);
		void OnRender();
		void OnEvent(Event &e);

	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*> m_Overlays;
	};
}