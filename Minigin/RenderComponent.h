#pragma once
#include "Component.h"

#include <string>
#include <memory>

namespace dae
{
	class Texture2D;
	class RenderComponent : public Component
	{
	public:
		virtual void Render() const;
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
