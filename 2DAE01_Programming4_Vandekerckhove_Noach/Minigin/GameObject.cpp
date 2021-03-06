#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "SubjectComponent.h"


dae::GameObject::GameObject()
{
	
}

dae::GameObject::~GameObject()
{
	for (BaseComponent* component : m_pComponents)
		safeDelete(component);
}

void dae::GameObject::Update()
{
	for (auto component : m_pComponents)
		component->UpdateComponent();
}

void dae::GameObject::Render() const
{
	for (auto component : m_pComponents)
		component->RenderComponent();
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::AddComponent(BaseComponent* component)
{
	m_pComponents.push_back(component);
	component->SetParentObject(this);
	component->PostAddedToGameObject();
	auto subject = GetComponent<SubjectComponent>();
	if(dynamic_cast<Observer*>(component) && subject) //Add observers to existing subject component
		subject->AddObserver(dynamic_cast<Observer*>(component));
}

