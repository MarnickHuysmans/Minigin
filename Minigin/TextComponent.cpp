#include "MiniginPCH.h"
#include "TextComponent.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent(const std::shared_ptr<Font>& font, const std::string& text, const Color& color)
	: RenderComponent(), m_NeedsUpdate(true), m_Text(text), m_Font(font), m_Color(color)
{
}

void dae::TextComponent::Update()
{
	if (!m_NeedsUpdate)
	{
		return;
	}
	if (m_Text == "")
	{
		m_Texture = nullptr;
		return;
	}
	SDL_Color color{ m_Color.r, m_Color.g, m_Color.b, m_Color.a };
	const auto surface = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
	if (surface == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surface);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surface);
	m_Texture = std::make_shared<Texture2D>(texture);
	m_NeedsUpdate = false;
}

void dae::TextComponent::SetText(const std::string& text)
{
	if (m_Text != text)
	{
		m_Text = text;
		m_NeedsUpdate = true;
	}
}

void dae::TextComponent::SetColor(const Color& color)
{
	if (m_Color != color)
	{
		m_Color = color;
		m_NeedsUpdate = true;
	}
}
