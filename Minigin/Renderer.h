#pragma once
#include "Singleton.h"

struct SDL_Window;
struct SDL_Renderer;

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float widthScale, float heightScale, float pivotX, float pivotY) const;

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }

		void GetWindowSize(int& width, int& height) const;

	private:
		friend class Singleton<Renderer>;
		Renderer() = default;
		int GetOpenGLDriverIndex();

		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		bool m_ShowDemo = false;
	};
}

