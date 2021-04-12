#pragma once
#include "RenderComponent.h"

namespace dae
{
	class Font;

	struct Color
	{
		uint8_t r{};
		uint8_t g{};
		uint8_t b{};
		uint8_t a{};


		friend bool operator==(const Color& lhs, const Color& rhs)
		{
			return lhs.r == rhs.r
				&& lhs.g == rhs.g
				&& lhs.b == rhs.b
				&& lhs.a == rhs.a;
		}

		friend bool operator!=(const Color& lhs, const Color& rhs)
		{
			return !(lhs == rhs);
		}
	};
	
	class TextComponent final : public RenderComponent
	{
	public:
		void Update() override;

		void SetText(const std::string& text);
		void SetColor(const Color& color);

		explicit TextComponent(const std::shared_ptr<Font>& font, const std::string& text = "", const Color& color = { 255,255,255 });
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) noexcept = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) noexcept = delete;
	protected:
		bool m_NeedsUpdate;
		std::string m_Text{};
		std::shared_ptr<Font> m_Font{};
		Color m_Color;
	};
}
