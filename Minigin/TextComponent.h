#pragma once
#include "RenderComponent.h"
#include <SDL_pixels.h>

namespace dae
{
	class Font;
	class TextComponent final : public RenderComponent
	{
	public:
		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

		explicit TextComponent(const std::shared_ptr<Font>& font, const std::string& text = "", const SDL_Color& color = { 255,255,255 });
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) noexcept = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) noexcept = delete;
	protected:
		bool m_NeedsUpdate;
		std::string m_Text{};
		std::shared_ptr<Font> m_Font{};
		SDL_Color m_Color;
	};
}
