#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "Transform.h"
#include "GameObject.h"

using namespace dae;

BaseComponent::BaseComponent()
	:m_pParentObj(nullptr)
{}

void BaseComponent::SetParentObject(GameObject* pParentObj)
{
	m_pParentObj = pParentObj;
}
