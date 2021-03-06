#pragma once
#include "BaseComponent.h"
#include "Subject.h"

namespace dae
{
	class SubjectComponent : public BaseComponent
	{
	public:
		SubjectComponent();
		~SubjectComponent() override = default;

		SubjectComponent(const SubjectComponent& other) = delete;
		SubjectComponent(SubjectComponent&& other) noexcept = delete;
		SubjectComponent& operator=(const SubjectComponent& other) = delete;
		SubjectComponent& operator=(SubjectComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override {};
		void PostAddedToGameObject() override;

		void Notify(Event event) const;
		void AddObserver(Observer* observer);

	private:
		std::unique_ptr<Subject> m_Subject;
	};
	
}

