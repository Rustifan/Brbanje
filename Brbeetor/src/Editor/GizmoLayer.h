#pragma once
#include "Brbanje/Core/Layer.h"
#include "Brbanje/Scene/Gizmo.h"

namespace Brbanje
{
	class GizmoLayer : public Layer
	{
	private:
		std::vector<Ref<Gizmo>> m_Gizmos;
		static GizmoLayer* s_Instance;
	private:
		GizmoLayer() = default;
	public:
		static GizmoLayer* Get();
		
		
		~GizmoLayer();
		void AddGizmo(Ref<Gizmo> gizmo);
		virtual void OnRender() override;
		
	};
}