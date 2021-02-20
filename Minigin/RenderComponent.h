#pragma once
#include "Component.h"

namespace dae
{
	class Texture2D;
	class RenderComponent : public Component
	{
	public:
		void Render() const override;

		void SetTexture(const std::string& filename);

		RenderComponent() = default;
		virtual ~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) noexcept = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) noexcept = delete;
	protected:
		std::shared_ptr<Texture2D> m_Texture{};
	};
}
