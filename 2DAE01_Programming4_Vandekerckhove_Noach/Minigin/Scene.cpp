#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "SceneObject.h"
using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

void Scene::Add(const std::shared_ptr<SceneObject>& object)
{
	object->SetParentScene(this);
	m_Objects.push_back(object);
}

void Scene::AddInputAction(const InputAction& action)
{
	m_InputActions.push_back(action);
}

void Scene::Update()
{
	for(auto& object : m_Objects)
	{
		object->Update();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

