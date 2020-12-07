#include "brpch.h"
#include "Scene.h"
#include "imgui.h"
#include "Brbanje/Renderer/Renderer2D.h"
#include "Entity.h"
#include "Brbanje/Scene/ScriptableEntity.h"
#include "Components.h"
#include "Brbanje/Core/Input.h"
#include "../../Brbeetor/src/Editor/GizmoLayer.h"
#include "Brbanje/Core/KeyCodes.h"

namespace Brbanje
{
	Scene::Scene(): m_CameraController(SceneCameraController(this)), m_SubTexEditor(this)
	{
		m_MainCamera = m_CameraController.GetSceneCamera();
		m_MainCameraTransform = m_CameraController.GetCameraTransform();

		m_Gizmo = std::make_shared<Gizmo>(this);
		

		GizmoLayer::Get()->AddGizmo(m_Gizmo);
	}

	Scene::~Scene()
	{
		m_TextureMap.clear();
	}

	void Scene::OnUpdate(Timestep ts)
	{
		

		m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& component)
			{
				if (!component.instance)
				{
					component.instance = component.InstatiateFunction();
					component.instance->m_Entity = Entity(entity, this);
					component.instance->OnCreate();
					
					

					
				}

				component.instance->OnUpdate(ts);
			});

		if (!m_EditorView)
		{
			m_MainCamera = nullptr;
			glm::mat4 transform;
			{
				auto group = m_Registry.group<>(entt::get<TransformComponent, CameraComponent>);
				for (auto entity : group)
				{
					auto [trasformComp, camera] = group.get<TransformComponent, CameraComponent>(entity);
					if (camera.primary)
					{
						m_MainCamera = &camera.camera;

						transform = trasformComp.GetTransform();
						m_MainCameraTransform = &trasformComp;
					}

				}
			}
		}
		

		if (m_MainCamera)
		{
			if (m_EditorView)
			{
				m_CameraController.OnUpdate(ts);
			}
			

			Renderer2D::BeginScene(m_MainCamera->GetProjection(), m_MainCameraTransform->GetTransform());
			
			
			
			
			if (m_Panel->m_EntitySelectionContext)
			{
				
				m_Gizmo->OnUpdate(ts);
			}
			
			m_Gizmo->OnMove();
			
			

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
			
			//is something selected
			bool Selected = false;

			for (auto entity : group)
			{
				
				if (m_Registry.has<SpriteComponent>(entity))
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

					
					//Selection 
					if (ImGui::IsMouseClicked(0) && m_MouseHoveredOnVIewport && !m_Gizmo->isWorking())
					{
						
						
							glm::vec2 mousePos = GetSceneMousePos();
							if (IsClicked(transform, mousePos))
							{
								
								Selected = true;
								
									if (m_Panel->m_EntitySelectionContext)
									{
										auto& selectedTransform = m_Panel->m_EntitySelectionContext.GetComponent<TransformComponent>();
										if (!IsClicked(selectedTransform, mousePos) || transform.position.z > selectedTransform.position.z)
										{
											m_Panel->m_EntitySelectionContext = Entity(entity, this);

										}
										
									}
									else
									{
										m_Panel->m_EntitySelectionContext = Entity(entity, this);
										
									}

								
							

							}
							

							
					}

					//Sprite
					
					if (sprite.texture)
					{
						if (!sprite.subTexture)
						{
							Renderer2D::DrawQuad(transform.GetTransform(), sprite.texture, sprite.tilingFactor, sprite.color);

						}
						else
						{
							Renderer2D::DrawRotatedQuad(transform.position, transform.size, transform.rotation.z, sprite.subTexture);
						}
					}
					else
					{
						Renderer2D::DrawQuad(transform.GetTransform(), sprite);

					}
					
					
					
					

				}

				
			}

			//Deselect Entity

			if (ImGui::IsMouseClicked(0) && !Selected && !m_Gizmo->isWorking() && m_MouseHoveredOnVIewport || Input::IsKeyPressed(BR_KEY_ESCAPE))
			{
				m_Panel->m_EntitySelectionContext = {};
			}
			

			
			Renderer2D::EndScene();
		}
		
		
		
		m_SubTexEditor.Update(ts);


	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		static uint32_t entittyID = 0;
		Entity entity = Entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		std::string name = tag == std::string() ? "Entity" : tag;
		auto& tagComp = entity.AddComponent<TagComponent>(name);
		tagComp.Entity_ID = entittyID++;
		

		return entity;
	}
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewPortHeight = height;
		auto cameraComponents = m_Registry.view<CameraComponent>();
		for (entt::entity entt : cameraComponents)
		{
			CameraComponent& cam = cameraComponents.get<CameraComponent>(entt);
			if (!cam.fixedAspectRatio)
			{
				cam.camera.ResizeViewport(width, height);
				
			}
		}
		if (m_EditorView)
		{
			m_MainCamera->ResizeViewport(width, height);
		}
	}	

	void Scene::NewScene()
	{
		
		m_Panel->m_EntitySelectionContext = {};
		m_Registry.clear();
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	glm::vec2 Scene::GetSceneMousePos()
	{
		if (m_MainCamera)
		{

			SceneCamera::Bounds bounds = m_MainCamera->GetBounds();
			ImGuiIO& io = ImGui::GetIO();
			auto [x, y] = io.MousePos;
			x -= m_ViewportWindowPos.x;
			y = m_ViewPortHeight - y + m_ViewportWindowPos.y;
			x /= m_ViewportWidth / 2;
			y /= m_ViewPortHeight / 2;
			x -= 1;
			y -= 1;
			x *= bounds.right;
			y *= bounds.up;
			glm::vec4 mousePos{ x,y,0.0f,1.0f };
			mousePos = m_MainCameraTransform->GetTransform() * mousePos;
			
			return { mousePos.x,mousePos.y };
		}
		return { 0,0 };
	}

	bool Scene::IsClicked(TransformComponent& transform, const glm::vec2& mousePos)
	{
		return
			mousePos.x > transform.position.x - glm::abs(transform.size.x) / 2 &&
			mousePos.x < transform.position.x + glm::abs(transform.size.x) / 2 &&
			mousePos.y > transform.position.y - glm::abs(transform.size.y) / 2 &&
			mousePos.y < transform.position.y + glm::abs(transform.size.y) / 2;

	}

	bool Scene::IsClicked(const glm::vec3& position, const glm::vec2& size, const glm::vec2& mousePos)
	{
		return
			mousePos.x > position.x - glm::abs(size.x )/ 2 &&
			mousePos.x < position.x + glm::abs(size.x) / 2 &&
			mousePos.y > position.y - glm::abs(size.y) / 2 &&
			mousePos.y < position.y + glm::abs(size.y) / 2;
	}

	Ref<Texture2D> Scene::GetTextureFromTextureMap(const std::string& filePath)
	{
		if (m_TextureMap.find(filePath) != m_TextureMap.end())
		{
			

			return m_TextureMap[filePath];
		}

		
		Ref<Texture2D> tex = m_TextureMap[filePath] = Texture2D::Create(filePath);
		return tex;

	}

	void Scene::OnImGuiRender()
	{
		m_SubTexEditor.OnImGuiRender();
	}

	void Scene::OnEvent(Event& event)
	{
		m_CameraController.OnEvent(event);
		m_SubTexEditor.OnEvent(event);

	}

	template<typename T>
	void Scene::OnComponentAdded(T& component, Entity entity)
	{
		
	}

	template<>
	void Scene::OnComponentAdded<SpriteComponent>(SpriteComponent& component, Entity entity)
	{
		

	}
	
	template<>
	void Scene::OnComponentAdded<CameraComponent>(CameraComponent& component, Entity entity)
	{
		component.camera.ResizeViewport(m_ViewportWidth, m_ViewPortHeight);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(TagComponent& component, Entity entity)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(NativeScriptComponent& component, Entity entity)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(TransformComponent& component, Entity entity)
	{

	}

	std::ostream& operator <<(std::ostream& stream, const TagComponent& tag) { stream << tag.tag; return stream; }

}