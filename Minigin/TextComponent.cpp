#include "MiniginPCH.h"
#include "TextComponent.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Util.h"

dae::TextComponent::TextComponent(const std::shared_ptr<Font>& font, const std::string& text, const SDL_Color& color)
	: m_NeedsUpdate(true), m_Text(text), m_Font(font), m_Color(color), RenderComponent()
{
}

void dae::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		if (m_Text == "")
		{
			m_Texture = nullptr;
			return;
		}
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_Texture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::SetText(const std::string& text)
{
	if (m_Text != text)
	{
		m_Text = text;
		m_NeedsUpdate = true;
	}
}

void dae::TextComponent::SetColor(const SDL_Color& color)
{
	if (m_Color != color)
	{
		m_Color = color;
		m_NeedsUpdate = true;
	}
}
