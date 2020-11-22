#include "brpch.h"
#include "GizmoLayer.h"

namespace Brbanje
{
	GizmoLayer* GizmoLayer::s_Instance = nullptr;

	GizmoLayer* GizmoLayer::Get()
	{
		if (!s_Instance)
		{
			s_Instance = new GizmoLayer();
			
		}

		return s_Instance;
	}

	GizmoLayer::~GizmoLayer()
	{
		m_Gizmos.clear();
	}

	void GizmoLayer::AddGizmo(Ref<Gizmo> gizmo)
	{
		m_Gizmos.push_back(gizmo);
	}

	void GizmoLayer::OnRender()
	{
		for (auto& gizmo : m_Gizmos)
		{
			gizmo->OnRender();
		}
	}



}