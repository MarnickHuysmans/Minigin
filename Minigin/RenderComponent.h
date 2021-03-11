#pragma once
#include "Component.h"

namespace dae
{
	class Texture2D;
	class RenderComponent : public Component
	{
	public:
		virtual void Render() const;
		void Update() override{};
		void SetTexture(const std::string& filename);

		explicit RenderComponent(const std::string& filename = "");
		virtual ~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) noexcept = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) noexcept = delete;
	protected:
		std::shared_ptr<Texture2D> m_Texture{};
	};
}