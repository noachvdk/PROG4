#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"


dae::GameObject::GameObject()
	: m_Subject(std::make_unique<Subject>())
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
	if(dynamic_cast<Observer*>(component))
		m_Subject->AddObserver(dynamic_cast<Observer*>(component));
}

void dae::GameObject::Notify(Event event)
{
	m_Subject->Notify(event);
}
